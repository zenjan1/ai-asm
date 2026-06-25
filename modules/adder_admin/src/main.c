/* adder_admin: Adder (Vipera berus) venomous viper snake (v1.0)
 * Adder heathland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fang_cm, strike_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fang_cm,stk_sp,sc_idx,age_yr,active;} adder_t;
typedef struct{int n_heath,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fang,t_stk,t_sc;} adder_state_t;
static adder_t adl[N],adf[N-2],adb[N-4],adh[N-6],adm[N-6]; static adder_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(adder_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;adder_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fang_cm=fc;x->stk_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ADDER] Adder ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int adder_init(void){if(init)return -1;st.n_heath=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fang=0;st.t_stk=0;st.t_sc=0;for(int i=0;i<N;i++)adl[i].active=0;for(int i=0;i<N-2;i++)adf[i].active=0;for(int i=0;i<N-4;i++)adb[i].active=0;for(int i=0;i<N-6;i++)adh[i].active=0;for(int i=0;i<N-6;i++)adm[i].active=0;init=1;ps("[ADDER] Adder initialized\n");return 0;}
int adder_heathland(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(adl,&st.n_heath,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int adder_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(adf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int adder_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(adb,&st.n_breed,&st.t_fang,N-4,lc,bl,bw,fc,ss,si,ay);}
int adder_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(adh,&st.n_health,&st.t_stk,N-6,lc,bl,bw,fc,ss,si,ay);}
int adder_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(adm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void adder_report(void){ps("[ADDER] Heath: ");pi(st.n_heath);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fang=");pi(st.t_fang);ps("\nHealth: ");pi(st.n_health);ps(" Stk=");pi(st.t_stk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void adder_state(void){ps("[ADDER] Heath=");pi(st.n_heath);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Adder Admin Demo ===\n\n");adder_init();
ps("Adder heathland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=50+(i*10),fc=1+(i%3),ss=10+(i*5),si=(i%8)+1,ay=(i%15)+1;adder_heathland(lc,bl,bw,fc,ss,si,ay);}
ps("\nAdder feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=55+(i*10),fc=1+(i%4),ss=12+(i*4),si=(i%7)+1,ay=(i%12)+1;adder_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nAdder breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*4),bw=60+(i*8),fc=2+(i%3),ss=15+(i*3),si=(i%6)+1,ay=(i%10)+1;adder_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nAdder health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=44+(i*2),bw=70+(i*7),fc=1+(i%5),ss=8+(i*6),si=(i%9)+1,ay=(i%8)+1;adder_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nAdder market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*2),bw=80+(i*5),fc=2+(i%2),ss=20+(i*3),si=(i%5)+1,ay=(i%6)+1;adder_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");adder_report();adder_state();ps("\n=== Demo Complete ===\n");return 0;}
