/* hawk_admin: Hawk (Buteo buteo) common medium raptor bird (v1.0)
 * Hawk forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wng_cm,fl_sp,fth_idx,age_yr,active;} hwk_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wng,t_fl,t_fth;} hwk_state_t;
static hwk_t frs[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static hwk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hwk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int fi,int ay){if(*cnt>=mx)return -1;hwk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wng_cm=wc;x->fl_sp=fs;x->fth_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HWK] Hawk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hawk_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wng=0;st.t_fl=0;st.t_fth=0;for(int i=0;i<N;i++)frs[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[HWK] Hawk initialized\n");return 0;}
int hawk_forest(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(frs,&st.n_forest,&st.t_ln,N,lc,bl,bw,wc,fs,fi,ay);}
int hawk_feeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,fi,ay);}
int hawk_breeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(frb,&st.n_breed,&st.t_wng,N-4,lc,bl,bw,wc,fs,fi,ay);}
int hawk_health(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(frh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,fi,ay);}
int hawk_market(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(frm,&st.n_mkt,&st.t_fth,N-6,lc,bl,bw,wc,fs,fi,ay);}
void hawk_report(void){ps("[HWK] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wng=");pi(st.t_wng);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fth=");pi(st.t_fth);ps("\n");}
void hawk_state(void){ps("[HWK] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hawk Admin Demo ===\n\n");hawk_init();
ps("Hawk forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=800+(i*60),wc=90+(i*5),fs=15+(i*4),fi=(i%8)+1,ay=(i%10)+1;hawk_forest(lc,bl,bw,wc,fs,fi,ay);}
ps("\nHawk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=820+(i*50),wc=92+(i*4),fs=16+(i*3),fi=(i%7)+1,ay=(i%9)+1;hawk_feeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nHawk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=49+(i*2),bw=840+(i*40),wc=94+(i*3),fs=14+(i*5),fi=(i%6)+1,ay=(i%8)+1;hawk_breeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nHawk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=44+(i*4),bw=780+(i*70),wc=88+(i*6),fs=13+(i*6),fi=(i%9)+1,ay=(i%7)+1;hawk_health(lc,bl,bw,wc,fs,fi,ay);}
ps("\nHawk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=51+(i*2),bw=860+(i*35),wc=96+(i*3),fs=17+(i*3),fi=(i%5)+1,ay=(i%6)+1;hawk_market(lc,bl,bw,wc,fs,fi,ay);}
ps("\n");hawk_report();hawk_state();ps("\n=== Demo Complete ===\n");return 0;}
