/* kiwi_admin: Kiwi (Apteryx mantelli) North Island flightless bird (v1.0)
 * Kiwi forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, run_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,rn_sp,fth_idx,age_yr,active;} kiwi_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_rn,t_fth;} kiwi_state_t;
static kiwi_t frs[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static kiwi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kiwi_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int rs,int fi,int ay){if(*cnt>=mx)return -1;kiwi_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->rn_sp=rs;x->fth_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KIWI] Kiwi ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kiwi_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_rn=0;st.t_fth=0;for(int i=0;i<N;i++)frs[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[KIWI] Kiwi initialized\n");return 0;}
int kiwi_forest(int lc,int bl,int bw,int bc,int rs,int fi,int ay){return add(frs,&st.n_forest,&st.t_ln,N,lc,bl,bw,bc,rs,fi,ay);}
int kiwi_feeding(int lc,int bl,int bw,int bc,int rs,int fi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,rs,fi,ay);}
int kiwi_breeding(int lc,int bl,int bw,int bc,int rs,int fi,int ay){return add(frb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,rs,fi,ay);}
int kiwi_health(int lc,int bl,int bw,int bc,int rs,int fi,int ay){return add(frh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,bc,rs,fi,ay);}
int kiwi_market(int lc,int bl,int bw,int bc,int rs,int fi,int ay){return add(frm,&st.n_mkt,&st.t_fth,N-6,lc,bl,bw,bc,rs,fi,ay);}
void kiwi_report(void){ps("[KIWI] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fth=");pi(st.t_fth);ps("\n");}
void kiwi_state(void){ps("[KIWI] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kiwi Admin Demo ===\n\n");kiwi_init();
ps("Kiwi forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=2000+(i*150),bc=12+(i*2),rs=5+(i*2),fi=(i%8)+1,ay=(i%25)+1;kiwi_forest(lc,bl,bw,bc,rs,fi,ay);}
ps("\nKiwi feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=2100+(i*120),bc=13+(i*2),rs=6+(i*1),fi=(i%7)+1,ay=(i%22)+1;kiwi_feeding(lc,bl,bw,bc,rs,fi,ay);}
ps("\nKiwi breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=49+(i*2),bw=2200+(i*100),bc=14+(i*1),rs=4+(i*2),fi=(i%6)+1,ay=(i%18)+1;kiwi_breeding(lc,bl,bw,bc,rs,fi,ay);}
ps("\nKiwi health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=44+(i*4),bw=1900+(i*180),bc=11+(i*3),rs=3+(i*3),fi=(i%9)+1,ay=(i%15)+1;kiwi_health(lc,bl,bw,bc,rs,fi,ay);}
ps("\nKiwi market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=51+(i*2),bw=2300+(i*80),bc=15+(i*1),rs=7+(i*1),fi=(i%5)+1,ay=(i%12)+1;kiwi_market(lc,bl,bw,bc,rs,fi,ay);}
ps("\n");kiwi_report();kiwi_state();ps("\n=== Demo Complete ===\n");return 0;}
