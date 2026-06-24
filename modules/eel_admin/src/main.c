/* eel_admin: Eel (Anguilla anguilla) freshwater eel aquaculture (v1.0)
 * Eel housing, feeding, breeding, harvest, market
 * Features: body_len_cm, body_wt_kg, slime_idx, fin_ht_cm, swim_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,slm_idx,fin_ht,swm_spd,age_yr,active;} eel_t;
typedef struct{int n_house,n_feed,n_breed,n_harv,n_mkt,t_ln,t_wt,t_sl,t_fn,t_sw;} eel_state_t;
static eel_t ehs[N],efd[N-2],ebr[N-4],ehv[N-6],emk[N-6]; static eel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eel_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int si,int fh,int ss,int ay){if(*cnt>=mx)return -1;eel_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->slm_idx=si;x->fin_ht=fh;x->swm_spd=ss;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EEL] Eel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" si=");pi(si);ps(" fh=");pi(fh);ps(" ss=");pi(ss);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int eel_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sl=0;st.t_fn=0;st.t_sw=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)ehv[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EEL] Eel initialized\n");return 0;}
int eel_housing(int lc,int bl,int bw,int si,int fh,int ss,int ay){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,si,fh,ss,ay);}
int eel_feeding(int lc,int bl,int bw,int si,int fh,int ss,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,si,fh,ss,ay);}
int eel_breeding(int lc,int bl,int bw,int si,int fh,int ss,int ay){return add(ebr,&st.n_breed,&st.t_sl,N-4,lc,bl,bw,si,fh,ss,ay);}
int eel_harvest(int lc,int bl,int bw,int si,int fh,int ss,int ay){return add(ehv,&st.n_harv,&st.t_fn,N-6,lc,bl,bw,si,fh,ss,ay);}
int eel_market(int lc,int bl,int bw,int si,int fh,int ss,int ay){return add(emk,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,si,fh,ss,ay);}
void eel_report(void){ps("[EEL] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sl=");pi(st.t_sl);ps("\nHarv: ");pi(st.n_harv);ps(" Fn=");pi(st.t_fn);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void eel_state(void){ps("[EEL] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Eel Admin Demo ===\n\n");eel_init();
/* 1=pond 2=tank 3=river 4=farm 5=market */
ps("Eel housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*5),bw=1+(i%3),si=(i%6)+1,fh=5+(i*2),ss=15+(i*3),ay=1+(i%8);eel_housing(lc,bl,bw,si,fh,ss,ay);}
ps("\nEel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=65+(i*4),bw=2+(i%2),si=(i%5)+1,fh=6+(i%2),ss=18+(i*2),ay=2+(i%6);eel_feeding(lc,bl,bw,si,fh,ss,ay);}
ps("\nEel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=55+(i*6),bw=1+(i%3),si=(i%4)+1,fh=4+(i%3),ss=12+(i*4),ay=2+(i%5);eel_breeding(lc,bl,bw,si,fh,ss,ay);}
ps("\nEel harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=70+(i*4),bw=2+(i%2),si=(i%7)+1,fh=7+(i%2),ss=20+(i*3),ay=3+(i%7);eel_harvest(lc,bl,bw,si,fh,ss,ay);}
ps("\nEel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=75+(i*3),bw=3+(i%2),si=(i%3)+4,fh=8+(i%2),ss=22+(i*2),ay=3+(i%4);eel_market(lc,bl,bw,si,fh,ss,ay);}
ps("\n");eel_report();eel_state();ps("\n=== Demo Complete ===\n");return 0;}
