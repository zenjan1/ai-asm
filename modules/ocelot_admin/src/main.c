/* ocelot_admin: Ocelot (Leopardus pardalis) ocelot spotted wildcat (v1.0)
 * Ocelot den, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, prowl_vol, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,pr_vl,sp_idx,age_yr,active;} oclo_t;
typedef struct{int n_den,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_pr,t_sp;} oclo_state_t;
static oclo_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static oclo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oclo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int pv,int si,int ay){if(*cnt>=mx)return -1;oclo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->pr_vl=pv;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OCLO] Ocelot ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" pv=");pi(pv);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int oclo_init(void){if(init)return -1;st.n_den=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_pr=0;st.t_sp=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[OCLO] Ocelot initialized\n");return 0;}
int oclo_den(int lc,int bl,int bw,int tl,int pv,int si,int ay){return add(apl,&st.n_den,&st.t_ln,N,lc,bl,bw,tl,pv,si,ay);}
int oclo_feeding(int lc,int bl,int bw,int tl,int pv,int si,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,pv,si,ay);}
int oclo_breeding(int lc,int bl,int bw,int tl,int pv,int si,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,pv,si,ay);}
int oclo_health(int lc,int bl,int bw,int tl,int pv,int si,int ay){return add(ahd,&st.n_health,&st.t_pr,N-6,lc,bl,bw,tl,pv,si,ay);}
int oclo_market(int lc,int bl,int bw,int tl,int pv,int si,int ay){return add(amd,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,tl,pv,si,ay);}
void oclo_report(void){ps("[OCLO] Den: ");pi(st.n_den);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Pr=");pi(st.t_pr);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void oclo_state(void){ps("[OCLO] Den=");pi(st.n_den);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ocelot Admin Demo ===\n\n");oclo_init();
ps("Ocelot den...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*3),bw=8+(i*2),tl=30+(i*2),pv=20+(i*5),si=(i%7)+1,ay=1+(i%12);oclo_den(lc,bl,bw,tl,pv,si,ay);}
ps("\nOcelot feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*3),bw=9+(i*2),tl=31+(i*2),pv=22+(i*5),si=(i%6)+2,ay=2+(i%10);oclo_feeding(lc,bl,bw,tl,pv,si,ay);}
ps("\nOcelot breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*3),bw=10+(i*2),tl=33+(i*2),pv=25+(i*5),si=(i%5)+1,ay=3+(i%8);oclo_breeding(lc,bl,bw,tl,pv,si,ay);}
ps("\nOcelot health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=53+(i*4),bw=7+(i*3),tl=28+(i*3),pv=18+(i*6),si=(i%4)+3,ay=4+(i%6);oclo_health(lc,bl,bw,tl,pv,si,ay);}
ps("\nOcelot market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=65+(i*3),bw=12+(i*2),tl=35+(i*2),pv=30+(i*5),si=(i%3)+4,ay=5+(i%5);oclo_market(lc,bl,bw,tl,pv,si,ay);}
ps("\n");oclo_report();oclo_state();ps("\n=== Demo Complete ===\n");return 0;}
