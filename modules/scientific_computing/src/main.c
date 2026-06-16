/* scientific_computing: FEA, MD, CFD, climate, numerical methods, viz (v1.0) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_NODES 128
#define MAX_ELEM  256
#define MAX_PART  64
#define CFD_N     12
#define CLIM_Z    8
#define MAX_SNZ   128
#define MAX_THR   4
#define PI 3.14159265

static double fabs_d(double x){return x<0?-x:x;}
static double sqrt_d(double x){double g=x*0.5;for(int i=0;i<24;i++)g=(g+x/g)*0.5;return x<=0?0:g;}
static double sin_d(double x){
    while(x>PI)x-=2*PI; while(x<-PI)x+=2*PI;
    double s=x,t=x;
    for(int i=1;i<12;i++){t*=(-x*x)/((2*i)*(2*i+1));s+=t;} return s;
}
static double cos_d(double x){return sin_d(x+PI*0.5);}

static unsigned int heap_pos=65536;
static unsigned int alloc(unsigned int n){unsigned int p=heap_pos;heap_pos+=n;heap_pos=(heap_pos+15)&~15u;return p;}
static void copy_to_mem(const char *s,unsigned int d,unsigned int n){char *p=(char*)d;for(unsigned int i=0;i<n;i++)p[i]=s[i];}
static unsigned int my_strlen(const char *s){unsigned int n=0;while(s[n])n++;return n;}
static int my_strcmp(const char *a,const char *b){while(*a&&*b){if(*a!=*b)return 1;a++;b++;}return(*a!=*b);}
static void print_str(const char *s){unsigned int l=my_strlen(s);unsigned int b=alloc(l+1);copy_to_mem(s,b,l+1);host_print(b,l);}
static void print_int(int n){
    char buf[16];int i=15,neg=0;buf[15]=0;
    if(n<0){neg=1;n=-n;} if(!n)buf[--i]='0';
    while(n>0){buf[--i]='0'+(n%10);n/=10;} if(neg)buf[--i]='-';
    unsigned int l=my_strlen(&buf[i]);unsigned int d=alloc(l+1);copy_to_mem(&buf[i],d,l+1);host_print(d,l);
}
static void print_float(double v){
    int neg=0;if(v<0){neg=1;v=-v;}
    int w=(int)v,f=(int)((v-(double)w)*10000);if(f<0)f=-f;
    if(neg)print_str("-");print_int(w);print_str(".");
    if(f<1000)print_str("0");if(f<100)print_str("0");if(f<10)print_str("0");print_int(f);
}

/* ====== structures ====== */
typedef struct{double x,y;}vec2_t;
typedef struct{
    int nn,ne;double young,poisson;
    vec2_t nodes[MAX_NODES];int elems[MAX_ELEM*3];
    double stiff[MAX_NODES*MAX_NODES],disp[MAX_NODES];
}fea_t;
typedef struct{
    int np;double mass,sig,eps,dt;
    vec2_t pos[MAX_PART],vel[MAX_PART],acc[MAX_PART];
}md_t;
typedef struct{
    double u[CFD_N][CFD_N],v[CFD_N][CFD_N],p[CFD_N][CFD_N],t[CFD_N][CFD_N];
    double re,dt,dx,dy,visc;
}cfd_t;
typedef struct{
    double temp[CLIM_Z],wx[CLIM_Z],wy[CLIM_Z],rad[CLIM_Z],alb[CLIM_Z];
}clim_t;
typedef struct{int r,c;double v;}sp_e_t;
typedef struct{int n,nnz;sp_e_t e[MAX_SNZ];}sp_t;
typedef struct{int n;double d[MAX_NODES];}vec_t;
typedef struct{double fov,asp,nr,fr;vec2_t rot;}cam_t;
typedef struct{int nt;int wl[MAX_THR];}par_t;

