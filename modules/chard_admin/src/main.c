/* chard_admin: Chard (Beta vulgaris) leafy green vegetable cultivation (v1.0)
 * Chard planting, cutting, blanching, harvest, market
 * Features: leaf_span_cm, stem_height_cm, leaf_color, rib_width_mm, bolt_week, cutting_count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_sp,stm_ht,leaf_clr,rib_wd,bolt_wk,cut_ct,active;} chrd_t;
typedef struct{int n_plant,n_cut,n_blanch,n_harv,n_mkt,t_leaf,t_stm,t_clr,t_rib,t_bolt;} chrd_state_t;
static chrd_t chrdps[N],chrdcs[N-2],chrdds[N-4],chrdhs[N-6],chrdms[N-6]; static chrd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chrd_t*a,int*cnt,int*sum,int mx,int lc,int ls,int sh,int lcr,int rw,int bw,int cc){if(*cnt>=mx)return -1;chrd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_sp=ls;x->stm_ht=sh;x->leaf_clr=lcr;x->rib_wd=rw;x->bolt_wk=bw;x->cut_ct=cc;x->active=1;*sum+=ls;(*cnt)++;ps("[CHRD] Chard ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ls=");pi(ls);ps(" sh=");pi(sh);ps(" lcr=");pi(lcr);ps(" rw=");pi(rw);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps("\n");return *cnt-1;}
int chrd_init(void){if(init)return -1;st.n_plant=0;st.n_cut=0;st.n_blanch=0;st.n_harv=0;st.n_mkt=0;st.t_leaf=0;st.t_stm=0;st.t_clr=0;st.t_rib=0;st.t_bolt=0;for(int i=0;i<N;i++)chrdps[i].active=0;for(int i=0;i<N-2;i++)chrdcs[i].active=0;for(int i=0;i<N-4;i++)chrdds[i].active=0;for(int i=0;i<N-6;i++)chrdhs[i].active=0;for(int i=0;i<N-6;i++)chrdms[i].active=0;init=1;ps("[CHRD] Chard initialized\n");return 0;}
int chrd_planting(int lc,int ls,int sh,int lcr,int rw,int bw,int cc){return add(chrdps,&st.n_plant,&st.t_leaf,N,lc,ls,sh,lcr,rw,bw,cc);}
int chrd_cutting(int lc,int ls,int sh,int lcr,int rw,int bw,int cc){return add(chrdcs,&st.n_cut,&st.t_stm,N-2,lc,ls,sh,lcr,rw,bw,cc);}
int chrd_blanching(int lc,int ls,int sh,int lcr,int rw,int bw,int cc){return add(chrdds,&st.n_blanch,&st.t_clr,N-4,lc,ls,sh,lcr,rw,bw,cc);}
int chrd_harvest(int lc,int ls,int sh,int lcr,int rw,int bw,int cc){return add(chrdhs,&st.n_harv,&st.t_rib,N-6,lc,ls,sh,lcr,rw,bw,cc);}
int chrd_market(int lc,int ls,int sh,int lcr,int rw,int bw,int cc){return add(chrdms,&st.n_mkt,&st.t_bolt,N-6,lc,ls,sh,lcr,rw,bw,cc);}
void chrd_report(void){ps("[CHRD] Plant: ");pi(st.n_plant);ps(" Leaf=");pi(st.t_leaf);ps("\nCut: ");pi(st.n_cut);ps(" Stem=");pi(st.t_stm);ps("\nBlanch: ");pi(st.n_blanch);ps(" Clr=");pi(st.t_clr);ps("\nHarv: ");pi(st.n_harv);ps(" Rib=");pi(st.t_rib);ps("\nMkt: ");pi(st.n_mkt);ps(" Bolt=");pi(st.t_bolt);ps("\n");}
void chrd_state(void){ps("[CHRD] Plant=");pi(st.n_plant);ps(" Cut=");pi(st.n_cut);ps(" Blanch=");pi(st.n_blanch);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chard Admin Demo ===\n\n");chrd_init();
/* 1=garden 2=raised_bed 3=greenhouse 4=organic 5=market */
ps("Chard planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ls=20+(i*5),sh=30+(i*8),lcr=(i%6)+1,rw=3+(i%4),bw=12+(i%4),cc=2+(i%5);chrd_planting(lc,ls,sh,lcr,rw,bw,cc);}
ps("\nChard cutting...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ls=25+(i*4),sh=35+(i*6),lcr=(i%5)+1,rw=4+(i%3),bw=14+(i%3),cc=3+(i%4);chrd_cutting(lc,ls,sh,lcr,rw,bw,cc);}
ps("\nChard blanching...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ls=30+(i*3),sh=40+(i*5),lcr=(i%4)+1,rw=5+(i%3),bw=16+(i%2),cc=4+(i%3);chrd_blanching(lc,ls,sh,lcr,rw,bw,cc);}
ps("\nChard harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ls=18+(i*6),sh=28+(i*9),lcr=(i%6)+1,rw=2+(i%5),bw=10+(i%5),cc=1+(i%6);chrd_harvest(lc,ls,sh,lcr,rw,bw,cc);}
ps("\nChard market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ls=35+(i*3),sh=45+(i*4),lcr=(i%3)+4,rw=6+(i%2),bw=18+(i%2),cc=5+(i%3);chrd_market(lc,ls,sh,lcr,rw,bw,cc);}
ps("\n");chrd_report();chrd_state();ps("\n=== Demo Complete ===\n");return 0;}
