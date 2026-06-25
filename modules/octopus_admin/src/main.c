/* octopus_admin: Octopus (Octopus vulgaris) common octopus cephalopod (v1.0)
 * Octopus reef, feeding, breeding, health, market
 * Features: arm_span_cm, body_wt_kg, tentacle_ct, ink_vol, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,arm_sp,bdy_wt,tn_ct,ink_vl,cl_idx,age_yr,active;} octp_t;
typedef struct{int n_reef,n_feed,n_breed,n_health,n_mkt,t_arm,t_wt,t_tn,t_ink,t_cl;} octp_state_t;
static octp_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static octp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(octp_t*a,int*cnt,int*sum,int mx,int lc,int ar,int bw,int tc,int iv,int ci,int ay){if(*cnt>=mx)return -1;octp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->arm_sp=ar;x->bdy_wt=bw;x->tn_ct=tc;x->ink_vl=iv;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=ar;(*cnt)++;ps("[OCTP] Octopus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ar=");pi(ar);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" iv=");pi(iv);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int octp_init(void){if(init)return -1;st.n_reef=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_arm=0;st.t_wt=0;st.t_tn=0;st.t_ink=0;st.t_cl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[OCTP] Octopus initialized\n");return 0;}
int octp_reef(int lc,int ar,int bw,int tc,int iv,int ci,int ay){return add(apl,&st.n_reef,&st.t_arm,N,lc,ar,bw,tc,iv,ci,ay);}
int octp_feeding(int lc,int ar,int bw,int tc,int iv,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,ar,bw,tc,iv,ci,ay);}
int octp_breeding(int lc,int ar,int bw,int tc,int iv,int ci,int ay){return add(abd,&st.n_breed,&st.t_tn,N-4,lc,ar,bw,tc,iv,ci,ay);}
int octp_health(int lc,int ar,int bw,int tc,int iv,int ci,int ay){return add(ahd,&st.n_health,&st.t_ink,N-6,lc,ar,bw,tc,iv,ci,ay);}
int octp_market(int lc,int ar,int bw,int tc,int iv,int ci,int ay){return add(amd,&st.n_mkt,&st.t_cl,N-6,lc,ar,bw,tc,iv,ci,ay);}
void octp_report(void){ps("[OCTP] Reef: ");pi(st.n_reef);ps(" Arm=");pi(st.t_arm);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tn=");pi(st.t_tn);ps("\nHealth: ");pi(st.n_health);ps(" Ink=");pi(st.t_ink);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void octp_state(void){ps("[OCTP] Reef=");pi(st.n_reef);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Octopus Admin Demo ===\n\n");octp_init();
ps("Octopus reef...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ar=80+(i*8),bw=3+(i*2),tc=8,iv=50+(i*10),ci=(i%6)+1,ay=1+(i%4);octp_reef(lc,ar,bw,tc,iv,ci,ay);}
ps("\nOctopus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ar=85+(i*8),bw=4+(i*2),tc=8,iv=55+(i*10),ci=(i%5)+2,ay=1+(i%3);octp_feeding(lc,ar,bw,tc,iv,ci,ay);}
ps("\nOctopus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ar=90+(i*8),bw=4+(i*2),tc=8,iv=60+(i*10),ci=(i%4)+1,ay=2+(i%3);octp_breeding(lc,ar,bw,tc,iv,ci,ay);}
ps("\nOctopus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ar=75+(i*10),bw=3+(i*3),tc=8,iv=45+(i*12),ci=(i%3)+3,ay=2+(i%2);octp_health(lc,ar,bw,tc,iv,ci,ay);}
ps("\nOctopus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ar=100+(i*8),bw=5+(i*2),tc=8,iv=70+(i*10),ci=(i%6)+1,ay=3+(i%2);octp_market(lc,ar,bw,tc,iv,ci,ay);}
ps("\n");octp_report();octp_state();ps("\n=== Demo Complete ===\n");return 0;}
