/* ostrich_admin: Ostrich (Struthio camelus) common ostrich flightless bird (v1.0)
 * Ostrich savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, run_speed, egg_wt, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,run_sp,eg_wt,age_yr,active;} ostr_t;
typedef struct{int n_savanna,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_run,t_eg;} ostr_state_t;
static ostr_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static ostr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ostr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lg,int rs,int ew,int ay){if(*cnt>=mx)return -1;ostr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lg;x->run_sp=rs;x->eg_wt=ew;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OSTR] Ostrich ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lg=");pi(lg);ps(" rs=");pi(rs);ps(" ew=");pi(ew);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ostr_init(void){if(init)return -1;st.n_savanna=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_run=0;st.t_eg=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[OSTR] Ostrich initialized\n");return 0;}
int ostr_savanna(int lc,int bl,int bw,int lg,int rs,int ew,int ay){return add(apl,&st.n_savanna,&st.t_ln,N,lc,bl,bw,lg,rs,ew,ay);}
int ostr_feeding(int lc,int bl,int bw,int lg,int rs,int ew,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lg,rs,ew,ay);}
int ostr_breeding(int lc,int bl,int bw,int lg,int rs,int ew,int ay){return add(abd,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lg,rs,ew,ay);}
int ostr_health(int lc,int bl,int bw,int lg,int rs,int ew,int ay){return add(ahd,&st.n_health,&st.t_run,N-6,lc,bl,bw,lg,rs,ew,ay);}
int ostr_market(int lc,int bl,int bw,int lg,int rs,int ew,int ay){return add(amd,&st.n_mkt,&st.t_eg,N-6,lc,bl,bw,lg,rs,ew,ay);}
void ostr_report(void){ps("[OSTR] Savanna: ");pi(st.n_savanna);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Eg=");pi(st.t_eg);ps("\n");}
void ostr_state(void){ps("[OSTR] Savanna=");pi(st.n_savanna);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ostrich Admin Demo ===\n\n");ostr_init();
ps("Ostrich savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*15),bw=100+(i*10),lg=120+(i*5),rs=50+(i*3),ew=150+(i*10),ay=2+(i%15);ostr_savanna(lc,bl,bw,lg,rs,ew,ay);}
ps("\nOstrich feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*15),bw=105+(i*10),lg=122+(i*5),rs=52+(i*3),ew=155+(i*10),ay=3+(i%12);ostr_feeding(lc,bl,bw,lg,rs,ew,ay);}
ps("\nOstrich breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*15),bw=110+(i*10),lg=125+(i*5),rs=55+(i*3),ew=160+(i*10),ay=4+(i%10);ostr_breeding(lc,bl,bw,lg,rs,ew,ay);}
ps("\nOstrich health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=190+(i*20),bw=95+(i*12),lg=118+(i*6),rs=48+(i*4),ew=145+(i*12),ay=5+(i%8);ostr_health(lc,bl,bw,lg,rs,ew,ay);}
ps("\nOstrich market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=240+(i*15),bw=120+(i*10),lg=130+(i*5),rs=60+(i*3),ew=170+(i*10),ay=6+(i%6);ostr_market(lc,bl,bw,lg,rs,ew,ay);}
ps("\n");ostr_report();ostr_state();ps("\n=== Demo Complete ===\n");return 0;}
