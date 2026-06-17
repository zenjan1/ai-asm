#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_BODIES 16
#define MAX_PARTS 64
#define MAX_FLUID 32
#define MAX_CLOTH 25
#define MAX_SOFT 4
#define MAX_CONS 8
#define MAX_EMIT 4
#define MAX_FF 4
#define PI 3.14159265F
#define TWO_PI 6.28318530F

static float fsin(float x) {
    while (x>PI) x-=TWO_PI; while (x<-PI) x+=TWO_PI;
    float x2=x*x; return x-x2*x/6.0F+x2*x2*x/120.0F;
}
static float fcos(float x) { return fsin(x+PI*0.5F); }

typedef struct { float x,y,z; } V3;
typedef struct { float x,y,z,w; } Quat;
static float vd(V3 a,V3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
static float vl(V3 v){return __builtin_sqrtf(vd(v,v));}
static V3 va(V3 a,V3 b){return(V3){a.x+b.x,a.y+b.y,a.z+b.z};}
static V3 vs(V3 a,V3 b){return(V3){a.x-b.x,a.y-b.y,a.z-b.z};}
static V3 vm(V3 v,float s){return(V3){v.x*s,v.y*s,v.z*s};}
static V3 vx(V3 a,V3 b){return(V3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
static V3 vn(V3 v){float l=vl(v);return(l>1e-06F)?vm(v,1.0F/l):(V3){0,0,0};}
static V3 vlerp(V3 a,V3 b,float t){return(V3){a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t};}

typedef struct { float density,friction,restitution,elasticity,heat_cond,specific_heat; char name[12]; } Mat;
static const Mat g_mat[]={
    {7800,0.60F,0.30F,0.20F,80,450,"Steel"},
    {2700,0.55F,0.25F,0.35F,237,900,"Aluminum"},
    {1000,0.90F,0.10F,0.05F,0.6F,4186,"Rubber"},
    {2500,0.65F,0.15F,0.10F,1.4F,840,"Glass"},
    {900,0.40F,0.20F,0.15F,0.15F,1800,"Wood"},
    {1025,0,0,0,0.6F,4000,"Water"}
};
#define NMAT 6

typedef struct { V3 pos,vel,ang; Quat ori; float mass,inv,rest,fric; int mat,active; } RB;
typedef struct { V3 n; float depth; V3 pt; int a,b; } Con;
typedef struct { V3 min,max; } AABB;
typedef struct { V3 pos,vel,frc; float density,pressure,mass; } FP;
typedef struct { V3 pos,vel,frc; float mass; int fixed; } SN;
typedef struct { int a,b; float rest,stiff,damp; } Sp;
typedef struct { SN nd[12]; Sp sp[20]; int nn,ns; float vol; int active; } SB;
typedef struct { V3 pos,vel,frc; float inv_mass; int pinned; } CN;
typedef struct { int a,b; float rest; int type; } CC;
typedef struct { CN nd[MAX_CLOTH]; CC con[50]; int nx,ny,nc; int active; } CL;
typedef struct { V3 pos,vel; float life,mlife,sz; int alive; } Pt;
typedef struct { V3 o,dir; float spd,spr,rate,timer; int lm,active; } Em;
typedef struct { V3 o; float rad,str; int type,active; } FF;

typedef struct {
    RB bodies[MAX_BODIES]; int nb; Con cons[MAX_CONS]; int nc;
    FP fluid[MAX_FLUID]; int nf; SB soft[MAX_SOFT]; int ns;
    CL cloth; Pt parts[MAX_PARTS]; int np;
    Em emit[MAX_EMIT]; int ne; FF fields[MAX_FF]; int nff;
    float grav,dt; int step; float srd,sgc,sv,sh;
} Sim;

static char gb[64];
static void ps(const char *s){host_print(s);}
static void pi(int v){
    if(!v){gb[0]='0';gb[1]='\n';gb[2]=0;host_print(gb);return;}
    int i=0,neg=0; if(v<0){neg=1;v=-v;}
    while(v>0&&i<20){gb[i++]='0'+(v%10);v/=10;}
    if(neg)gb[i++]='-'; gb[i++]='\n';gb[i]=0;
    for(int j=0;j<i/2;j++){char t=gb[j];gb[j]=gb[i-1-j];gb[i-1-j]=t;}
    host_print(gb);
}
static void pf(float v){
    int w=(int)v,f=(int)((v-(float)w)*100); if(f<0)f=-f;
    int i=0,neg=0; if(v<0){neg=1;w=-w;}
    if(!w)gb[i++]='0';
    else{int ww=w;char t[12];int ti=0;while(ww>0&&ti<10){t[ti++]='0'+(ww%10);ww/=10;}
    for(int j=ti-1;j>=0;j--)gb[i++]=t[j];}
    gb[i++]='.';gb[i++]='0'+f/10;gb[i++]='0'+f%10;
    if(neg){for(int j=i;j>0;j--)gb[j]=gb[j-1];gb[0]='-';i++;}
    gb[i++]='\n';gb[i]=0;host_print(gb);
}
static Quat qi(void){return(Quat){0,0,0,1};}

static float sp6(float r2,float h){
    float h2=h*h;if(r2>=h2)return 0;float d=h2-r2,h9=h*h*h*h*h*h*h*h*h;
    return 315.0F/(64.0F*PI*h9)*d*d*d;
}
static V3 spg(V3 rj,float r,float h){
    if(r>=h||r<1e-06F)return(V3){0,0,0};
    float d=h-r,h6=h*h*h*h*h*h;return vm(rj,-45.0F/(PI*h6)*d*d/r);
}
static float spl(float r,float h){
    if(r>=h)return 0;float h6=h*h*h*h*h*h;return 45.0F/(PI*h6)*(h-r);
}
static AABB mkb(V3 p,V3 he){
    AABB b;b.min=vs(p,he);b.max=va(p,he);return b;
}
static int abhit(AABB a,AABB b){
    return(a.min.x<=b.max.x&&a.max.x>=b.min.x)&&(a.min.y<=b.max.y&&a.max.y>=b.min.y)
        &&(a.min.z<=b.max.z&&a.max.z>=b.min.z);
}

static int rbmk(Sim *s,V3 p,float mass,float rest,float fric,int mat){
    if(s->nb>=MAX_BODIES)return -1; int i=s->nb++; RB *b=&s->bodies[i];
    b->pos=p;b->vel=(V3){0,0,0};b->ang=(V3){0,0,0};b->ori=qi();
    b->mass=mass;b->inv=(mass>0)?1.0F/mass:0;b->rest=rest;b->fric=fric;b->mat=mat;b->active=1;
    return i;
}
static void rbgrav(Sim *s){
    for(int i=0;i<s->nb;i++){RB *b=&s->bodies[i];if(b->active&&b->inv>0)b->vel.y+=s->grav*s->dt;}
}
static void rbdet(Sim *s){
    s->nc=0;V3 he={0.5F,0.5F,0.5F};
    for(int i=0;i<s->nb;i++)for(int j=i+1;j<s->nb;j++){
        RB *a=&s->bodies[i],*b=&s->bodies[j];
        if(!a->active||!b->active)continue;
        if(!abhit(mkb(a->pos,he),mkb(b->pos,he)))continue;
        if(s->nc>=MAX_CONS)continue;
        Con *c=&s->cons[s->nc++];c->a=i;c->b=j;
        V3 d=vs(b->pos,a->pos);float dl=vl(d);
        c->n=(dl>1e-06F)?vn(d):(V3){0,1,0};
        c->depth=(1.0F-dl>0)?1.0F-dl:0.01F;c->pt=vlerp(a->pos,b->pos,0.5F);
    }
}
static void rbslv(Sim *s){
    for(int i=0;i<s->nc;i++){
        Con *ct=&s->cons[i];RB *a=&s->bodies[ct->a],*b=&s->bodies[ct->b];
        V3 rv=vs(b->vel,a->vel);float vn2=vd(rv,ct->n);if(vn2>0)continue;
        float e=(a->rest+b->rest)*0.5F,j=-(1.0F+e)*vn2/(a->inv+b->inv);
        V3 imp=vm(ct->n,j);a->vel=vs(a->vel,vm(imp,a->inv));b->vel=va(b->vel,vm(imp,b->inv));
        rv=vs(b->vel,a->vel);V3 tn=vs(rv,vm(ct->n,vd(rv,ct->n)));float tl=vl(tn);
        if(tl>1e-06F){
            tn=vm(tn,1.0F/tl);float jt=-vd(rv,tn)/(a->inv+b->inv),mu=(a->fric+b->fric)*0.5F;
            if(jt>j*mu)jt=j*mu;else if(jt<-j*mu)jt=-j*mu;
            V3 fi=vm(tn,jt);a->vel=vs(a->vel,vm(fi,a->inv));b->vel=va(b->vel,vm(fi,b->inv));
        }
        float cr=(ct->depth-0.01F)*0.4F/(a->inv+b->inv);V3 cv=vm(ct->n,cr);
        a->pos=vs(a->pos,vm(cv,a->inv));b->pos=va(b->pos,vm(cv,b->inv));
    }
}
static void rbint(Sim *s){
    for(int i=0;i<s->nb;i++){
        RB *b=&s->bodies[i];if(!b->active)continue;
        b->pos.x+=b->vel.x*s->dt;b->pos.y+=b->vel.y*s->dt;b->pos.z+=b->vel.z*s->dt;
        Quat q=b->ori,dq;
        dq.w=-0.5F*(q.x*b->ang.x+q.y*b->ang.y+q.z*b->ang.z);
        dq.x= 0.5F*(q.w*b->ang.x+q.y*b->ang.z-q.z*b->ang.y);
        dq.y= 0.5F*(q.w*b->ang.y+q.z*b->ang.x-q.x*b->ang.z);
        dq.z= 0.5F*(q.w*b->ang.z+q.x*b->ang.y-q.y*b->ang.x);
        q.w+=dq.w*s->dt;q.x+=dq.x*s->dt;q.y+=dq.y*s->dt;q.z+=dq.z*s->dt;
        float nl=__builtin_sqrtf(q.w*q.w+q.x*q.x+q.y*q.y+q.z*q.z);
        if(nl>1e-06F){q.w/=nl;q.x/=nl;q.y/=nl;q.z/=nl;} b->ori=q;
    }
}

static void fladd(Sim *s,V3 o,int nx,int ny,float sp){
    for(int ix=0;ix<nx;ix++)for(int iy=0;iy<ny;iy++){
        if(s->nf>=MAX_FLUID)return;FP *p=&s->fluid[s->nf++];
        p->pos.x=o.x+ix*sp;p->pos.y=o.y+iy*sp;p->pos.z=o.z;
        p->vel=(V3){0,0,0};p->frc=(V3){0,0,0};p->density=0;p->pressure=0;
        p->mass=s->srd*sp*sp*sp;
    }
}
static void flden(Sim *s){
    float h=s->sh;
    for(int i=0;i<s->nf;i++){
        s->fluid[i].density=0;
        for(int j=0;j<s->nf;j++){V3 r=vs(s->fluid[i].pos,s->fluid[j].pos);
            s->fluid[i].density+=s->fluid[j].mass*sp6(vd(r,r),h);}
        s->fluid[i].pressure=s->sgc*(s->fluid[i].density-s->srd);
    }
}
static void flfrc(Sim *s){
    float h=s->sh;
    for(int i=0;i<s->nf;i++){
        V3 fp={0,0,0},fv={0,0,0};
        for(int j=0;j<s->nf;j++){if(i==j)continue;
            V3 r=vs(s->fluid[i].pos,s->fluid[j].pos);float rl=vl(r);
            if(rl<h&&s->fluid[j].density>1e-06F){
                V3 g=spg(r,rl,h);float pa=(s->fluid[i].pressure+s->fluid[j].pressure)*0.5F;
                fp=vs(fp,vm(g,s->fluid[j].mass*pa/s->fluid[j].density));
                float lap=spl(rl,h);V3 dv=vs(s->fluid[j].vel,s->fluid[i].vel);
                fv=va(fv,vm(dv,s->sv*s->fluid[j].mass*lap/s->fluid[j].density));
                if(rl>h*0.5F)fp=vs(fp,vm(vn(r),-0.001F*s->fluid[j].mass));
            }
        }
        float di=(s->fluid[i].density>1e-06F)?s->fluid[i].density:1.0F;
        s->fluid[i].frc=va(vm(fp,di),vm(fv,di));s->fluid[i].frc.y+=s->fluid[i].mass*s->grav;
    }
}
static void flstp(Sim *s){
    for(int i=0;i<s->nf;i++){FP *p=&s->fluid[i];
        float di=(p->density>1e-06F)?p->density:1.0F;
        p->vel=va(p->vel,vm(vm(p->frc,1.0F/di),s->dt));p->pos=va(p->pos,vm(p->vel,s->dt));
        if(p->pos.y<0){p->pos.y=0;p->vel.y*=-0.3F;}
        if(p->pos.x<-1){p->pos.x=-1;p->vel.x*=-0.3F;}
        if(p->pos.x>1){p->pos.x=1;p->vel.x*=-0.3F;}
    }
}

static int sbmk(Sim *s,V3 o,float sz,float stiff,float damp){
    if(s->ns>=MAX_SOFT)return -1;int idx=s->ns++;SB *sb=&s->soft[idx];
    sb->nn=8;sb->ns=0;sb->active=1;sb->vol=sz*sz*sz;float h=sz*0.5F;
    V3 c[8]={{o.x-h,o.y-h,o.z-h},{o.x+h,o.y-h,o.z-h},{o.x+h,o.y+h,o.z-h},{o.x-h,o.y+h,o.z-h},
             {o.x-h,o.y-h,o.z+h},{o.x+h,o.y-h,o.z+h},{o.x+h,o.y+h,o.z+h},{o.x-h,o.y+h,o.z+h}};
    for(int i=0;i<8;i++){sb->nd[i].pos=c[i];sb->nd[i].vel=(V3){0,0,0};
        sb->nd[i].frc=(V3){0,0,0};sb->nd[i].mass=1.0F;sb->nd[i].fixed=0;}
    sb->nd[0].fixed=1;
    int e[12][2]={{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
    for(int i=0;i<12;i++){sb->sp[i].a=e[i][0];sb->sp[i].b=e[i][1];
        sb->sp[i].rest=vl(vs(c[e[i][0]],c[e[i][1]]));sb->sp[i].stiff=stiff;sb->sp[i].damp=damp;}
    int cr[6][2]={{0,2},{1,3},{4,6},{5,7},{0,5},{2,7}};
    for(int i=0;i<6;i++){int si=12+i;sb->sp[si].a=cr[i][0];sb->sp[si].b=cr[i][1];
        sb->sp[si].rest=vl(vs(c[cr[i][0]],c[cr[i][1]]));
        sb->sp[si].stiff=stiff*0.5F;sb->sp[si].damp=damp*0.5F;}
    sb->ns=18;return idx;
}
static void sbstp(Sim *s,int idx){
    SB *sb=&s->soft[idx];if(!sb->active)return;
    for(int i=0;i<sb->nn;i++)sb->nd[i].frc=(V3){0,sb->nd[i].mass*s->grav,0};
    for(int i=0;i<sb->ns;i++){Sp *sp=&sb->sp[i];SN *na=&sb->nd[sp->a],*nb=&sb->nd[sp->b];
        V3 d=vs(nb->pos,na->pos);float dl=vl(d);if(dl<1e-06F)continue;V3 dir=vm(d,1.0F/dl);
        float disp=dl-sp->rest,df=sp->damp*vd(vs(nb->vel,na->vel),dir);
        V3 f=vm(dir,sp->stiff*disp+df);na->frc=va(na->frc,f);nb->frc=vs(nb->frc,f);}
    if(sb->nn>=4){V3 e1=vs(sb->nd[1].pos,sb->nd[0].pos),e2=vs(sb->nd[3].pos,sb->nd[0].pos),
        e3=vs(sb->nd[4].pos,sb->nd[0].pos);float cv=vd(vx(e1,e2),e3);if(cv<0)cv=-cv;
        if(cv>1e-06F&&sb->vol>1e-06F){float exc=(sb->vol/cv-1.0F)*50.0F;V3 cen={0,0,0};
        for(int i=0;i<sb->nn;i++)cen=va(cen,sb->nd[i].pos);cen=vm(cen,1.0F/(float)sb->nn);
        for(int i=0;i<sb->nn;i++)if(!sb->nd[i].fixed)
            sb->nd[i].frc=va(sb->nd[i].frc,vm(vs(sb->nd[i].pos,cen),exc));}}
    for(int i=0;i<sb->nn;i++)for(int j=i+1;j<sb->nn;j++){
        V3 d=vs(sb->nd[j].pos,sb->nd[i].pos);float dl=vl(d);
        if(dl<0.05F&&dl>1e-06F){V3 dir=vm(d,1.0F/dl);float rp=100.0F*(0.05F-dl);
        sb->nd[j].frc=va(sb->nd[j].frc,vm(dir,rp));sb->nd[i].frc=vs(sb->nd[i].frc,vm(dir,rp));}}
    for(int i=0;i<sb->nn;i++){if(sb->nd[i].fixed)continue;
        sb->nd[i].vel=va(sb->nd[i].vel,vm(vm(sb->nd[i].frc,1.0F/sb->nd[i].mass),s->dt));
        sb->nd[i].vel=vm(sb->nd[i].vel,0.99F);sb->nd[i].pos=va(sb->nd[i].pos,vm(sb->nd[i].vel,s->dt));
        if(sb->nd[i].pos.y<0){sb->nd[i].pos.y=0;sb->nd[i].vel.y*=-0.2F;}}
}

static void clmk(Sim *s,V3 o,float sz,int nx,int ny){
    s->cloth.nx=nx;s->cloth.ny=ny;s->cloth.active=1;s->cloth.nc=0;
    float dx=sz/(float)(nx-1),dy=sz/(float)(ny-1);
    for(int iy=0;iy<ny;iy++)for(int ix=0;ix<nx;ix++){int id=iy*nx+ix;if(id>=MAX_CLOTH)break;
        CN *n=&s->cloth.nd[id];n->pos.x=o.x+ix*dx;n->pos.y=o.y+iy*dy;n->pos.z=o.z;
        n->vel=(V3){0,0,0};n->frc=(V3){0,0,0};n->inv_mass=1.0F;
        n->pinned=(iy==ny-1&&(ix==0||ix==nx-1));if(n->pinned)n->inv_mass=0;}
    for(int iy=0;iy<ny;iy++)for(int ix=0;ix<nx;ix++){int id=iy*nx+ix;
        if(ix<nx-1){int c=s->cloth.nc++;s->cloth.con[c]=(CC){id,id+1,dx,0};}
        if(iy<ny-1){int c=s->cloth.nc++;s->cloth.con[c]=(CC){id,id+nx,dy,0};}
        if(ix<nx-1&&iy<ny-1){int c=s->cloth.nc++;
            s->cloth.con[c]=(CC){id,id+nx+1,__builtin_sqrtf(dx*dx+dy*dy),1};}
        if(ix<nx-2){int c=s->cloth.nc++;s->cloth.con[c]=(CC){id,id+2,dx*2.0F,2};}}
}
static void clstp(Sim *s){
    if(!s->cloth.active)return;CL *cl=&s->cloth;int tot=cl->nx*cl->ny;
    if(tot>MAX_CLOTH)tot=MAX_CLOTH;
    float wx=0.3F*fsin((float)s->step*0.05F),wz=0.2F*fcos((float)s->step*0.07F);
    for(int i=0;i<tot;i++)cl->nd[i].frc=(V3){wx,s->grav,wz};
    for(int it=0;it<3;it++)for(int c=0;c<cl->nc;c++){CC *cc=&cl->con[c];
        CN *na=&cl->nd[cc->a],*nb=&cl->nd[cc->b];V3 d=vs(nb->pos,na->pos);float dl=vl(d);
        if(dl<1e-06F)continue;float st=(cc->type==0)?1.0F:(cc->type==1)?0.5F:0.3F;
        V3 co=vm(d,(dl-cc->rest)/dl*0.5F*st);
        if(na->inv_mass>0)na->pos=va(na->pos,co);if(nb->inv_mass>0)nb->pos=vs(nb->pos,co);}
    for(int i=0;i<tot;i++){CN *n=&cl->nd[i];if(n->inv_mass==0)continue;
        n->vel=va(n->vel,vm(vm(n->frc,n->inv_mass),s->dt));n->vel=vm(n->vel,0.98F);
        n->pos=va(n->pos,vm(n->vel,s->dt));
        if(n->pos.z<0){n->pos.z=0;n->vel.z*=-0.1F;}if(n->pos.y<0){n->pos.y=0;n->vel.y*=-0.2F;}}
}

static int emmk(Sim *s,V3 o,V3 d,float spd,float spr,float rate){
    if(s->ne>=MAX_EMIT)return -1;int i=s->ne++;Em *e=&s->emit[i];
    e->o=o;e->dir=vn(d);e->spd=spd;e->spr=spr;e->rate=rate;e->timer=0;e->lm=200;e->active=1;
    return i;
}
static int ffmk(Sim *s,V3 o,float rad,float str,int type){
    if(s->nff>=MAX_FF)return -1;int i=s->nff++;FF *f=&s->fields[i];
    f->o=o;f->rad=rad;f->str=str;f->type=type;f->active=1;return i;
}
static void ptstp(Sim *s){
    for(int e=0;e<s->ne;e++){Em *em=&s->emit[e];if(!em->active)continue;
        em->timer+=s->dt;float iv=1.0F/em->rate;
        while(em->timer>=iv&&s->np<MAX_PARTS){em->timer-=iv;Pt *p=&s->parts[s->np++];
            p->pos=em->o;float ang=(float)s->step*0.07F;
            p->vel.x=em->dir.x*em->spd+em->spr*fsin(ang*3.0F);
            p->vel.y=em->dir.y*em->spd+em->spr*fcos(ang*5.0F)*0.5F;
            p->vel.z=em->dir.z*em->spd+em->spr*fsin(ang*2.0F)*0.3F;
            p->life=(float)em->lm*s->dt*5.0F;p->mlife=p->life;p->sz=0.02F;p->alive=1;}}
    for(int i=0;i<s->np;i++){Pt *p=&s->parts[i];if(!p->alive)continue;
        p->life-=s->dt;if(p->life<=0){p->alive=0;continue;}
        V3 acc={0,s->grav,0};
        for(int f=0;f<s->nff;f++){FF *ff=&s->fields[f];if(!ff->active)continue;
            V3 d=vs(ff->o,p->pos);float dl=vl(d);
            if(dl<ff->rad&&dl>1e-06F){V3 dir=vm(d,1.0F/dl);float fo=1.0F-dl/ff->rad;
                if(ff->type==0)acc=va(acc,vm(dir,ff->str*fo));
                else if(ff->type==1)acc=vs(acc,vm(dir,ff->str*fo));
                else if(ff->type==2)acc=va(acc,vm(vx(dir,(V3){0,1,0}),ff->str*fo));}}
        p->vel=va(p->vel,vm(acc,s->dt));p->vel=vm(p->vel,0.99F);
        p->pos=va(p->pos,vm(p->vel,s->dt));if(p->pos.y<0){p->pos.y=0;p->vel.y*=-0.4F;}}
    int w=0;for(int i=0;i<s->np;i++)if(s->parts[i].alive){if(w!=i)s->parts[w]=s->parts[i];w++;}
    s->np=w;
}

static void sim_init(Sim *s){
    s->grav=-9.81F;s->dt=0.016F;s->step=0;
    s->nb=0;s->nc=0;s->nf=0;s->ns=0;s->np=0;s->ne=0;s->nff=0;
    s->srd=1000;s->sgc=2000;s->sv=250;s->sh=0.04F;s->cloth.active=0;s->cloth.nc=0;
}
static void sim_step(Sim *s){
    rbgrav(s);rbdet(s);rbslv(s);rbint(s);flden(s);flfrc(s);flstp(s);
    for(int i=0;i<s->ns;i++)sbstp(s,i);clstp(s);ptstp(s);s->step++;
}
static void stats(Sim *s){
    ps("=== Physics Stats ===");
    ps("Bodies:");pi(s->nb);ps("Contacts:");pi(s->nc);ps("Fluid:");pi(s->nf);
    ps("Soft:");pi(s->ns);ps("Cloth:");pi(s->cloth.nc);ps("Parts:");pi(s->np);
    ps("Emits:");pi(s->ne);ps("Fields:");pi(s->nff);ps("Step:");pi(s->step);
}

int main(void){
    ps("=== AI-ASM Physics Simulation Engine ===");
    unsigned int ptr=host_alloc(sizeof(Sim),16);
    Sim *sim=(Sim *)(unsigned long)ptr;
    if(!ptr){ps("ERROR: alloc failed");host_exit(1);}
    sim_init(sim);

    ps("--- Rigid Bodies ---");
    rbmk(sim,(V3){-1,5,0},10,0.3F,0.6F,0);rbmk(sim,(V3){1,5,0},5,0.25F,0.55F,1);
    rbmk(sim,(V3){0,8,0},3,0.1F,0.9F,2);rbmk(sim,(V3){0.5F,3,0.5F},8,0.15F,0.65F,3);
    rbmk(sim,(V3){-0.5F,6,-0.5F},4,0.2F,0.4F,4);
    ps("Bodies:");pi(sim->nb);

    ps("--- SPH Fluid ---");
    fladd(sim,(V3){-0.3F,0.5F,0},5,4,0.03F);
    ps("Fluid:");pi(sim->nf);

    ps("--- Soft Bodies ---");
    sbmk(sim,(V3){2,3,0},0.4F,200,5);sbmk(sim,(V3){-2,4,0},0.3F,150,3);
    ps("Soft:");pi(sim->ns);

    ps("--- Cloth ---");
    clmk(sim,(V3){-0.5F,3,-0.5F},1.0F,6,6);
    ps("Cloth cons:");pi(sim->cloth.nc);

    ps("--- Particles ---");
    emmk(sim,(V3){0,1,0},(V3){0,1,0},3,0.5F,20);
    emmk(sim,(V3){1,0.5F,0},(V3){-1,1,0},2,0.8F,15);
    ffmk(sim,(V3){0,3,0},2,5,0);ffmk(sim,(V3){-1,2,0},1.5F,3,2);
    ps("Emits:");pi(sim->ne);ps("Fields:");pi(sim->nff);

    ps("--- Materials ---");
    for(int i=0;i<NMAT;i++){ps(" ");ps(g_mat[i].name);ps(" d=");pf(g_mat[i].density);}

    ps("--- Running 100 steps ---");
    for(int i=0;i<100;i++){sim_step(sim);if(i%25==0)pi(i);}

    ps("--- Results ---");
    for(int i=0;i<sim->nb;i++){RB *b=&sim->bodies[i];
        ps("B");pi(i);ps(" y=");pf(b->pos.y);ps(" vy=");pf(b->vel.y);
        if(b->mat>=0&&b->mat<NMAT){ps(" mat=");ps(g_mat[b->mat].name);}}
    float ke=0;for(int i=0;i<sim->nf;i++)
        ke+=0.5F*sim->fluid[i].mass*vd(sim->fluid[i].vel,sim->fluid[i].vel);
    ps("FluidKE:");pf(ke);

    ps("--- 100 more ---");
    for(int i=0;i<100;i++)sim_step(sim);
    stats(sim);ps("=== Physics Simulation Complete ===");
    host_exit(0);return 0;
}
