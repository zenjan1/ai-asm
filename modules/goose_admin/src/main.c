/* goose_admin: Goose (Anser anser) greylag waterfowl bird (v1.0)
 * Goose lake, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,pm_idx,age_yr,active;} gse_t;
typedef struct{int n_lake,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_pm;} gse_state_t;
static gse_t lke[N],lkf[N-2],lkb[N-4],lkh[N-6],lkm[N-6]; static gse_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gse_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int pm,int ay){if(*cnt>=mx)return -1;gse_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->pm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GSE] Goose ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int goose_init(void){if(init)return -1;st.n_lake=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)lke[i].active=0;for(int i=0;i<N-2;i++)lkf[i].active=0;for(int i=0;i<N-4;i++)lkb[i].active=0;for(int i=0;i<N-6;i++)lkh[i].active=0;for(int i=0;i<N-6;i++)lkm[i].active=0;init=1;ps("[GSE] Goose initialized\n");return 0;}
int goose_lake(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(lke,&st.n_lake,&st.t_ln,N,lc,bl,bw,bc,fs,pm,ay);}
int goose_feeding(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(lkf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,pm,ay);}
int goose_breeding(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(lkb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,pm,ay);}
int goose_health(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(lkh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,pm,ay);}
int goose_market(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(lkm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,fs,pm,ay);}
void goose_report(void){ps("[GSE] Lake: ");pi(st.n_lake);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void goose_state(void){ps("[GSE] Lake=");pi(st.n_lake);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Goose Admin Demo ===\n\n");goose_init();
ps("Goose lake...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=3000+(i*250),bc=6+(i*1),fs=18+(i*4),pm=(i%8)+1,ay=(i%10)+1;goose_lake(lc,bl,bw,bc,fs,pm,ay);}
ps("\nGoose feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=82+(i*4),bw=3100+(i*200),bc=6+(i*1),fs=19+(i*3),pm=(i%7)+1,ay=(i%9)+1;goose_feeding(lc,bl,bw,bc,fs,pm,ay);}
ps("\nGoose breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=84+(i*3),bw=3200+(i*150),bc=7+(i*1),fs=17+(i*5),pm=(i%6)+1,ay=(i%8)+1;goose_breeding(lc,bl,bw,bc,fs,pm,ay);}
ps("\nGoose health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*6),bw=2900+(i*300),bc=5+(i*2),fs=16+(i*6),pm=(i%9)+1,ay=(i%7)+1;goose_health(lc,bl,bw,bc,fs,pm,ay);}
ps("\nGoose market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=86+(i*3),bw=3300+(i*120),bc=7+(i*1),fs=20+(i*3),pm=(i%5)+1,ay=(i%6)+1;goose_market(lc,bl,bw,bc,fs,pm,ay);}
ps("\n");goose_report();goose_state();ps("\n=== Demo Complete ===\n");return 0;}
