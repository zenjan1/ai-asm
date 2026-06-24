/* celery_admin: Celery (Apium graveolens) stalk vegetable cultivation (v1.0)
 * Celery transplanting, hilling, blanching, harvest, market
 * Features: stalk height, stalk width, rib count, leaf pct, heart firmness, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stk_ht,stk_wd,rib_ct,leaf_pct,heart_fm,harv_wk,active;} cele_t;
typedef struct{int n_trans,n_hill,n_blanch,n_harv,n_mkt,t_stk,t_wd,t_rib,t_leaf,t_heart;} cele_state_t;
static cele_t celetr[N],celehs[N-2],celebs[N-4],celehvs[N-6],celems[N-6]; static cele_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cele_t*a,int*cnt,int*sum,int mx,int lc,int sh,int sw,int rc,int lp,int hf,int hw){if(*cnt>=mx)return -1;cele_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stk_ht=sh;x->stk_wd=sw;x->rib_ct=rc;x->leaf_pct=lp;x->heart_fm=hf;x->harv_wk=hw;x->active=1;*sum+=sh;(*cnt)++;ps("[CELE] Celery ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" sw=");pi(sw);ps(" rc=");pi(rc);ps(" lp=");pi(lp);ps(" hf=");pi(hf);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cele_init(void){if(init)return -1;st.n_trans=0;st.n_hill=0;st.n_blanch=0;st.n_harv=0;st.n_mkt=0;st.t_stk=0;st.t_wd=0;st.t_rib=0;st.t_leaf=0;st.t_heart=0;for(int i=0;i<N;i++)celetr[i].active=0;for(int i=0;i<N-2;i++)celehs[i].active=0;for(int i=0;i<N-4;i++)celebs[i].active=0;for(int i=0;i<N-6;i++)celehvs[i].active=0;for(int i=0;i<N-6;i++)celems[i].active=0;init=1;ps("[CELE] Celery initialized\n");return 0;}
int cele_transplanting(int lc,int sh,int sw,int rc,int lp,int hf,int hw){return add(celetr,&st.n_trans,&st.t_stk,N,lc,sh,sw,rc,lp,hf,hw);}
int cele_hilling(int lc,int sh,int sw,int rc,int lp,int hf,int hw){return add(celehs,&st.n_hill,&st.t_wd,N-2,lc,sh,sw,rc,lp,hf,hw);}
int cele_blanching(int lc,int sh,int sw,int rc,int lp,int hf,int hw){return add(celebs,&st.n_blanch,&st.t_rib,N-4,lc,sh,sw,rc,lp,hf,hw);}
int cele_harvest(int lc,int sh,int sw,int rc,int lp,int hf,int hw){return add(celehvs,&st.n_harv,&st.t_leaf,N-6,lc,sh,sw,rc,lp,hf,hw);}
int cele_market(int lc,int sh,int sw,int rc,int lp,int hf,int hw){return add(celems,&st.n_mkt,&st.t_heart,N-6,lc,sh,sw,rc,lp,hf,hw);}
void cele_report(void){ps("[CELE] Trans: ");pi(st.n_trans);ps(" Stk=");pi(st.t_stk);ps("\nHill/Wd=");pi(st.n_hill);ps("/");pi(st.t_wd);ps("\nBlanch/Rib=");pi(st.n_blanch);ps("/");pi(st.t_rib);ps("\nHarv/Leaf=");pi(st.n_harv);ps("/");pi(st.t_leaf);ps("\nMkt/Heart=");pi(st.n_mkt);ps("/");pi(st.t_heart);ps("\n");}
void cele_state(void){ps("[CELE] Trans=");pi(st.n_trans);ps(" Hill=");pi(st.n_hill);ps(" Blanch=");pi(st.n_blanch);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Celery Admin Demo ===\n\n");cele_init();
/* 1=field 2=raised_bed 3=greenhouse 4=organic 5=hydroponic */
ps("Celery transplanting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sh=25+(i*5),sw=2+(i%3),rc=5+(i*2),lp=15+(i*3),hf=4+(i%5),hw=14+(i%4);cele_transplanting(lc,sh,sw,rc,lp,hf,hw);}
ps("\nCelery hilling...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sh=28+(i*4),sw=3+(i%2),rc=6+(i*2),lp=18+(i*2),hf=5+(i%4),hw=16+(i%3);cele_hilling(lc,sh,sw,rc,lp,hf,hw);}
ps("\nCelery blanching...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sh=30+(i*3),sw=3+(i%2),rc=7+(i*2),lp=20+(i*2),hf=6+(i%3),hw=18+(i%2);cele_blanching(lc,sh,sw,rc,lp,hf,hw);}
ps("\nCelery harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sh=22+(i*6),sw=2+(i%3),rc=4+(i*3),lp=12+(i*4),hf=3+(i%5),hw=12+(i%5);cele_harvest(lc,sh,sw,rc,lp,hf,hw);}
ps("\nCelery market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sh=32+(i*3),sw=4+(i%2),rc=8+(i*2),lp=22+(i*2),hf=7+(i%2),hw=20+(i%2);cele_market(lc,sh,sw,rc,lp,hf,hw);}
ps("\n");cele_report();cele_state();ps("\n=== Demo Complete ===\n");return 0;}
