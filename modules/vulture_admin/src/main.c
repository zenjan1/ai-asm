/* vulture_admin: Vulture (Gyps fulvus) scavenger bird (v1.0)
 * Vulture savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_cm, soar_speed, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,sr_sp,pl_idx,age_yr,active;} vltr_t;
typedef struct{int n_savn,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sr,t_pl;} vltr_state_t;
static vltr_t vpl[N],vfd[N-2],vbr[N-4],vhl[N-6],vmk[N-6]; static vltr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vltr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int ss,int pi2,int ay){if(*cnt>=mx)return -1;vltr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->sr_sp=ss;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[VLTR] Vulture ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" ss=");pi(ss);ps(" pl=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int vltr_init(void){if(init)return -1;st.n_savn=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sr=0;st.t_pl=0;for(int i=0;i<N;i++)vpl[i].active=0;for(int i=0;i<N-2;i++)vfd[i].active=0;for(int i=0;i<N-4;i++)vbr[i].active=0;for(int i=0;i<N-6;i++)vhl[i].active=0;for(int i=0;i<N-6;i++)vmk[i].active=0;init=1;ps("[VLTR] Vulture initialized\n");return 0;}
int vltr_savanna(int lc,int bl,int bw,int wc,int ss,int pi2,int ay){return add(vpl,&st.n_savn,&st.t_ln,N,lc,bl,bw,wc,ss,pi2,ay);}
int vltr_feeding(int lc,int bl,int bw,int wc,int ss,int pi2,int ay){return add(vfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,ss,pi2,ay);}
int vltr_breeding(int lc,int bl,int bw,int wc,int ss,int pi2,int ay){return add(vbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,ss,pi2,ay);}
int vltr_health(int lc,int bl,int bw,int wc,int ss,int pi2,int ay){return add(vhl,&st.n_health,&st.t_sr,N-6,lc,bl,bw,wc,ss,pi2,ay);}
int vltr_market(int lc,int bl,int bw,int wc,int ss,int pi2,int ay){return add(vmk,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,wc,ss,pi2,ay);}
void vltr_report(void){ps("[VLTR] Savanna: ");pi(st.n_savn);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Sr=");pi(st.t_sr);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void vltr_state(void){ps("[VLTR] Savn=");pi(st.n_savn);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Vulture Admin Demo ===\n\n");vltr_init();
ps("Vulture savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*8),bw=6+(i*2),wc=200+(i*12),ss=30+(i*5),pl=(i%6)+1,ay=(i%5)+1;vltr_savanna(lc,bl,bw,wc,ss,pl,ay);}
ps("\nVulture feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=100+(i*6),bw=8+(i),wc=220+(i*10),ss=35+(i*4),pl=(i%5)+1,ay=(i%4)+1;vltr_feeding(lc,bl,bw,wc,ss,pl,ay);}
ps("\nVulture breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=80+(i*10),bw=5+(i*3),wc=180+(i*15),ss=25+(i*6),pl=(i%4)+1,ay=(i%3)+1;vltr_breeding(lc,bl,bw,wc,ss,pl,ay);}
ps("\nVulture health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=110+(i*5),bw=10+(i),wc=240+(i*8),ss=40+(i*3),pl=(i%7)+1,ay=(i%6)+2;vltr_health(lc,bl,bw,wc,ss,pl,ay);}
ps("\nVulture market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=120+(i*4),bw=12+(i%3),wc=260+(i*6),ss=20+(i*5),pl=(i%3)+3,ay=(i%4)+3;vltr_market(lc,bl,bw,wc,ss,pl,ay);}
ps("\n");vltr_report();vltr_state();ps("\n=== Demo Complete ===\n");return 0;}
