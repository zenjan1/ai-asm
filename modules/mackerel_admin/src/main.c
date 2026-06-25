/* mackerel_admin: Mackerel (Scomber scombrus) Atlantic mackerel pelagic fish (v1.0)
 * Mackerel ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_span_cm, school_idx, swim_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_sp,sch_idx,sw_sp,age_yr,active;} mack_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sch,t_sw;} mack_state_t;
static mack_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mack_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mack_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fs,int si,int ss,int ay){if(*cnt>=mx)return -1;mack_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_sp=fs;x->sch_idx=si;x->sw_sp=ss;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MACK] Mackerel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fs=");pi(fs);ps(" si=");pi(si);ps(" ss=");pi(ss);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mack_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sch=0;st.t_sw=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MACK] Mackerel initialized\n");return 0;}
int mack_ocean(int lc,int bl,int bw,int fs,int si,int ss,int ay){return add(apl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,fs,si,ss,ay);}
int mack_feeding(int lc,int bl,int bw,int fs,int si,int ss,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fs,si,ss,ay);}
int mack_breeding(int lc,int bl,int bw,int fs,int si,int ss,int ay){return add(abd,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fs,si,ss,ay);}
int mack_health(int lc,int bl,int bw,int fs,int si,int ss,int ay){return add(ahd,&st.n_health,&st.t_sch,N-6,lc,bl,bw,fs,si,ss,ay);}
int mack_market(int lc,int bl,int bw,int fs,int si,int ss,int ay){return add(amd,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,fs,si,ss,ay);}
void mack_report(void){ps("[MACK] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sch=");pi(st.t_sch);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void mack_state(void){ps("[MACK] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mackerel Admin Demo ===\n\n");mack_init();
ps("Mackerel ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=200+(i*30),fs=8+(i*2),si=50+(i*10),ss=10+(i*3),ay=1+(i%8);mack_ocean(lc,bl,bw,fs,si,ss,ay);}
ps("\nMackerel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*2),bw=220+(i*30),fs=9+(i*2),si=55+(i*10),ss=11+(i*3),ay=2+(i%6);mack_feeding(lc,bl,bw,fs,si,ss,ay);}
ps("\nMackerel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=30+(i*2),bw=250+(i*30),fs=10+(i*2),si=60+(i*10),ss=12+(i*3),ay=3+(i%5);mack_breeding(lc,bl,bw,fs,si,ss,ay);}
ps("\nMackerel health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=180+(i*40),fs=7+(i*3),si=45+(i*12),ss=9+(i*4),ay=4+(i%4);mack_health(lc,bl,bw,fs,si,ss,ay);}
ps("\nMackerel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=35+(i*2),bw=300+(i*30),fs=12+(i*2),si=70+(i*10),ss=14+(i*3),ay=5+(i%3);mack_market(lc,bl,bw,fs,si,ss,ay);}
ps("\n");mack_report();mack_state();ps("\n=== Demo Complete ===\n");return 0;}
