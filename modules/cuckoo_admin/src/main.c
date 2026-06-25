/* cuckoo_admin: Cuckoo (Cuculus canorus) common brood parasite bird (v1.0)
 * Cuckoo woodland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,fl_sp,pm_idx,age_yr,active;} ckoo_t;
typedef struct{int n_wood,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_fl,t_pm;} ckoo_state_t;
static ckoo_t wdl[N],wdf[N-2],wdb[N-4],wdh[N-6],wdm[N-6]; static ckoo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ckoo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;ckoo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->fl_sp=fs;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CKOO] Cuckoo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" fs=");pi(fs);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cuckoo_init(void){if(init)return -1;st.n_wood=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)wdl[i].active=0;for(int i=0;i<N-2;i++)wdf[i].active=0;for(int i=0;i<N-4;i++)wdb[i].active=0;for(int i=0;i<N-6;i++)wdh[i].active=0;for(int i=0;i<N-6;i++)wdm[i].active=0;init=1;ps("[CKOO] Cuckoo initialized\n");return 0;}
int cuckoo_woodland(int lc,int bl,int bw,int tc,int fs,int pi2,int ay){return add(wdl,&st.n_wood,&st.t_ln,N,lc,bl,bw,tc,fs,pi2,ay);}
int cuckoo_feeding(int lc,int bl,int bw,int tc,int fs,int pi2,int ay){return add(wdf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,fs,pi2,ay);}
int cuckoo_breeding(int lc,int bl,int bw,int tc,int fs,int pi2,int ay){return add(wdb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,fs,pi2,ay);}
int cuckoo_health(int lc,int bl,int bw,int tc,int fs,int pi2,int ay){return add(wdh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,tc,fs,pi2,ay);}
int cuckoo_market(int lc,int bl,int bw,int tc,int fs,int pi2,int ay){return add(wdm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,tc,fs,pi2,ay);}
void cuckoo_report(void){ps("[CKOO] Wood: ");pi(st.n_wood);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void cuckoo_state(void){ps("[CKOO] Wood=");pi(st.n_wood);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cuckoo Admin Demo ===\n\n");cuckoo_init();
ps("Cuckoo woodland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=100+(i*15),tc=15+(i*2),fs=12+(i*2),pi2=(i%8)+1,ay=(i%8)+1;cuckoo_woodland(lc,bl,bw,tc,fs,pi2,ay);}
ps("\nCuckoo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*2),bw=110+(i*12),tc=16+(i*1),fs=13+(i*1),pi2=(i%6)+1,ay=(i%7)+1;cuckoo_feeding(lc,bl,bw,tc,fs,pi2,ay);}
ps("\nCuckoo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*1),bw=120+(i*10),tc=17+(i%2),fs=11+(i*3),pi2=(i%5)+1,ay=(i%6)+1;cuckoo_breeding(lc,bl,bw,tc,fs,pi2,ay);}
ps("\nCuckoo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=24+(i*3),bw=95+(i*18),tc=14+(i*3),fs=9+(i*4),pi2=(i%10)+1,ay=(i%5)+1;cuckoo_health(lc,bl,bw,tc,fs,pi2,ay);}
ps("\nCuckoo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=30+(i*1),bw=130+(i*8),tc=18+(i%2),fs=15+(i*2),pi2=(i%4)+1,ay=(i%4)+1;cuckoo_market(lc,bl,bw,tc,fs,pi2,ay);}
ps("\n");cuckoo_report();cuckoo_state();ps("\n=== Demo Complete ===\n");return 0;}
