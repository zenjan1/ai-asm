/* caladium_admin: Caladium ornamental foliage tuber management (v1.0)
 * Caladium propagation, foliage, dormancy, display, market
 * Features: leaf_span_cm, plant_height_cm, tuber_diameter_cm, color_variety, eye_count, dormancy_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_sp,plnt_ht,tuber_dia,clr_var,eye_ct,dorm_wk,active;} cald_t;
typedef struct{int n_prop,n_foliage,n_dorm,n_disp,n_mkt,t_leaf,t_ht,t_tuber,t_color,t_eye;} cald_state_t;
static cald_t caldps[N],caldfs[N-2],caldds[N-4],calddps[N-6],caldms[N-6]; static cald_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cald_t*a,int*cnt,int*sum,int mx,int lc,int ls,int ph,int td,int cv,int ec,int dw){if(*cnt>=mx)return -1;cald_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_sp=ls;x->plnt_ht=ph;x->tuber_dia=td;x->clr_var=cv;x->eye_ct=ec;x->dorm_wk=dw;x->active=1;*sum+=ls;(*cnt)++;ps("[CAL2] Caladium ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ls=");pi(ls);ps(" ph=");pi(ph);ps(" td=");pi(td);ps(" cv=");pi(cv);ps(" ec=");pi(ec);ps(" dw=");pi(dw);ps("\n");return *cnt-1;}
int cald_init(void){if(init)return -1;st.n_prop=0;st.n_foliage=0;st.n_dorm=0;st.n_disp=0;st.n_mkt=0;st.t_leaf=0;st.t_ht=0;st.t_tuber=0;st.t_color=0;st.t_eye=0;for(int i=0;i<N;i++)caldps[i].active=0;for(int i=0;i<N-2;i++)caldfs[i].active=0;for(int i=0;i<N-4;i++)caldds[i].active=0;for(int i=0;i<N-6;i++)calddps[i].active=0;for(int i=0;i<N-6;i++)caldms[i].active=0;init=1;ps("[CAL2] Caladium initialized\n");return 0;}
int cald_propagation(int lc,int ls,int ph,int td,int cv,int ec,int dw){return add(caldps,&st.n_prop,&st.t_leaf,N,lc,ls,ph,td,cv,ec,dw);}
int cald_foliage(int lc,int ls,int ph,int td,int cv,int ec,int dw){return add(caldfs,&st.n_foliage,&st.t_ht,N-2,lc,ls,ph,td,cv,ec,dw);}
int cald_dormancy(int lc,int ls,int ph,int td,int cv,int ec,int dw){return add(caldds,&st.n_dorm,&st.t_tuber,N-4,lc,ls,ph,td,cv,ec,dw);}
int cald_display(int lc,int ls,int ph,int td,int cv,int ec,int dw){return add(calddps,&st.n_disp,&st.t_color,N-6,lc,ls,ph,td,cv,ec,dw);}
int cald_market(int lc,int ls,int ph,int td,int cv,int ec,int dw){return add(caldms,&st.n_mkt,&st.t_eye,N-6,lc,ls,ph,td,cv,ec,dw);}
void cald_report(void){ps("[CAL2] Prop: ");pi(st.n_prop);ps(" Leaf=");pi(st.t_leaf);ps("\nFoliage: ");pi(st.n_foliage);ps(" Ht=");pi(st.t_ht);ps("\nDorm: ");pi(st.n_dorm);ps(" Tuber=");pi(st.t_tuber);ps("\nDisplay: ");pi(st.n_disp);ps(" Color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" Eye=");pi(st.t_eye);ps("\n");}
void cald_state(void){ps("[CAL2] Prop=");pi(st.n_prop);ps(" Foliage=");pi(st.n_foliage);ps(" Dorm=");pi(st.n_dorm);ps(" Display=");pi(st.n_disp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Caladium Admin Demo ===\n\n");cald_init();
/* 1=shade_garden 2=container 3=landscape 4=indoor 5=nursery */
ps("Caladium propagation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ls=20+(i*8),ph=25+(i*6),td=3+(i%5),cv=(i%6)+1,ec=3+(i*2),dw=8+(i%4);cald_propagation(lc,ls,ph,td,cv,ec,dw);}
ps("\nCaladium foliage...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ls=25+(i*6),ph=30+(i*5),td=4+(i%4),cv=(i%5)+1,ec=4+(i*2),dw=10+(i%3);cald_foliage(lc,ls,ph,td,cv,ec,dw);}
ps("\nCaladium dormancy...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ls=30+(i*5),ph=35+(i*4),td=5+(i%3),cv=(i%4)+1,ec=5+(i*2),dw=12+(i%2);cald_dormancy(lc,ls,ph,td,cv,ec,dw);}
ps("\nCaladium display...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ls=18+(i*9),ph=20+(i*7),td=2+(i%5),cv=(i%6)+1,ec=2+(i*3),dw=6+(i%5);cald_display(lc,ls,ph,td,cv,ec,dw);}
ps("\nCaladium market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ls=35+(i*4),ph=40+(i*3),td=6+(i*2),cv=(i%3)+4,ec=6+(i*2),dw=14+(i%2);cald_market(lc,ls,ph,td,cv,ec,dw);}
ps("\n");cald_report();cald_state();ps("\n=== Demo Complete ===\n");return 0;}
