/* hippo_admin: Hippo (Hippopotamus amphibius) large semi-aquatic mammal (v1.0)
 * Hippo river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, swim_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,sw_sp,hd_idx,age_yr,active;} hippo_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_sw,t_hd;} hippo_state_t;
static hippo_t rvr[N],rvf[N-2],rvb[N-4],rvh[N-6],rvm[N-6]; static hippo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hippo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int hi,int ay){if(*cnt>=mx)return -1;hippo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->sw_sp=ss;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HIPPO] Hippo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hippo_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_sw=0;st.t_hd=0;for(int i=0;i<N;i++)rvr[i].active=0;for(int i=0;i<N-2;i++)rvf[i].active=0;for(int i=0;i<N-4;i++)rvb[i].active=0;for(int i=0;i<N-6;i++)rvh[i].active=0;for(int i=0;i<N-6;i++)rvm[i].active=0;init=1;ps("[HIPPO] Hippo initialized\n");return 0;}
int hippo_river(int lc,int bl,int bw,int tc,int ss,int hi,int ay){return add(rvr,&st.n_river,&st.t_ln,N,lc,bl,bw,tc,ss,hi,ay);}
int hippo_feeding(int lc,int bl,int bw,int tc,int ss,int hi,int ay){return add(rvf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,hi,ay);}
int hippo_breeding(int lc,int bl,int bw,int tc,int ss,int hi,int ay){return add(rvb,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,ss,hi,ay);}
int hippo_health(int lc,int bl,int bw,int tc,int ss,int hi,int ay){return add(rvh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,hi,ay);}
int hippo_market(int lc,int bl,int bw,int tc,int ss,int hi,int ay){return add(rvm,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,tc,ss,hi,ay);}
void hippo_report(void){ps("[HIPPO] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void hippo_state(void){ps("[HIPPO] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hippo Admin Demo ===\n\n");hippo_init();
ps("Hippo river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=350+(i*12),bw=1500+(i*120),tc=40+(i*5),ss=5+(i*2),hi=(i%8)+1,ay=(i%30)+1;hippo_river(lc,bl,bw,tc,ss,hi,ay);}
ps("\nHippo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=355+(i*10),bw=1550+(i*100),tc=42+(i*4),ss=6+(i*2),hi=(i%7)+1,ay=(i%25)+1;hippo_feeding(lc,bl,bw,tc,ss,hi,ay);}
ps("\nHippo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=360+(i*8),bw=1600+(i*80),tc=44+(i*3),ss=4+(i*3),hi=(i%6)+1,ay=(i%20)+1;hippo_breeding(lc,bl,bw,tc,ss,hi,ay);}
ps("\nHippo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=345+(i*15),bw=1450+(i*150),tc=38+(i*6),ss=3+(i*4),hi=(i%9)+1,ay=(i%15)+1;hippo_health(lc,bl,bw,tc,ss,hi,ay);}
ps("\nHippo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=365+(i*7),bw=1650+(i*70),tc=46+(i*3),ss=7+(i*2),hi=(i%5)+1,ay=(i%12)+1;hippo_market(lc,bl,bw,tc,ss,hi,ay);}
ps("\n");hippo_report();hippo_state();ps("\n=== Demo Complete ===\n");return 0;}
