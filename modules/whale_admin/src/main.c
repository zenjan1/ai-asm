/* whale_admin: Whale (Balaenoptera musculus) blue whale (v1.0)
 * Whale pelagic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, blow_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,bl_idx,age_yr,active;} whl_t;
typedef struct{int n_pelag,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_bl;} whl_state_t;
static whl_t wpl[N],wfd[N-2],wbr[N-4],whl_a[N-6],wmk[N-6]; static whl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(whl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int bi,int ay){if(*cnt>=mx)return -1;whl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->bl_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WHL] Whale ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int whl_init(void){if(init)return -1;st.n_pelag=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_bl=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl_a[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WHL] Whale initialized\n");return 0;}
int whl_pelagic(int lc,int bl,int bw,int fc,int ss,int bi,int ay){return add(wpl,&st.n_pelag,&st.t_ln,N,lc,bl,bw,fc,ss,bi,ay);}
int whl_feeding(int lc,int bl,int bw,int fc,int ss,int bi,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,bi,ay);}
int whl_breeding(int lc,int bl,int bw,int fc,int ss,int bi,int ay){return add(wbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,bi,ay);}
int whl_health(int lc,int bl,int bw,int fc,int ss,int bi,int ay){return add(whl_a,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,bi,ay);}
int whl_market(int lc,int bl,int bw,int fc,int ss,int bi,int ay){return add(wmk,&st.n_mkt,&st.t_bl,N-6,lc,bl,bw,fc,ss,bi,ay);}
void whl_report(void){ps("[WHL] Pelagic: ");pi(st.n_pelag);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Bl=");pi(st.t_bl);ps("\n");}
void whl_state(void){ps("[WHL] Pelag=");pi(st.n_pelag);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Whale Admin Demo ===\n\n");whl_init();
ps("Whale pelagic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=2000+(i*200),bw=100000+(i*20000),fc=300+(i*30),ss=10+(i*3),bi=(i%6)+1,ay=(i%8)+5;whl_pelagic(lc,bl,bw,fc,ss,bi,ay);}
ps("\nWhale feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=2200+(i*150),bw=120000+(i*15000),fc=330+(i*25),ss=12+(i*2),bi=(i%5)+1,ay=(i%6)+4;whl_feeding(lc,bl,bw,fc,ss,bi,ay);}
ps("\nWhale breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=1800+(i*250),bw=80000+(i*25000),fc=270+(i*35),ss=8+(i*4),bi=(i%4)+1,ay=(i%5)+3;whl_breeding(lc,bl,bw,fc,ss,bi,ay);}
ps("\nWhale health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=2500+(i*100),bw=150000+(i*10000),fc=360+(i*20),ss=14+(i),bi=(i%7)+1,ay=(i%4)+6;whl_health(lc,bl,bw,fc,ss,bi,ay);}
ps("\nWhale market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=2800+(i*80),bw=180000+(i*8000),fc=390+(i*15),ss=6+(i*2),bi=(i%3)+3,ay=(i%3)+10;whl_market(lc,bl,bw,fc,ss,bi,ay);}
ps("\n");whl_report();whl_state();ps("\n=== Demo Complete ===\n");return 0;}
