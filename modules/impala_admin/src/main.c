/* impala_admin: Impala (Aepyceros melampus) graceful antelope (v1.0)
 * Impala savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, jump_ht, coat_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,jp_ht,ct_idx,age_yr,active;} imp_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_jp,t_ct;} imp_state_t;
static imp_t svn[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static imp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(imp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int jh,int ci,int ay){if(*cnt>=mx)return -1;imp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->jp_ht=jh;x->ct_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IMP] Impala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" jh=");pi(jh);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int impala_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_jp=0;st.t_ct=0;for(int i=0;i<N;i++)svn[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[IMP] Impala initialized\n");return 0;}
int impala_savanna(int lc,int bl,int bw,int hc,int jh,int ci,int ay){return add(svn,&st.n_savan,&st.t_ln,N,lc,bl,bw,hc,jh,ci,ay);}
int impala_feeding(int lc,int bl,int bw,int hc,int jh,int ci,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,jh,ci,ay);}
int impala_breeding(int lc,int bl,int bw,int hc,int jh,int ci,int ay){return add(svb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,jh,ci,ay);}
int impala_health(int lc,int bl,int bw,int hc,int jh,int ci,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,hc,jh,ci,ay);}
int impala_market(int lc,int bl,int bw,int hc,int jh,int ci,int ay){return add(svm,&st.n_mkt,&st.t_ct,N-6,lc,bl,bw,hc,jh,ci,ay);}
void impala_report(void){ps("[IMP] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Ct=");pi(st.t_ct);ps("\n");}
void impala_state(void){ps("[IMP] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Impala Admin Demo ===\n\n");impala_init();
ps("Impala savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*6),bw=50+(i*4),hc=40+(i*4),jh=200+(i*20),ci=(i%8)+1,ay=(i%12)+1;impala_savanna(lc,bl,bw,hc,jh,ci,ay);}
ps("\nImpala feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=123+(i*5),bw=52+(i*3),hc=42+(i*3),jh=210+(i*15),ci=(i%7)+1,ay=(i%10)+1;impala_feeding(lc,bl,bw,hc,jh,ci,ay);}
ps("\nImpala breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=126+(i*4),bw=54+(i*3),hc=44+(i*2),jh=190+(i*25),ci=(i%6)+1,ay=(i%8)+1;impala_breeding(lc,bl,bw,hc,jh,ci,ay);}
ps("\nImpala health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*7),bw=48+(i*5),hc=38+(i*5),jh=180+(i*30),ci=(i%9)+1,ay=(i%7)+1;impala_health(lc,bl,bw,hc,jh,ci,ay);}
ps("\nImpala market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=128+(i*4),bw=56+(i*3),hc=46+(i*2),jh=220+(i*12),ci=(i%5)+1,ay=(i%6)+1;impala_market(lc,bl,bw,hc,jh,ci,ay);}
ps("\n");impala_report();impala_state();ps("\n=== Demo Complete ===\n");return 0;}
