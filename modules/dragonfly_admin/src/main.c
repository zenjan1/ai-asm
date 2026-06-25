/* dragonfly_admin: Dragonfly (Anax imperator) emperor flying insect (v1.0)
 * Dragonfly pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, eye_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,ey_idx,age_yr,active;} dgfl_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_ey;} dgfl_state_t;
static dgfl_t pdl[N],pdf[N-2],pdb[N-4],pdh[N-6],pdm[N-6]; static dgfl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dgfl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int ei,int ay){if(*cnt>=mx)return -1;dgfl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->ey_idx=ei;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DGFL] Dragonfly ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" ei=");pi(ei);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dragonfly_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_ey=0;for(int i=0;i<N;i++)pdl[i].active=0;for(int i=0;i<N-2;i++)pdf[i].active=0;for(int i=0;i<N-4;i++)pdb[i].active=0;for(int i=0;i<N-6;i++)pdh[i].active=0;for(int i=0;i<N-6;i++)pdm[i].active=0;init=1;ps("[DGFL] Dragonfly initialized\n");return 0;}
int dragonfly_pond(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(pdl,&st.n_pond,&st.t_ln,N,lc,bl,bw,wc,fs,ei,ay);}
int dragonfly_feeding(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(pdf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,ei,ay);}
int dragonfly_breeding(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(pdb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,ei,ay);}
int dragonfly_health(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(pdh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,ei,ay);}
int dragonfly_market(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(pdm,&st.n_mkt,&st.t_ey,N-6,lc,bl,bw,wc,fs,ei,ay);}
void dragonfly_report(void){ps("[DGFL] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ey=");pi(st.t_ey);ps("\n");}
void dragonfly_state(void){ps("[DGFL] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dragonfly Admin Demo ===\n\n");dragonfly_init();
ps("Dragonfly pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i%4),bw=30+(i*5),wc=4+(i%3),fs=20+(i*3),ei=(i%8)+1,ay=(i%2)+1;dragonfly_pond(lc,bl,bw,wc,fs,ei,ay);}
ps("\nDragonfly feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=6+(i%3),bw=33+(i*4),wc=4+(i%2),fs=22+(i*2),ei=(i%6)+1,ay=(i%2)+1;dragonfly_feeding(lc,bl,bw,wc,fs,ei,ay);}
ps("\nDragonfly breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i%3),bw=36+(i*3),wc=5+(i%2),fs=18+(i*4),ei=(i%5)+1,ay=(i%2)+1;dragonfly_breeding(lc,bl,bw,wc,fs,ei,ay);}
ps("\nDragonfly health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i%5),bw=28+(i*6),wc=3+(i%4),fs=15+(i*5),ei=(i%10)+1,ay=(i%2)+1;dragonfly_health(lc,bl,bw,wc,fs,ei,ay);}
ps("\nDragonfly market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=8+(i%3),bw=38+(i*3),wc=5+(i%2),fs=25+(i*2),ei=(i%4)+1,ay=(i%2)+1;dragonfly_market(lc,bl,bw,wc,fs,ei,ay);}
ps("\n");dragonfly_report();dragonfly_state();ps("\n=== Demo Complete ===\n");return 0;}
