/* crow_admin: Crow (Corvus corax) common raven intelligent bird (v1.0)
 * Crow urban, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,ft_idx,age_yr,active;} crow_t;
typedef struct{int n_urban,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_ft;} crow_state_t;
static crow_t ubl[N],ubf[N-2],ubb[N-4],ubh[N-6],ubm[N-6]; static crow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crow_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int fi,int ay){if(*cnt>=mx)return -1;crow_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CROW] Crow ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int crow_init(void){if(init)return -1;st.n_urban=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_ft=0;for(int i=0;i<N;i++)ubl[i].active=0;for(int i=0;i<N-2;i++)ubf[i].active=0;for(int i=0;i<N-4;i++)ubb[i].active=0;for(int i=0;i<N-6;i++)ubh[i].active=0;for(int i=0;i<N-6;i++)ubm[i].active=0;init=1;ps("[CROW] Crow initialized\n");return 0;}
int crow_urban(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ubl,&st.n_urban,&st.t_ln,N,lc,bl,bw,wc,fs,fi,ay);}
int crow_feeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ubf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,fi,ay);}
int crow_breeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ubb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,fi,ay);}
int crow_health(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ubh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,fi,ay);}
int crow_market(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ubm,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,wc,fs,fi,ay);}
void crow_report(void){ps("[CROW] Urban: ");pi(st.n_urban);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void crow_state(void){ps("[CROW] Urban=");pi(st.n_urban);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crow Admin Demo ===\n\n");crow_init();
ps("Crow urban...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=300+(i*30),wc=25+(i*2),fs=15+(i*3),fi=(i%8)+1,ay=(i%10)+1;crow_urban(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCrow feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=320+(i*25),wc=26+(i*2),fs=17+(i*2),fi=(i%6)+1,ay=(i%8)+1;crow_feeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCrow breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=340+(i*20),wc=27+(i%2),fs=13+(i*4),fi=(i%5)+1,ay=(i%7)+1;crow_breeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCrow health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=290+(i*40),wc=24+(i*3),fs=10+(i*5),fi=(i%10)+1,ay=(i%6)+1;crow_health(lc,bl,bw,wc,fs,fi,ay);}
ps("\nCrow market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*2),bw=360+(i*15),wc=28+(i%2),fs=20+(i*3),fi=(i%4)+1,ay=(i%5)+1;crow_market(lc,bl,bw,wc,fs,fi,ay);}
ps("\n");crow_report();crow_state();ps("\n=== Demo Complete ===\n");return 0;}