/* ====== FEA ====== */
static void fea_init(fea_t *m,int nx,int ny,double E,double nu){
    m->nn=nx*ny;m->ne=(nx-1)*(ny-1)*2;m->young=E;m->poisson=nu;
    for(int j=0;j<ny;j++)for(int i=0;i<nx;i++){
        int k=j*nx+i;m->nodes[k].x=(double)i/(nx-1);m->nodes[k].y=(double)j/(ny-1);
    }
    int idx=0;
    for(int j=0;j<ny-1;j++)for(int i=0;i<nx-1;i++){
        int n0=j*nx+i;
        m->elems[idx*3]=n0;m->elems[idx*3+1]=n0+1;m->elems[idx*3+2]=n0+nx;idx++;
        m->elems[idx*3]=n0+1;m->elems[idx*3+1]=n0+nx+1;m->elems[idx*3+2]=n0+nx;idx++;
    }
    for(int i=0;i<m->nn*m->nn;i++)m->stiff[i]=0;
    for(int i=0;i<m->nn;i++)m->disp[i]=0;
}
static void fea_assemble(fea_t *m){
    double C=m->young/(1-m->poisson*m->poisson);
    for(int e=0;e<m->ne;e++){
        double ax=m->nodes[m->elems[e*3]].x,ay=m->nodes[m->elems[e*3]].y;
        double bx=m->nodes[m->elems[e*3+1]].x,by=m->nodes[m->elems[e*3+1]].y;
        double cx=m->nodes[m->elems[e*3+2]].x,cy=m->nodes[m->elems[e*3+2]].y;
        double area=fabs_d((bx-ax)*(cy-ay)-(cx-ax)*(by-ay))*0.5;
        if(area<1e-12)continue;double ke=C*area*0.25;
        for(int i=0;i<3;i++)for(int j=0;j<3;j++){
            int ni=m->elems[e*3+i],nj=m->elems[e*3+j];
            m->stiff[ni*m->nn+nj]+=ke*((i==j)?2.0:-0.5);
        }
    }
}
static void fea_bc(fea_t *m,int nx,int ny){
    for(int j=0;j<ny;j++){int n=j*nx;m->stiff[n*m->nn+n]=1e10;m->disp[n]=0;}
}
static void fea_solve(fea_t *m,int iters){
    double r[MAX_NODES],p[MAX_NODES],Ap[MAX_NODES],rsold=0,alpha,beta,rsnew,pAp;
    for(int i=0;i<m->nn;i++){
        double s=0;for(int j=0;j<m->nn;j++)s+=m->stiff[i*m->nn+j]*m->disp[j];
        r[i]=1-s;p[i]=r[i];
    }
    for(int i=0;i<m->nn;i++)rsold+=r[i]*r[i];
    for(int it=0;it<iters;it++){
        pAp=0;for(int i=0;i<m->nn;i++){
            Ap[i]=0;for(int j=0;j<m->nn;j++)Ap[i]+=m->stiff[i*m->nn+j]*p[j];pAp+=p[i]*Ap[i];
        }
        alpha=rsold/(pAp+1e-30);rsnew=0;
        for(int i=0;i<m->nn;i++){m->disp[i]+=alpha*p[i];r[i]-=alpha*Ap[i];rsnew+=r[i]*r[i];}
        if(sqrt_d(rsnew)<1e-8)break;
        beta=rsnew/(rsold+1e-30);for(int i=0;i<m->nn;i++)p[i]=r[i]+beta*p[i];rsold=rsnew;
    }
    print_str("  FEA CG residual=");print_float(sqrt_d(rsold));print_str("\n");
}

