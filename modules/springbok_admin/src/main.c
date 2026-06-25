/* springbok_admin: Springbok (Antidorcas marsupialis) leaping antelope (v1.0)
 * Springbok veld, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, jump_cm, sp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,jp_cm,sp_idx,age_yr,active;} spbk_t;
typedef struct{int n_veld,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_jp,t_sp;} spbk_state_t;
static spbk_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static spbk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spbk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int jc,int si,int ay){if(*cnt>=mx)return -1;spbk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->jp_cm=jc;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SPBK] Springbok ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" jc=");pi(jc);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int springbok_init(void){if(init)return -1;st.n_veld=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_jp=0;st.t_sp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SPBK] Springbok initialized\n");return 0;}
int springbok_veld(int lc,int bl,int bw,int hc,int jc,int si,int ay){return add(svl,&st.n_veld,&st.t_ln,N,lc,bl,bw,hc,jc,si,ay);}
int springbok_feeding(int lc,int bl,int bw,int hc,int jc,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,jc,si,ay);}
int springbok_breeding(int lc,int bl,int bw,int hc,int jc,int si,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,jc,si,ay);}
int springbok_health(int lc,int bl,int bw,int hc,int jc,int si,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,hc,jc,si,ay);}
int springbok_market(int lc,int bl,int bw,int hc,int jc,int si,int ay){return add(svm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,hc,jc,si,ay);}
void springbok_report(void){ps("[SPBK] Veld: ");pi(st.n_veld);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void springbok_state(void){ps("[SPBK] Veld=");pi(st.n_veld);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Springbok Admin Demo ===\n\n");springbok_init();
ps("Springbok veld...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=110+(i*6),bw=35+(i*3),hc=25+(i*2),jc=150+(i*10),si=(i%8)+1,ay=(i%12)+1;springbok_veld(lc,bl,bw,hc,jc,si,ay);}
ps("\nSpringbok feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=115+(i*5),bw=37+(i*2),hc=26+(i*2),jc=155+(i*8),si=(i%6)+1,ay=(i%10)+1;springbok_feeding(lc,bl,bw,hc,jc,si,ay);}
ps("\nSpringbok breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=120+(i*4),bw=40+(i*2),hc=28+(i*2),jc=160+(i*6),si=(i%5)+1,ay=(i%8)+1;springbok_breeding(lc,bl,bw,hc,jc,si,ay);}
ps("\nSpringbok health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=108+(i*8),bw=33+(i*4),hc=24+(i*3),jc=145+(i*12),si=(i%10)+1,ay=(i%6)+1;springbok_health(lc,bl,bw,hc,jc,si,ay);}
ps("\nSpringbok market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=125+(i*3),bw=42+(i*2),hc=30+(i*1),jc=165+(i*5),si=(i%4)+1,ay=(i%5)+1;springbok_market(lc,bl,bw,hc,jc,si,ay);}
ps("\n");springbok_report();springbok_state();ps("\n=== Demo Complete ===\n");return 0;}
