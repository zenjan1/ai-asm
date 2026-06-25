/* impala_admin: Impala (Aepyceros melampus) impala antelope ungulate (v1.0)
 * Impala habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, leap_m, coat_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,lp_m,ct_idx,age_yr,active;} impala_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_lp,t_ct;} impala_state_t;
static impala_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static impala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(impala_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int lm,int ci,int ay){if(*cnt>=mx)return -1;impala_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->lp_m=lm;x->ct_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IMP] Impala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" lm=");pi(lm);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int impala_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_lp=0;st.t_ct=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[IMP] Impala initialized\n");return 0;}
int impala_habitat(int lc,int bl,int bw,int hc,int lm,int ci,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,hc,lm,ci,ay);}
int impala_feeding(int lc,int bl,int bw,int hc,int lm,int ci,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,lm,ci,ay);}
int impala_breeding(int lc,int bl,int bw,int hc,int lm,int ci,int ay){return add(gbr,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,lm,ci,ay);}
int impala_health(int lc,int bl,int bw,int hc,int lm,int ci,int ay){return add(ghl,&st.n_health,&st.t_lp,N-6,lc,bl,bw,hc,lm,ci,ay);}
int impala_market(int lc,int bl,int bw,int hc,int lm,int ci,int ay){return add(gmk,&st.n_mkt,&st.t_ct,N-6,lc,bl,bw,hc,lm,ci,ay);}
void impala_report(void){ps("[IMP] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHlth: ");pi(st.n_health);ps(" Lp=");pi(st.t_lp);ps("\nMkt: ");pi(st.n_mkt);ps(" Ct=");pi(st.t_ct);ps("\n");}
void impala_state(void){ps("[IMP] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Impala Admin Demo ===\n\n");impala_init();
/* 1=savanna 2=grassland 3=woodland 4=reserve 5=park */
ps("Impala habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*5),bw=40+(i*4),hc=40+(i*3),lm=3+(i%5),ci=(i%5)+1,ay=(i%6)+1;impala_habitat(lc,bl,bw,hc,lm,ci,ay);}
ps("\nImpala feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=125+(i*4),bw=42+(i*3),hc=42+(i*3),lm=4+(i%4),ci=(i%4)+1,ay=(i%5)+1;impala_feeding(lc,bl,bw,hc,lm,ci,ay);}
ps("\nImpala breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=115+(i*6),bw=38+(i*5),hc=38+(i*4),lm=3+(i%5),ci=(i%3)+2,ay=(i%4)+1;impala_breeding(lc,bl,bw,hc,lm,ci,ay);}
ps("\nImpala health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=130+(i*4),bw=45+(i*3),hc=45+(i*2),lm=5+(i%3),ci=(i%5)+1,ay=(i%5)+1;impala_health(lc,bl,bw,hc,lm,ci,ay);}
ps("\nImpala market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=135+(i*3),bw=48+(i*3),hc=48+(i*2),lm=6+(i%3),ci=(i%4)+1,ay=(i%3)+2;impala_market(lc,bl,bw,hc,lm,ci,ay);}
ps("\n");impala_report();impala_state();ps("\n=== Demo Complete ===\n");return 0;}