/* ====== MD ====== */
static void md_init(md_t *s,int n,double m,double sig,double eps,double dt){
    int side=1,idx=0;s->np=n;s->mass=m;s->sig=sig;s->eps=eps;s->dt=dt;
    while(side*side<n)side++;
    for(int r=0;r<side&&idx<n;r++)for(int c=0;c<side&&idx<n;c++){
        s->pos[idx].x=c*sig*1.5+0.1;s->pos[idx].y=r*sig*1.5+0.1;
        s->vel[idx].x=sin_d(idx)*0.1;s->vel[idx].y=cos_d(idx)*0.1;
        s->acc[idx].x=0;s->acc[idx].y=0;idx++;
    }
}
static void md_forces(md_t *s){
    double s2=s->sig*s->sig;
    for(int i=0;i<s->np;i++){s->acc[i].x=0;s->acc[i].y=0;}
    for(int i=0;i<s->np;i++)for(int j=i+1;j<s->np;j++){
        double dx=s->pos[j].x-s->pos[i].x,dy=s->pos[j].y-s->pos[i].y;
        double r2=dx*dx+dy*dy;if(r2<1e-6)r2=1e-6;
        double r6=s2/r2;r6=r6*r6*r6;
        double f=24*s->eps/r2*r6*(2*r6-1);
        s->acc[i].x-=f*dx/s->mass;s->acc[i].y-=f*dy/s->mass;
        s->acc[j].x+=f*dx/s->mass;s->acc[j].y+=f*dy/s->mass;
    }
}
static void md_verlet(md_t *s){
    double h=0.5*s->dt;
    for(int i=0;i<s->np;i++){
        s->vel[i].x+=s->acc[i].x*h;s->vel[i].y+=s->acc[i].y*h;
        s->pos[i].x+=s->vel[i].x*s->dt;s->pos[i].y+=s->vel[i].y*s->dt;
    }
    md_forces(s);
    for(int i=0;i<s->np;i++){s->vel[i].x+=s->acc[i].x*h;s->vel[i].y+=s->acc[i].y*h;}
}
static double md_energy(md_t *s){
    double ke=0,pe=0,s2=s->sig*s->sig;
    for(int i=0;i<s->np;i++)ke+=0.5*s->mass*(s->vel[i].x*s->vel[i].x+s->vel[i].y*s->vel[i].y);
    for(int i=0;i<s->np;i++)for(int j=i+1;j<s->np;j++){
        double dx=s->pos[j].x-s->pos[i].x,dy=s->pos[j].y-s->pos[i].y;
        double r2=dx*dx+dy*dy;if(r2<1e-6)r2=1e-6;
        double r6=s2/r2;r6=r6*r6*r6;pe+=4*s->eps*r6*(r6-1);
    }
    return ke+pe;
}
static void md_minimize(md_t *s,int steps){
    double g=0.1;
    for(int st=0;st<steps;st++){
        md_forces(s);
        for(int i=0;i<s->np;i++){s->pos[i].x+=g*s->acc[i].x*s->mass;s->pos[i].y+=g*s->acc[i].y*s->mass;}
        g*=0.98;
    }
}

