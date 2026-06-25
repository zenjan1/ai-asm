/* condor_admin: Condor (Vultur gryphus) Andean large vulture (v1.0)
 * Condor mountain, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_cm, fly_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,ft_idx,age_yr,active;} cndr_t;
typedef struct{int n_mount,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_ft;} cndr_state_t;
static cndr_t mtl[N],mtf[N-2],mtb[N-4],mth[N-6],mtm[N-6]; static cndr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cndr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int fi,int ay){if(*cnt>=mx)return -1;cndr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CNDR] Condor ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int condor_init(void){if(init)return -1;st.n_mount=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_ft=0;for(int i=0;i<N;i++)mtl[i].active=0;for(int i=0;i<N-2;i++)mtf[i].active=0;for(int i=0;i<N-4;i++)mtb[i].active=0;for(int i=0;i<N-6;i++)mth[i].active=0;for(int i=0;i<N-6;i++)mtm[i].active=0;init=1;ps("[CNDR] Condor initialized\n");return 0;}
int condor_mountain(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(mtl,&st.n_mount,&st.t_ln,N,lc,bl,bw,wc,fs,fi,ay);}
int condor_feeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(mtf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,fi,ay);}
int condor_breeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(mtb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,fi,ay);}
int condor_health(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(mth,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,fi,ay);}
int condor_market(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(mtm,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,wc,fs,fi,ay);}
void condor_report(void){ps("[CNDR] Mount: ");pi(st.n_mount);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void condor_state(void){ps("[CNDR] Mount=");pi(st.n_mount);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Condor Admin Demo ===\n\n");condor_init();
ps("Condor mountain...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=6+(i*2),wc=280+(i*10),fs=40+(i*3),fi=(i%8)+1,ay=(i%25)+1;condor_mountain(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCondor feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*6),bw=7+(i*2),wc=290+(i*8),fs=45+(i*2),fi=(i%6)+1,ay=(i%20)+1;condor_feeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCondor breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*5),bw=8+(i*1),wc=300+(i*6),fs=35+(i*4),fi=(i%5)+1,ay=(i%15)+1;condor_breeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCondor health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*9),bw=5+(i*3),wc=275+(i*12),fs=30+(i*5),fi=(i%10)+1,ay=(i%12)+1;condor_health(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCondor market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*4),bw=9+(i*1),wc=310+(i*5),fs=50+(i*2),fi=(i%4)+1,ay=(i%10)+1;condor_market(lc,bl,bw,wc,fs,fi,ay);}
ps("\n");condor_report();condor_state();ps("\n=== Demo Complete ===\n");return 0;}
