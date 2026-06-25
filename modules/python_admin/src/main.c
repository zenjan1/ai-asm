/* python_admin: Python (Python regius) ball python large constrictor (v1.0)
 * Python terrarium, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, coil_str, strike_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,coil_st,sk_sp,sc_idx,age_yr,active;} pyth_t;
typedef struct{int n_terr,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_coil,t_sk,t_sc;} pyth_state_t;
static pyth_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static pyth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pyth_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cs,int ss,int si,int ay){if(*cnt>=mx)return -1;pyth_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->coil_st=cs;x->sk_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PYTH] Python ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cs=");pi(cs);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pyth_init(void){if(init)return -1;st.n_terr=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_coil=0;st.t_sk=0;st.t_sc=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PYTH] Python initialized\n");return 0;}
int pyth_terr(int lc,int bl,int bw,int cs,int ss,int si,int ay){return add(apl,&st.n_terr,&st.t_ln,N,lc,bl,bw,cs,ss,si,ay);}
int pyth_feeding(int lc,int bl,int bw,int cs,int ss,int si,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cs,ss,si,ay);}
int pyth_breeding(int lc,int bl,int bw,int cs,int ss,int si,int ay){return add(abd,&st.n_breed,&st.t_coil,N-4,lc,bl,bw,cs,ss,si,ay);}
int pyth_health(int lc,int bl,int bw,int cs,int ss,int si,int ay){return add(ahd,&st.n_health,&st.t_sk,N-6,lc,bl,bw,cs,ss,si,ay);}
int pyth_market(int lc,int bl,int bw,int cs,int ss,int si,int ay){return add(amd,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,cs,ss,si,ay);}
void pyth_report(void){ps("[PYTH] Terrarium: ");pi(st.n_terr);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Coil=");pi(st.t_coil);ps("\nHealth: ");pi(st.n_health);ps(" Sk=");pi(st.t_sk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void pyth_state(void){ps("[PYTH] Terr=");pi(st.n_terr);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Python Admin Demo ===\n\n");pyth_init();
ps("Python terrarium...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=2+(i*1),cs=30+(i*5),ss=50+(i*8),si=(i%6)+1,ay=1+(i%15);pyth_terr(lc,bl,bw,cs,ss,si,ay);}
ps("\nPython feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*8),bw=2+(i*1),cs=32+(i*5),ss=52+(i*8),si=(i%5)+2,ay=2+(i%12);pyth_feeding(lc,bl,bw,cs,ss,si,ay);}
ps("\nPython breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*8),bw=3+(i*1),cs=35+(i*5),ss=55+(i*8),si=(i%4)+1,ay=3+(i%10);pyth_breeding(lc,bl,bw,cs,ss,si,ay);}
ps("\nPython health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=95+(i*10),bw=2+(i*1),cs=28+(i*6),ss=48+(i*10),si=(i%3)+3,ay=4+(i%8);pyth_health(lc,bl,bw,cs,ss,si,ay);}
ps("\nPython market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=120+(i*8),bw=3+(i*1),cs=40+(i*5),ss=60+(i*8),si=(i%6)+1,ay=5+(i%6);pyth_market(lc,bl,bw,cs,ss,si,ay);}
ps("\n");pyth_report();pyth_state();ps("\n=== Demo Complete ===\n");return 0;}