/* ====== CFD ====== */
static void cfd_init(cfd_t *g,double re,double dt){
    g->re=re;g->dt=dt;g->dx=1.0/(CFD_N-1);g->dy=1.0/(CFD_N-1);g->visc=1.0/re;
    for(int i=0;i<CFD_N;i++)for(int j=0;j<CFD_N;j++){g->u[i][j]=0;g->v[i][j]=0;g->p[i][j]=0;g->t[i][j]=0;}
    for(int j=0;j<CFD_N;j++){double y=j*g->dy;g->u[0][j]=4*y*(1-y);g->t[0][j]=1;}
}
static void cfd_step(cfd_t *g){
    double ix2=1/(g->dx*g->dx),iy2=1/(g->dy*g->dy);
    for(int i=1;i<CFD_N-1;i++)for(int j=1;j<CFD_N-1;j++){
        double lap=(g->u[i+1][j]-2*g->u[i][j]+g->u[i-1][j])*ix2+(g->u[i][j+1]-2*g->u[i][j]+g->u[i][j-1])*iy2;
        double dp=(g->p[i+1][j]-g->p[i-1][j])/(2*g->dx);
        double ax=g->u[i][j]*(g->u[i+1][j]-g->u[i-1][j])/(2*g->dx);
        double ay=g->v[i][j]*(g->u[i][j+1]-g->u[i][j-1])/(2*g->dy);
        g->t[i][j]=g->u[i][j]+g->dt*(g->visc*lap-dp-ax-ay);
    }
    for(int i=1;i<CFD_N-1;i++)for(int j=1;j<CFD_N-1;j++)g->u[i][j]=g->t[i][j];
    double cs=0.1;
    for(int i=1;i<CFD_N-1;i++)for(int j=1;j<CFD_N-1;j++){
        double dudx=(g->u[i+1][j]-g->u[i-1][j])/(2*g->dx);
        double dvdy=(g->v[i][j+1]-g->v[i][j-1])/(2*g->dy);
        g->p[i][j]+=cs*cs*g->dx*g->dy*sqrt_d(dudx*dudx+dvdy*dvdy)*0.01;
    }
}
static double cfd_res(cfd_t *g){
    double r=0;
    for(int i=1;i<CFD_N-1;i++)for(int j=1;j<CFD_N-1;j++){
        double d=(g->u[i+1][j]-g->u[i-1][j])/(2*g->dx)+(g->v[i][j+1]-g->v[i][j-1])/(2*g->dy);r+=d*d;
    }
    return sqrt_d(r);
}

/* ====== Climate ====== */
static void clim_init(clim_t *c){
    for(int i=0;i<CLIM_Z;i++){
        double lat=(double)i/(CLIM_Z-1);
        c->temp[i]=300-40*fabs_d(lat-0.5)*2;c->wx[i]=5*sin_d(lat*PI);
        c->wy[i]=2*cos_d(lat*PI);c->rad[i]=1361*(0.7+0.3*cos_d(lat*PI));c->alb[i]=0.3;
    }
}
static void clim_step(clim_t *c,double dt){
    double sig=5.67e-8;
    for(int i=0;i<CLIM_Z;i++){
        double abs=c->rad[i]*(1-c->alb[i])*0.25;
        double t=c->temp[i],emi=sig*t*t*t*t;
        c->temp[i]+=(abs-emi)*dt/4e8;
    }
    for(int i=1;i<CLIM_Z-1;i++){
        double gr=(c->temp[i+1]-c->temp[i-1])*0.5;
        c->wx[i]+=gr*0.001*dt;c->wy[i]+=gr*0.0005*dt;
    }
}
static double clim_avg(clim_t *c){double s=0;for(int i=0;i<CLIM_Z;i++)s+=c->temp[i];return s/CLIM_Z;}

