/* goldfish_admin: Goldfish (Carassius auratus) ornamental aquarium fish (v1.0)
 * Goldfish tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_span_cm, color_idx, scale_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_sp,cl_idx,sc_ct,age_yr,active;} goldfish_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_cl,t_sc;} goldfish_state_t;
static goldfish_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static goldfish_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(goldfish_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fs,int ci,int sc,int ay){if(*cnt>=mx)return -1;goldfish_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_sp=fs;x->cl_idx=ci;x->sc_ct=sc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GOLD] Goldfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fs=");pi(fs);ps(" ci=");pi(ci);ps(" sc=");pi(sc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int goldfish_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_cl=0;st.t_sc=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GOLD] Goldfish initialized\n");return 0;}
int goldfish_tank(int lc,int bl,int bw,int fs,int ci,int sc,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,fs,ci,sc,ay);}
int goldfish_feeding(int lc,int bl,int bw,int fs,int ci,int sc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fs,ci,sc,ay);}
int goldfish_breeding(int lc,int bl,int bw,int fs,int ci,int sc,int ay){return add(gbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fs,ci,sc,ay);}
int goldfish_health(int lc,int bl,int bw,int fs,int ci,int sc,int ay){return add(ghl,&st.n_health,&st.t_cl,N-6,lc,bl,bw,fs,ci,sc,ay);}
int goldfish_market(int lc,int bl,int bw,int fs,int ci,int sc,int ay){return add(gmk,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fs,ci,sc,ay);}
void goldfish_report(void){ps("[GOLD] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHlth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void goldfish_state(void){ps("[GOLD] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Goldfish Admin Demo ===\n\n");goldfish_init();
/* 1=bowl 2=pond 3=aquarium 4=tank 5=show */
ps("Goldfish tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*2),bw=10+(i*5),fs=4+(i%3),ci=(i%4)+1,sc=20+(i*8),ay=(i%4)+1;goldfish_tank(lc,bl,bw,fs,ci,sc,ay);}
ps("\nGoldfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=10+(i*2),bw=12+(i*4),fs=5+(i%3),ci=(i%3)+1,sc=25+(i*6),ay=(i%3)+1;goldfish_feeding(lc,bl,bw,fs,ci,sc,ay);}
ps("\nGoldfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=6+(i*3),bw=8+(i*5),fs=3+(i%4),ci=(i%3)+2,sc=15+(i*7),ay=(i%3)+1;goldfish_breeding(lc,bl,bw,fs,ci,sc,ay);}
ps("\nGoldfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i*2),bw=15+(i*3),fs=6+(i%2),ci=(i%5)+1,sc=30+(i*5),ay=(i%4)+1;goldfish_health(lc,bl,bw,fs,ci,sc,ay);}
ps("\nGoldfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=14+(i*2),bw=18+(i*3),fs=7+(i%2),ci=(i%4)+1,sc=35+(i*4),ay=(i%3)+1;goldfish_market(lc,bl,bw,fs,ci,sc,ay);}
ps("\n");goldfish_report();goldfish_state();ps("\n=== Demo Complete ===\n");return 0;}
