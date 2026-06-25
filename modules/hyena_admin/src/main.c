/* hyena_admin: Hyena (Crocuta crocuta) spotted hyena scavenger mammal (v1.0)
 * Hyena habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, jaw_kgf, laugh_vol, pack_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,jaw_kgf,lf_vl,pk_idx,age_yr,active;} hyena_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_jaw,t_lf,t_pk;} hyena_state_t;
static hyena_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hyena_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyena_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int jk,int lv,int pi2,int ay){if(*cnt>=mx)return -1;hyena_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->jaw_kgf=jk;x->lf_vl=lv;x->pk_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HYEN] Hyena ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" jk=");pi(jk);ps(" lv=");pi(lv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hyena_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_jaw=0;st.t_lf=0;st.t_pk=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HYEN] Hyena initialized\n");return 0;}
int hyena_habitat(int lc,int bl,int bw,int jk,int lv,int pi2,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,jk,lv,pi2,ay);}
int hyena_feeding(int lc,int bl,int bw,int jk,int lv,int pi2,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,jk,lv,pi2,ay);}
int hyena_breeding(int lc,int bl,int bw,int jk,int lv,int pi2,int ay){return add(gbr,&st.n_breed,&st.t_jaw,N-4,lc,bl,bw,jk,lv,pi2,ay);}
int hyena_health(int lc,int bl,int bw,int jk,int lv,int pi2,int ay){return add(ghl,&st.n_health,&st.t_lf,N-6,lc,bl,bw,jk,lv,pi2,ay);}
int hyena_market(int lc,int bl,int bw,int jk,int lv,int pi2,int ay){return add(gmk,&st.n_mkt,&st.t_pk,N-6,lc,bl,bw,jk,lv,pi2,ay);}
void hyena_report(void){ps("[HYEN] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Jaw=");pi(st.t_jaw);ps("\nHlth: ");pi(st.n_health);ps(" Lf=");pi(st.t_lf);ps("\nMkt: ");pi(st.n_mkt);ps(" Pk=");pi(st.t_pk);ps("\n");}
void hyena_state(void){ps("[HYEN] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hyena Admin Demo ===\n\n");hyena_init();
/* 1=savanna 2=scrub 3=grassland 4=den 5=reserve */
ps("Hyena habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*6),bw=40+(i*4),jk=400+(i*30),lv=50+(i*8),pk=(i%5)+1,ay=(i%6)+1;hyena_habitat(lc,bl,bw,jk,lv,pk,ay);}
ps("\nHyena feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=125+(i*5),bw=42+(i*3),jk=420+(i*25),lv=55+(i*6),pk=(i%4)+1,ay=(i%5)+1;hyena_feeding(lc,bl,bw,jk,lv,pk,ay);}
ps("\nHyena breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=115+(i*8),bw=38+(i*5),jk=380+(i*35),lv=45+(i*9),pk=(i%3)+2,ay=(i%4)+1;hyena_breeding(lc,bl,bw,jk,lv,pk,ay);}
ps("\nHyena health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=130+(i*5),bw=45+(i*3),jk=440+(i*20),lv=60+(i*5),pk=(i%5)+1,ay=(i%5)+1;hyena_health(lc,bl,bw,jk,lv,pk,ay);}
ps("\nHyena market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=135+(i*4),bw=48+(i*3),jk=460+(i*18),lv=65+(i*4),pk=(i%4)+1,ay=(i%3)+2;hyena_market(lc,bl,bw,jk,lv,pk,ay);}
ps("\n");hyena_report();hyena_state();ps("\n=== Demo Complete ===\n");return 0;}