/* ====== Numerical Methods ====== */
static double ode_euler(double y0,double t0,double dt,int steps){
    double t=t0,y=y0;for(int i=0;i<steps;i++){y+=(-y+sin_d(t))*dt;t+=dt;}return y;
}
static double ode_rk4(double y0,double t0,double dt,int steps){
    double t=t0,y=y0,k1,k2,k3,k4;
    for(int i=0;i<steps;i++){
        k1=-y+sin_d(t);k2=-(y+0.5*dt*k1)+sin_d(t+0.5*dt);
        k3=-(y+0.5*dt*k2)+sin_d(t+0.5*dt);k4=-(y+dt*k3)+sin_d(t+dt);
        y+=dt/6*(k1+2*k2+2*k3+k4);t+=dt;
    }
    return y;
}
static void sp_set(sp_t *m,int r,int c,double v){
    for(int i=0;i<m->nnz;i++)if(m->e[i].r==r&&m->e[i].c==c){m->e[i].v=v;return;}
    if(m->nnz<MAX_SNZ){m->e[m->nnz].r=r;m->e[m->nnz].c=c;m->e[m->nnz].v=v;m->nnz++;}
}
static void sp_mv(sp_t *m,vec_t *x,vec_t *y){
    for(int i=0;i<y->n;i++)y->d[i]=0;
    for(int k=0;k<m->nnz;k++)y->d[m->e[k].r]+=m->e[k].v*x->d[m->e[k].c];
}
static void sp_cg(sp_t *A,vec_t *b,vec_t *x,int iters){
    vec_t r,p,Ap;r.n=p.n=Ap.n=A->n;
    double rsold=0,alpha,beta,rsnew,pAp;
    sp_mv(A,x,&r);
    for(int i=0;i<A->n;i++){r.d[i]=b->d[i]-r.d[i];p.d[i]=r.d[i];rsold+=r.d[i]*r.d[i];}
    for(int it=0;it<iters;it++){
        sp_mv(A,&p,&Ap);pAp=0;
        for(int i=0;i<A->n;i++)pAp+=p.d[i]*Ap.d[i];
        alpha=rsold/(pAp+1e-30);rsnew=0;
        for(int i=0;i<A->n;i++){x->d[i]+=alpha*p.d[i];r.d[i]-=alpha*Ap.d[i];rsnew+=r.d[i]*r.d[i];}
        if(sqrt_d(rsnew)<1e-10)break;
        beta=rsnew/(rsold+1e-30);for(int i=0;i<A->n;i++)p.d[i]=r.d[i]+beta*p.d[i];rsold=rsnew;
    }
}

/* ====== Parallel ====== */
static void par_init(par_t *c,int nt){c->nt=nt;for(int i=0;i<nt;i++)c->wl[i]=0;}
static int par_dispatch(par_t *c,int w){
    int mi=0;for(int i=1;i<c->nt;i++)if(c->wl[i]<c->wl[mi])mi=i;
    c->wl[mi]+=w;return mi;
}
static int par_balance(par_t *c){
    int tot=0,mx=0;
    for(int i=0;i<c->nt;i++){tot+=c->wl[i];if(c->wl[i]>mx)mx=c->wl[i];}
    return mx>0?tot/mx:c->nt;
}

/* ====== Visualization ====== */
static void viz_bar(const char *l,double v,double mx){
    int n=(int)(v/mx*30);if(n<0)n=0;if(n>30)n=30;
    print_str("  ");print_str(l);print_str(" |");
    for(int i=0;i<n;i++)print_str("#");for(int i=n;i<30;i++)print_str(" ");
    print_str("| ");print_float(v);print_str("\n");
}
static void viz_3d(cam_t *cam,double *x,double *y,double *z){
    double cr=cos_d(cam->rot.x),sr=sin_d(cam->rot.x);
    double cp=cos_d(cam->rot.y),sp=sin_d(cam->rot.y);
    double y1=*y*cr-*z*sr,z1=*y*sr+*z*cr;
    double x2=*x*cp+z1*sp,z2=-*x*sp+z1*cp;
    double s=cam->fr/(cam->fr-z2+1e-6);
    *x=x2*s*cam->fov;*y=y1*s*cam->fov/cam->asp;*z=z2;
}
static void viz_scatter(vec2_t *pts,int n,const char *title){
    char grid[20*41];for(int i=0;i<20*41;i++)grid[i]=' ';
    for(int i=0;i<20;i++)grid[i*41+40]='\n';
    for(int i=0;i<n;i++){
        int px=(int)(pts[i].x*39),py=(int)(pts[i].y*19);
        if(px>=0&&px<40&&py>=0&&py<20)grid[py*41+px]='*';
    }
    grid[20*41-1]=0;print_str(title);print_str("\n");print_str((const char*)grid);
}
static void viz_anim(int f,int total){
    print_str("  [anim ");print_int(f);print_str("/");print_int(total);print_str("]\n");
}

