/* ginseng_admin: Ginseng (Panax ginseng) medicinal root herb (v1.0)
 * Ginseng soil, feeding, pruning, health, market
 * Features: root_len_cm, root_wt_g, branch_ct, ginsenoside_idx, leaf_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,rt_ln,rt_wt,br_ct,gn_idx,lf_ct,age_yr,active;} ginseng_t;
typedef struct{int n_soil,n_feed,n_prun,n_health,n_mkt,t_ln,t_wt,t_br,t_gn,t_lf;} ginseng_state_t;
static ginseng_t gpl[N],gfd[N-2],gpr[N-4],ghl[N-6],gmk[N-6]; static ginseng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ginseng_t*a,int*cnt,int*sum,int mx,int lc,int rl,int rw,int bc,int gi,int lfc,int ay){if(*cnt>=mx)return -1;ginseng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->rt_ln=rl;x->rt_wt=rw;x->br_ct=bc;x->gn_idx=gi;x->lf_ct=lfc;x->age_yr=ay;x->active=1;*sum+=rl;(*cnt)++;ps("[GIN] Ginseng ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rl=");pi(rl);ps(" rw=");pi(rw);ps(" bc=");pi(bc);ps(" gi=");pi(gi);ps(" lfc=");pi(lfc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ginseng_init(void){if(init)return -1;st.n_soil=0;st.n_feed=0;st.n_prun=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_br=0;st.t_gn=0;st.t_lf=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gpr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GIN] Ginseng initialized\n");return 0;}
int ginseng_soil(int lc,int rl,int rw,int bc,int gi,int lfc,int ay){return add(gpl,&st.n_soil,&st.t_ln,N,lc,rl,rw,bc,gi,lfc,ay);}
int ginseng_feeding(int lc,int rl,int rw,int bc,int gi,int lfc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,rl,rw,bc,gi,lfc,ay);}
int ginseng_pruning(int lc,int rl,int rw,int bc,int gi,int lfc,int ay){return add(gpr,&st.n_prun,&st.t_br,N-4,lc,rl,rw,bc,gi,lfc,ay);}
int ginseng_health(int lc,int rl,int rw,int bc,int gi,int lfc,int ay){return add(ghl,&st.n_health,&st.t_gn,N-6,lc,rl,rw,bc,gi,lfc,ay);}
int ginseng_market(int lc,int rl,int rw,int bc,int gi,int lfc,int ay){return add(gmk,&st.n_mkt,&st.t_lf,N-6,lc,rl,rw,bc,gi,lfc,ay);}
void ginseng_report(void){ps("[GIN] Soil: ");pi(st.n_soil);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nPrun: ");pi(st.n_prun);ps(" Br=");pi(st.t_br);ps("\nHlth: ");pi(st.n_health);ps(" Gn=");pi(st.t_gn);ps("\nMkt: ");pi(st.n_mkt);ps(" Lf=");pi(st.t_lf);ps("\n");}
void ginseng_state(void){ps("[GIN] Soil=");pi(st.n_soil);ps(" Feed=");pi(st.n_feed);ps(" Prun=");pi(st.n_prun);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ginseng Admin Demo ===\n\n");ginseng_init();
/* 1=mountain 2=forest 3=garden 4=farm 5=greenhouse */
ps("Ginseng soil...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,rl=15+(i*3),rw=20+(i*8),bc=2+(i%4),gi=40+(i*5),lfc=4+(i%3),ay=(i%6)+1;ginseng_soil(lc,rl,rw,bc,gi,lfc,ay);}
ps("\nGinseng feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,rl=18+(i*2),rw=25+(i*6),bc=3+(i%3),gi=45+(i*4),lfc=5+(i%3),ay=(i%5)+1;ginseng_feeding(lc,rl,rw,bc,gi,lfc,ay);}
ps("\nGinseng pruning...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,rl=12+(i*4),rw=18+(i*7),bc=2+(i%5),gi=35+(i*6),lfc=3+(i%4),ay=(i%4)+2;ginseng_pruning(lc,rl,rw,bc,gi,lfc,ay);}
ps("\nGinseng health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,rl=20+(i*2),rw=30+(i*5),bc=4+(i%3),gi=50+(i*4),lfc=6+(i%2),ay=(i%5)+1;ginseng_health(lc,rl,rw,bc,gi,lfc,ay);}
ps("\nGinseng market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,rl=25+(i*2),rw=35+(i*4),bc=5+(i%2),gi=55+(i*3),lfc=7+(i%2),ay=(i%3)+3;ginseng_market(lc,rl,rw,bc,gi,lfc,ay);}
ps("\n");ginseng_report();ginseng_state();ps("\n=== Demo Complete ===\n");return 0;}
