/* lemur_admin: Lemur (Lemur catta) ring-tailed lemur primate (v1.0)
 * Lemur habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, leap_m, vocal_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,lp_m,v_idx,age_yr,active;} lemur_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_lp,t_v;} lemur_state_t;
static lemur_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static lemur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lemur_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int lm,int vi,int ay){if(*cnt>=mx)return -1;lemur_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->lp_m=lm;x->v_idx=vi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LEM] Lemur ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" lm=");pi(lm);ps(" vi=");pi(vi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int lemur_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_lp=0;st.t_v=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LEM] Lemur initialized\n");return 0;}
int lemur_habitat(int lc,int bl,int bw,int tc,int lm,int vi,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,tc,lm,vi,ay);}
int lemur_feeding(int lc,int bl,int bw,int tc,int lm,int vi,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,lm,vi,ay);}
int lemur_breeding(int lc,int bl,int bw,int tc,int lm,int vi,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,lm,vi,ay);}
int lemur_health(int lc,int bl,int bw,int tc,int lm,int vi,int ay){return add(ghl,&st.n_health,&st.t_lp,N-6,lc,bl,bw,tc,lm,vi,ay);}
int lemur_market(int lc,int bl,int bw,int tc,int lm,int vi,int ay){return add(gmk,&st.n_mkt,&st.t_v,N-6,lc,bl,bw,tc,lm,vi,ay);}
void lemur_report(void){ps("[LEM] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Lp=");pi(st.t_lp);ps("\nMkt: ");pi(st.n_mkt);ps(" V=");pi(st.t_v);ps("\n");}
void lemur_state(void){ps("[LEM] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Lemur Admin Demo ===\n\n");lemur_init();
/* 1=forest 2=bush 3=reserve 4=sanctuary 5=zoo */
ps("Lemur habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=2+(i*2),tc=50+(i*3),lm=2+(i%4),vi=(i%5)+1,ay=(i%6)+1;lemur_habitat(lc,bl,bw,tc,lm,vi,ay);}
ps("\nLemur feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=42+(i*2),bw=2+(i*2),tc=52+(i*2),lm=3+(i%3),vi=(i%4)+1,ay=(i%5)+1;lemur_feeding(lc,bl,bw,tc,lm,vi,ay);}
ps("\nLemur breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*4),bw=3+(i*2),tc=48+(i*4),lm=2+(i%4),vi=(i%3)+2,ay=(i%4)+1;lemur_breeding(lc,bl,bw,tc,lm,vi,ay);}
ps("\nLemur health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*2),bw=3+(i*2),tc=55+(i*2),lm=4+(i%3),vi=(i%5)+1,ay=(i%5)+1;lemur_health(lc,bl,bw,tc,lm,vi,ay);}
ps("\nLemur market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=48+(i*2),bw=3+(i*2),tc=58+(i*2),lm=5+(i%2),vi=(i%4)+1,ay=(i%3)+2;lemur_market(lc,bl,bw,tc,lm,vi,ay);}
ps("\n");lemur_report();lemur_state();ps("\n=== Demo Complete ===\n");return 0;}