/* ====== derived quantities ====== */
static double fea_max_disp(fea_t *m){
    double mx=0;for(int i=0;i<m->nn;i++)if(fabs_d(m->disp[i])>mx)mx=fabs_d(m->disp[i]);return mx;
}
static double md_max_speed(md_t *s){
    double mx=0;
    for(int i=0;i<s->np;i++){
        double v2=s->vel[i].x*s->vel[i].x+s->vel[i].y*s->vel[i].y;
        if(v2>mx)mx=v2;
    }
    return sqrt_d(mx);
}
static double cfd_max_u(cfd_t *g){
    double mx=0;
    for(int i=0;i<CFD_N;i++)for(int j=0;j<CFD_N;j++)if(g->u[i][j]>mx)mx=g->u[i][j];
    return mx;
}
static double clim_min_temp(clim_t *c){
    double mn=c->temp[0];for(int i=1;i<CLIM_Z;i++)if(c->temp[i]<mn)mn=c->temp[i];return mn;
}
static double clim_max_temp(clim_t *c){
    double mx=c->temp[0];for(int i=1;i<CLIM_Z;i++)if(c->temp[i]>mx)mx=c->temp[i];return mx;
}
static void viz_contour(double *data,int nx,int ny,double lo,double hi){
    int levels=5;
    for(int lv=0;lv<levels;lv++){
        double thr=lo+(hi-lo)*lv/(levels-1);
        print_str("  lv");print_int(lv);print_str("(>=");print_float(thr);print_str("): ");
        int cnt=0;
        for(int i=0;i<nx*ny;i++)if(data[i]>=thr)cnt++;
        print_int(cnt);print_str(" cells\n");
    }
}

