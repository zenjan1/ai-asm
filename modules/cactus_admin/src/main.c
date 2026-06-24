/* cactus_admin: Cactus desert succulent cultivation and management (v1.0)
 * Cactus cultivation, propagation, pest control, exhibition, market
 * Features: height_cm, spine_count, flower_diameter_cm, water_store_ml, root_depth_cm, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,ht_cm,spine_ct,flr_dia,wtr_ml,root_dp,bloom_wk,active;} cact_t;
typedef struct{int n_cult,n_prop,n_pest,n_exhibit,n_mkt,t_ht,t_spine,t_flr,t_wtr,t_root;} cact_state_t;
static cact_t cactps[N],cactpr[N-2],cactpe[N-4],cactex[N-6],cactms[N-6]; static cact_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cact_t*a,int*cnt,int*sum,int mx,int lc,int ht,int sp,int fd,int wm,int rd,int bw){if(*cnt>=mx)return -1;cact_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->ht_cm=ht;x->spine_ct=sp;x->flr_dia=fd;x->wtr_ml=wm;x->root_dp=rd;x->bloom_wk=bw;x->active=1;*sum+=ht;(*cnt)++;ps("[CACT] Cactus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ht=");pi(ht);ps(" sp=");pi(sp);ps(" fd=");pi(fd);ps(" wm=");pi(wm);ps(" rd=");pi(rd);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cact_init(void){if(init)return -1;st.n_cult=0;st.n_prop=0;st.n_pest=0;st.n_exhibit=0;st.n_mkt=0;st.t_ht=0;st.t_spine=0;st.t_flr=0;st.t_wtr=0;st.t_root=0;for(int i=0;i<N;i++)cactps[i].active=0;for(int i=0;i<N-2;i++)cactpr[i].active=0;for(int i=0;i<N-4;i++)cactpe[i].active=0;for(int i=0;i<N-6;i++)cactex[i].active=0;for(int i=0;i<N-6;i++)cactms[i].active=0;init=1;ps("[CACT] Cactus initialized\n");return 0;}
int cact_cultivation(int lc,int ht,int sp,int fd,int wm,int rd,int bw){return add(cactps,&st.n_cult,&st.t_ht,N,lc,ht,sp,fd,wm,rd,bw);}
int cact_propagation(int lc,int ht,int sp,int fd,int wm,int rd,int bw){return add(cactpr,&st.n_prop,&st.t_spine,N-2,lc,ht,sp,fd,wm,rd,bw);}
int cact_pest_control(int lc,int ht,int sp,int fd,int wm,int rd,int bw){return add(cactpe,&st.n_pest,&st.t_flr,N-4,lc,ht,sp,fd,wm,rd,bw);}
int cact_exhibition(int lc,int ht,int sp,int fd,int wm,int rd,int bw){return add(cactex,&st.n_exhibit,&st.t_wtr,N-6,lc,ht,sp,fd,wm,rd,bw);}
int cact_market(int lc,int ht,int sp,int fd,int wm,int rd,int bw){return add(cactms,&st.n_mkt,&st.t_root,N-6,lc,ht,sp,fd,wm,rd,bw);}
void cact_report(void){ps("[CACT] Cult: ");pi(st.n_cult);ps(" Ht=");pi(st.t_ht);ps("\nProp: ");pi(st.n_prop);ps(" Spine=");pi(st.t_spine);ps("\nPest: ");pi(st.n_pest);ps(" Flr=");pi(st.t_flr);ps("\nExhibit: ");pi(st.n_exhibit);ps(" Wtr=");pi(st.t_wtr);ps("\nMkt: ");pi(st.n_mkt);ps(" Root=");pi(st.t_root);ps("\n");}
void cact_state(void){ps("[CACT] Cult=");pi(st.n_cult);ps(" Prop=");pi(st.n_prop);ps(" Pest=");pi(st.n_pest);ps(" Exhibit=");pi(st.n_exhibit);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cactus Admin Demo ===\n\n");cact_init();
/* 1=desert 2=greenhouse 3=xeriscape 4=container 5=rock_garden */
ps("Cactus cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ht=10+(i*5),sp=20+(i*10),fd=3+(i%5),wm=50+(i*20),rd=15+(i*8),bw=20+(i%4);cact_cultivation(lc,ht,sp,fd,wm,rd,bw);}
ps("\nCactus propagation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ht=12+(i*4),sp=25+(i*8),fd=4+(i%4),wm=60+(i*15),rd=18+(i*6),bw=22+(i%3);cact_propagation(lc,ht,sp,fd,wm,rd,bw);}
ps("\nCactus pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ht=15+(i*3),sp=30+(i*6),fd=5+(i%3),wm=70+(i*12),rd=20+(i*5),bw=24+(i%3);cact_pest_control(lc,ht,sp,fd,wm,rd,bw);}
ps("\nCactus exhibition...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ht=8+(i*6),sp=15+(i*12),fd=2+(i%5),wm=40+(i*25),rd=12+(i*10),bw=18+(i%5);cact_exhibition(lc,ht,sp,fd,wm,rd,bw);}
ps("\nCactus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ht=18+(i*3),sp=35+(i*5),fd=6+(i%2),wm=80+(i*10),rd=25+(i*4),bw=26+(i%2);cact_market(lc,ht,sp,fd,wm,rd,bw);}
ps("\n");cact_report();cact_state();ps("\n=== Demo Complete ===\n");return 0;}
