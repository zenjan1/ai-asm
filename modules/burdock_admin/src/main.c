/* burdock_admin: Burdock (Arctium lappa) root vegetable cultivation management (v1.0)
 * Burdock planting, cultivation, harvest, storage, market
 * Features: root length, root diameter, leaf area, seed yield, growth days, soil pH
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,root_ln,root_dia,leaf_ar,seed_yd,grow_dy,soil_ph,active;} burd_t;
typedef struct{int n_plant,n_cult,n_harv,n_store,n_mkt,t_root,t_dia,t_leaf,t_seed,t_growth;} burd_state_t;
static burd_t burdps[N],burdcs[N-2],burdhs[N-4],burdss[N-6],burdms[N-6]; static burd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(burd_t*a,int*cnt,int*sum,int mx,int lc,int rl,int rd,int la,int sy,int gd,int sp){if(*cnt>=mx)return -1;burd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->root_ln=rl;x->root_dia=rd;x->leaf_ar=la;x->seed_yd=sy;x->grow_dy=gd;x->soil_ph=sp;x->active=1;*sum+=rl;(*cnt)++;ps("[BURD] Burdock ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rl=");pi(rl);ps(" rd=");pi(rd);ps(" la=");pi(la);ps(" sy=");pi(sy);ps(" gd=");pi(gd);ps(" sp=");pi(sp);ps("\n");return *cnt-1;}
int burd_init(void){if(init)return -1;st.n_plant=0;st.n_cult=0;st.n_harv=0;st.n_store=0;st.n_mkt=0;st.t_root=0;st.t_dia=0;st.t_leaf=0;st.t_seed=0;st.t_growth=0;for(int i=0;i<N;i++)burdps[i].active=0;for(int i=0;i<N-2;i++)burdcs[i].active=0;for(int i=0;i<N-4;i++)burdhs[i].active=0;for(int i=0;i<N-6;i++)burdss[i].active=0;for(int i=0;i<N-6;i++)burdms[i].active=0;init=1;ps("[BURD] Burdock initialized\n");return 0;}
int burd_planting(int lc,int rl,int rd,int la,int sy,int gd,int sp){return add(burdps,&st.n_plant,&st.t_root,N,lc,rl,rd,la,sy,gd,sp);}
int burd_cultivation(int lc,int rl,int rd,int la,int sy,int gd,int sp){return add(burdcs,&st.n_cult,&st.t_dia,N-2,lc,rl,rd,la,sy,gd,sp);}
int burd_harvest(int lc,int rl,int rd,int la,int sy,int gd,int sp){return add(burdhs,&st.n_harv,&st.t_leaf,N-4,lc,rl,rd,la,sy,gd,sp);}
int burd_storage(int lc,int rl,int rd,int la,int sy,int gd,int sp){return add(burdss,&st.n_store,&st.t_seed,N-6,lc,rl,rd,la,sy,gd,sp);}
int burd_market(int lc,int rl,int rd,int la,int sy,int gd,int sp){return add(burdms,&st.n_mkt,&st.t_growth,N-6,lc,rl,rd,la,sy,gd,sp);}
void burd_report(void){ps("[BURD] Plant: ");pi(st.n_plant);ps(" Root=");pi(st.t_root);ps("\nCult: ");pi(st.n_cult);ps(" Dia=");pi(st.t_dia);ps("\nHarv: ");pi(st.n_harv);ps(" Leaf=");pi(st.t_leaf);ps("\nStore: ");pi(st.n_store);ps(" Seed=");pi(st.t_seed);ps("\nMkt: ");pi(st.n_mkt);ps(" Growth=");pi(st.t_growth);ps("\n");}
void burd_state(void){ps("[BURD] Plant=");pi(st.n_plant);ps(" Cult=");pi(st.n_cult);ps(" Harv=");pi(st.n_harv);ps(" Store=");pi(st.n_store);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Burdock Admin Demo ===\n\n");burd_init();
/* 1=field 2=garden 3=farm 4=greenhouse 5=organic */
ps("Burdock planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,rl=20+(i*8),rd=3+(i%4),la=50+(i*20),sy=2+(i%5),gd=90+(i*10),sp=6+(i%2);burd_planting(lc,rl,rd,la,sy,gd,sp);}
ps("\nBurdock cultivation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,rl=25+(i*6),rd=4+(i%3),la=60+(i*15),sy=3+(i%4),gd=100+(i*8),sp=6+(i%2);burd_cultivation(lc,rl,rd,la,sy,gd,sp);}
ps("\nBurdock harvest...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,rl=30+(i*5),rd=5+(i%2),la=70+(i*12),sy=4+(i%3),gd=110+(i*6),sp=7+(i%1);burd_harvest(lc,rl,rd,la,sy,gd,sp);}
ps("\nBurdock storage...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,rl=18+(i*9),rd=3+(i%4),la=45+(i*25),sy=1+(i%5),gd=80+(i*12),sp=5+(i%3);burd_storage(lc,rl,rd,la,sy,gd,sp);}
ps("\nBurdock market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,rl=35+(i*4),rd=6+(i%2),la=80+(i*10),sy=5+(i%3),gd=120+(i*5),sp=7+(i%1);burd_market(lc,rl,rd,la,sy,gd,sp);}
ps("\n");burd_report();burd_state();ps("\n=== Demo Complete ===\n");return 0;}