/* ====== Tests ====== */
static void test_fem(void){
    fea_t m;print_str("  [FEA] 4x4 mesh, E=200GPa\n");
    fea_init(&m,4,4,200,0.3);
    print_str("  nodes=");print_int(m.nn);print_str(" elems=");print_int(m.ne);print_str("\n");
    fea_assemble(&m);fea_bc(&m,4,4);fea_solve(&m,50);
    print_str("  max_disp=");print_float(fea_max_disp(&m));print_str("\n");
}
static void test_md(void){
    md_t s;print_str("  [MD] 16 LJ particles\n");
    md_init(&s,16,1,1,1,0.005);
    double e0=md_energy(&s);print_str("  E0=");print_float(e0);print_str("\n");
    for(int i=0;i<50;i++)md_verlet(&s);
    print_str("  E50=");print_float(md_energy(&s));print_str(" vmax=");
    print_float(md_max_speed(&s));print_str("\n");
    md_minimize(&s,20);
    print_str("  Emin=");print_float(md_energy(&s));print_str("\n");
}
static void test_cfd(void){
    cfd_t g;print_str("  [CFD] ");print_int(CFD_N);print_str("x");print_int(CFD_N);print_str(" Re=100\n");
    cfd_init(&g,100,0.01);
    for(int i=0;i<100;i++)cfd_step(&g);
    print_str("  residual=");print_float(cfd_res(&g));
    print_str(" umax=");print_float(cfd_max_u(&g));print_str("\n");
    double *uc=(double*)&g.u;
    viz_contour(uc,CFD_N,CFD_N,0.0,1.0);
}
static void test_clim(void){
    clim_t c;print_str("  [CLIMATE] ");print_int(CLIM_Z);print_str(" zones\n");
    clim_init(&c);
    print_str("  T0 avg=");print_float(clim_avg(&c));
    print_str(" min=");print_float(clim_min_temp(&c));
    print_str(" max=");print_float(clim_max_temp(&c));print_str("K\n");
    for(int i=0;i<1000;i++)clim_step(&c,3600);
    print_str("  T1000 avg=");print_float(clim_avg(&c));
    print_str(" min=");print_float(clim_min_temp(&c));
    print_str(" max=");print_float(clim_max_temp(&c));print_str("K\n");
}
static void test_num(void){
    double ye=ode_euler(0,0,0.01,100),yr=ode_rk4(0,0,0.01,100);
    print_str("  [ODE] Euler=");print_float(ye);print_str(" RK4=");print_float(yr);print_str("\n");
    sp_t A;vec_t b,x;A.n=4;A.nnz=0;b.n=4;x.n=4;
    sp_set(&A,0,0,4);sp_set(&A,0,1,-1);sp_set(&A,1,0,-1);sp_set(&A,1,1,4);
    sp_set(&A,1,2,-1);sp_set(&A,2,1,-1);sp_set(&A,2,2,4);sp_set(&A,2,3,-1);
    sp_set(&A,3,2,-1);sp_set(&A,3,3,4);
    b.d[0]=1;b.d[1]=2;b.d[2]=3;b.d[3]=4;x.d[0]=x.d[1]=x.d[2]=x.d[3]=0;
    sp_cg(&A,&b,&x,100);
    print_str("  [SPARSE] x=");for(int i=0;i<4;i++){print_float(x.d[i]);print_str(" ");}print_str("\n");
    par_t pc;par_init(&pc,MAX_THR);
    par_dispatch(&pc,10);par_dispatch(&pc,20);par_dispatch(&pc,5);par_dispatch(&pc,15);
    print_str("  [PARALLEL] efficiency=");print_int(par_balance(&pc));print_str("/");print_int(MAX_THR);print_str("\n");
}
static void test_viz(void){
    cam_t cam;cam.fov=1;cam.asp=1.778;cam.nr=0.1;cam.fr=100;cam.rot.x=0.3;cam.rot.y=0.5;
    print_str("  [VIZ] bars:\n");viz_bar("T1",0.75,1);viz_bar("T2",0.42,1);viz_bar("T3",0.91,1);
    double px=1,py=0.5,pz=2;viz_3d(&cam,&px,&py,&pz);
    print_str("  3D(1,0.5,2)->(");print_float(px);print_str(",");print_float(py);print_str(",");print_float(pz);print_str(")\n");
    vec2_t pts[10];for(int i=0;i<10;i++){pts[i].x=i*0.1;pts[i].y=0.5+0.4*sin_d(i*0.628);}
    viz_scatter(pts,10,"  [VIZ] scatter:");
    for(int f=0;f<3;f++)viz_anim(f+1,3);
}

static void run_tests(void){
    print_str("scientific_computing: test suite\n\n");
    test_fem();print_str("\n");test_md();print_str("\n");test_cfd();print_str("\n");
    test_clim();print_str("\n");test_num();print_str("\n");test_viz();
    print_str("\nscientific_computing: tests complete\n");
}

static void show_help(void){
    print_str("scientific_computing - FEA/MD/CFD/Climate sim (v1.0)\n");
    print_str("  -h  show help\n");
    print_str("  -t  run tests\n");
    print_str("Modules: FEA(mesh,stiffness,CG) MD(LJ,Verlet,minimize)\n");
    print_str("  CFD(Navier-Stokes,Smagorinsky) Climate(atm,ocean,rad)\n");
    print_str("  Numerical(ODE/sparse/parallel) Viz(3D,scatter,anim)\n");
}

void _start(void){
    unsigned int buf=host_alloc(512,16);
    host_get_argv(buf,512);
    int hf=0,tf=0;unsigned int pos=0;char *ap=(char*)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){
        char *a=&ap[pos];
        if(!my_strcmp(a,"-h")||!my_strcmp(a,"--help"))hf=1;
        else if(!my_strcmp(a,"-t")||!my_strcmp(a,"--test"))tf=1;
        while(pos<512&&ap[pos])pos++;pos++;
    }
    if(hf){show_help();host_exit(0);}
    if(tf){run_tests();host_exit(0);}
    print_str("scientific_computing: -h help, -t tests\n");
    host_exit(0);
}
