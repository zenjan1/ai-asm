/* dove_admin: Dove (Streptopelia decaocto) Eurasian collared dove bird (v1.0)
 * Dove garden, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,pm_idx,age_yr,active;} dove_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_pm;} dove_state_t;
static dove_t gdl[N],gdf[N-2],gdb[N-4],gdh[N-6],gdm[N-6]; static dove_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dove_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;dove_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DOVE] Dove ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dove_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)gdl[i].active=0;for(int i=0;i<N-2;i++)gdf[i].active=0;for(int i=0;i<N-4;i++)gdb[i].active=0;for(int i=0;i<N-6;i++)gdh[i].active=0;for(int i=0;i<N-6;i++)gdm[i].active=0;init=1;ps("[DOVE] Dove initialized\n");return 0;}
int dove_garden(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(gdl,&st.n_garden,&st.t_ln,N,lc,bl,bw,wc,fs,pi2,ay);}
int dove_feeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(gdf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,pi2,ay);}
int dove_breeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(gdb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,pi2,ay);}
int dove_health(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(gdh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,pi2,ay);}
int dove_market(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(gdm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,wc,fs,pi2,ay);}
void dove_report(void){ps("[DOVE] Garden: ");pi(st.n_garden);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void dove_state(void){ps("[DOVE] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dove Admin Demo ===\n\n");dove_init();
ps("Dove garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=130+(i*15),wc=18+(i*2),fs=15+(i*2),pi2=(i%8)+1,ay=(i%10)+1;dove_garden(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nDove feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*2),bw=140+(i*12),wc=19+(i%2),fs=16+(i*1),pi2=(i%6)+1,ay=(i%8)+1;dove_feeding(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nDove breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i%2),bw=150+(i*10),wc=20+(i%2),fs=14+(i*3),pi2=(i%5)+1,ay=(i%7)+1;dove_breeding(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nDove health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=24+(i*3),bw=125+(i*18),wc=17+(i*3),fs=12+(i*4),pi2=(i%10)+1,ay=(i%6)+1;dove_health(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nDove market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=30+(i%2),bw=160+(i*8),wc=21+(i%2),fs=18+(i*2),pi2=(i%4)+1,ay=(i%5)+1;dove_market(lc,bl,bw,wc,fs,pi2,ay);}
ps("\n");dove_report();dove_state();ps("\n=== Demo Complete ===\n");return 0;}
