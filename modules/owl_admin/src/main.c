/* owl_admin: Owl (Bubo bubo) Eurasian eagle-owl nocturnal raptor (v1.0)
 * Owl forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wingspan_cm, fly_speed, tl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wn_sp,fl_sp,tl_idx,age_yr,active;} owl_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wn,t_fl,t_tl;} owl_state_t;
static owl_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static owl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(owl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int fs,int ti,int ay){if(*cnt>=mx)return -1;owl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wn_sp=ws;x->fl_sp=fs;x->tl_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OWL] Owl ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" fs=");pi(fs);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int owl_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wn=0;st.t_fl=0;st.t_tl=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[OWL] Owl initialized\n");return 0;}
int owl_forest(int lc,int bl,int bw,int ws,int fs,int ti,int ay){return add(arr_f,&st.n_forest,&st.t_ln,N,lc,bl,bw,ws,fs,ti,ay);}
int owl_feeding(int lc,int bl,int bw,int ws,int fs,int ti,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,fs,ti,ay);}
int owl_breeding(int lc,int bl,int bw,int ws,int fs,int ti,int ay){return add(arr_b,&st.n_breed,&st.t_wn,N-4,lc,bl,bw,ws,fs,ti,ay);}
int owl_health(int lc,int bl,int bw,int ws,int fs,int ti,int ay){return add(arr_h,&st.n_health,&st.t_fl,N-6,lc,bl,bw,ws,fs,ti,ay);}
int owl_market(int lc,int bl,int bw,int ws,int fs,int ti,int ay){return add(arr_m,&st.n_mkt,&st.t_tl,N-6,lc,bl,bw,ws,fs,ti,ay);}
void owl_report(void){ps("[OWL] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wn=");pi(st.t_wn);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Tl=");pi(st.t_tl);ps("\n");}
void owl_state(void){ps("[OWL] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Owl Admin Demo ===\n\n");owl_init();
ps("Owl forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*3),bw=1500+(i*100),ws=120+(i*8),fs=40+(i*5),ti=(i%7)+1,ay=(i%20)+1;owl_forest(lc,bl,bw,ws,fs,ti,ay);}
ps("\nOwl feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=52+(i*2),bw=1600+(i*80),ws=125+(i*6),fs=42+(i*4),ti=(i%6)+1,ay=(i%18)+1;owl_feeding(lc,bl,bw,ws,fs,ti,ay);}
ps("\nOwl breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=55+(i*2),bw=1700+(i*70),ws=130+(i*5),fs=45+(i*3),ti=(i%5)+1,ay=(i%15)+1;owl_breeding(lc,bl,bw,ws,fs,ti,ay);}
ps("\nOwl health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=48+(i*4),bw=1400+(i*120),ws=115+(i*9),fs=38+(i*6),ti=(i%8)+1,ay=(i%12)+1;owl_health(lc,bl,bw,ws,fs,ti,ay);}
ps("\nOwl market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=58+(i*2),bw=1800+(i*60),ws=135+(i*4),fs=48+(i*3),ti=(i%4)+1,ay=(i%10)+1;owl_market(lc,bl,bw,ws,fs,ti,ay);}
ps("\n");owl_report();owl_state();ps("\n=== Demo Complete ===\n");return 0;}
