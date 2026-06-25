/* rhino_admin: Rhino (Rhinoceros unicornis) Indian rhinoceros large herbivore (v1.0)
 * Rhino savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, charge_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,ch_sp,hd_idx,age_yr,active;} rhno_t;
typedef struct{int n_savanna,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_ch,t_hd;} rhno_state_t;
static rhno_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static rhno_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rhno_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int cs,int hi,int ay){if(*cnt>=mx)return -1;rhno_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->ch_sp=cs;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RHNO] Rhino ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" cs=");pi(cs);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rhno_init(void){if(init)return -1;st.n_savanna=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_ch=0;st.t_hd=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[RHNO] Rhino initialized\n");return 0;}
int rhno_savanna(int lc,int bl,int bw,int hc,int cs,int hi,int ay){return add(apl,&st.n_savanna,&st.t_ln,N,lc,bl,bw,hc,cs,hi,ay);}
int rhno_feeding(int lc,int bl,int bw,int hc,int cs,int hi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,cs,hi,ay);}
int rhno_breeding(int lc,int bl,int bw,int hc,int cs,int hi,int ay){return add(abd,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,cs,hi,ay);}
int rhno_health(int lc,int bl,int bw,int hc,int cs,int hi,int ay){return add(ahd,&st.n_health,&st.t_ch,N-6,lc,bl,bw,hc,cs,hi,ay);}
int rhno_market(int lc,int bl,int bw,int hc,int cs,int hi,int ay){return add(amd,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,hc,cs,hi,ay);}
void rhno_report(void){ps("[RHNO] Savanna: ");pi(st.n_savanna);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Ch=");pi(st.t_ch);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void rhno_state(void){ps("[RHNO] Savanna=");pi(st.n_savanna);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rhino Admin Demo ===\n\n");rhno_init();
ps("Rhino savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=350+(i*20),bw=1500+(i*100),hc=50+(i*5),cs=30+(i*5),hi=(i%6)+1,ay=5+(i%25);rhno_savanna(lc,bl,bw,hc,cs,hi,ay);}
ps("\nRhino feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=360+(i*20),bw=1550+(i*100),hc=52+(i*5),cs=32+(i*5),hi=(i%5)+2,ay=6+(i%22);rhno_feeding(lc,bl,bw,hc,cs,hi,ay);}
ps("\nRhino breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=370+(i*20),bw=1600+(i*100),hc=55+(i*5),cs=35+(i*5),hi=(i%4)+1,ay=7+(i%20);rhno_breeding(lc,bl,bw,hc,cs,hi,ay);}
ps("\nRhino health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=340+(i*25),bw=1450+(i*120),hc=48+(i*6),cs=28+(i*6),hi=(i%3)+3,ay=8+(i%18);rhno_health(lc,bl,bw,hc,cs,hi,ay);}
ps("\nRhino market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=390+(i*20),bw=1700+(i*100),hc=60+(i*5),cs=40+(i*5),hi=(i%6)+1,ay=10+(i%15);rhno_market(lc,bl,bw,hc,cs,hi,ay);}
ps("\n");rhno_report();rhno_state();ps("\n=== Demo Complete ===\n");return 0;}
