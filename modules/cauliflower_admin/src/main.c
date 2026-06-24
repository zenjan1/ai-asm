/* cauliflower_admin: Cauliflower (Brassica oleracea) vegetable cultivation (v1.0)
 * Cauliflower planting, blanching, pest control, harvest, market
 * Features: head diameter, head weight, leaf span, curd whiteness, days to mature, seed count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct,active;} caul_t;
typedef struct{int n_plant,n_blanch,n_pest,n_harv,n_mkt,t_head,t_wt,t_leaf,t_curd,t_days;} caul_state_t;
static caul_t caulps[N],caulbs[N-2],caulpe[N-4],caulhs[N-6],caulms[N-6]; static caul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(caul_t*a,int*cnt,int*sum,int mx,int lc,int hd,int hw,int ls,int cw,int dm,int sc){if(*cnt>=mx)return -1;caul_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->head_dia=hd;x->head_wt=hw;x->leaf_sp=ls;x->curd_wt=cw;x->days_mat=dm;x->seed_ct=sc;x->active=1;*sum+=hd;(*cnt)++;ps("[CAUL] Cauliflower ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" hd=");pi(hd);ps(" hw=");pi(hw);ps(" ls=");pi(ls);ps(" cw=");pi(cw);ps(" dm=");pi(dm);ps(" sc=");pi(sc);ps("\n");return *cnt-1;}
int caul_init(void){if(init)return -1;st.n_plant=0;st.n_blanch=0;st.n_pest=0;st.n_harv=0;st.n_mkt=0;st.t_head=0;st.t_wt=0;st.t_leaf=0;st.t_curd=0;st.t_days=0;for(int i=0;i<N;i++)caulps[i].active=0;for(int i=0;i<N-2;i++)caulbs[i].active=0;for(int i=0;i<N-4;i++)caulpe[i].active=0;for(int i=0;i<N-6;i++)caulhs[i].active=0;for(int i=0;i<N-6;i++)caulms[i].active=0;init=1;ps("[CAUL] Cauliflower initialized\n");return 0;}
int caul_planting(int lc,int hd,int hw,int ls,int cw,int dm,int sc){return add(caulps,&st.n_plant,&st.t_head,N,lc,hd,hw,ls,cw,dm,sc);}
int caul_blanching(int lc,int hd,int hw,int ls,int cw,int dm,int sc){return add(caulbs,&st.n_blanch,&st.t_wt,N-2,lc,hd,hw,ls,cw,dm,sc);}
int caul_pest_control(int lc,int hd,int hw,int ls,int cw,int dm,int sc){return add(caulpe,&st.n_pest,&st.t_leaf,N-4,lc,hd,hw,ls,cw,dm,sc);}
int caul_harvest(int lc,int hd,int hw,int ls,int cw,int dm,int sc){return add(caulhs,&st.n_harv,&st.t_curd,N-6,lc,hd,hw,ls,cw,dm,sc);}
int caul_market(int lc,int hd,int hw,int ls,int cw,int dm,int sc){return add(caulms,&st.n_mkt,&st.t_days,N-6,lc,hd,hw,ls,cw,dm,sc);}
void caul_report(void){ps("[CAUL] Plant: ");pi(st.n_plant);ps(" Head=");pi(st.t_head);ps("\nBlanch: ");pi(st.n_blanch);ps(" Wt=");pi(st.t_wt);ps("\nPest: ");pi(st.n_pest);ps(" Leaf=");pi(st.t_leaf);ps("\nHarv: ");pi(st.n_harv);ps(" Curd=");pi(st.t_curd);ps("\nMkt: ");pi(st.n_mkt);ps(" Days=");pi(st.t_days);ps("\n");}
void caul_state(void){ps("[CAUL] Plant=");pi(st.n_plant);ps(" Blanch=");pi(st.n_blanch);ps(" Pest=");pi(st.n_pest);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cauliflower Admin Demo ===\n\n");caul_init();
/* 1=field 2=garden 3=greenhouse 4=organic 5=market_farm */
ps("Cauliflower planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,head_dia=10+(i*3),head_wt=1+(i%4),leaf_sp=30+(i*5),curd_wt=8+(i%3),days_mat=60+(i*5),seed_ct=20+(i*10);caul_planting(lc,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct);}
ps("\nCauliflower blanching...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,head_dia=12+(i*2),head_wt=2+(i%3),leaf_sp=35+(i*4),curd_wt=9+(i%2),days_mat=65+(i*4),seed_ct=25+(i*8);caul_blanching(lc,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct);}
ps("\nCauliflower pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,head_dia=14+(i*2),head_wt=3+(i%2),leaf_sp=40+(i*3),curd_wt=10+(i%2),days_mat=70+(i*3),seed_ct=30+(i*6);caul_pest_control(lc,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct);}
ps("\nCauliflower harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,head_dia=8+(i*4),head_wt=1+(i%4),leaf_sp=25+(i*6),curd_wt=7+(i%3),days_mat=55+(i*6),seed_ct=15+(i*12);caul_harvest(lc,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct);}
ps("\nCauliflower market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,head_dia=16+(i*2),head_wt=4+(i%2),leaf_sp=45+(i*3),curd_wt=11+(i%2),days_mat=75+(i*3),seed_ct=35+(i*5);caul_market(lc,head_dia,head_wt,leaf_sp,curd_wt,days_mat,seed_ct);}
ps("\n");caul_report();caul_state();ps("\n=== Demo Complete ===\n");return 0;}
