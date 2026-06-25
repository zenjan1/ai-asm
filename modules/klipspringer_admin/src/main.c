/* klipspringer_admin: Klipspringer (Oreotragus oreotragus) rock antelope (v1.0)
 * Klipspringer rocky, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, climb_speed, kl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,cl_sp,kl_idx,age_yr,active;} klip_t;
typedef struct{int n_rocky,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_cl,t_kl;} klip_state_t;
static klip_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static klip_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(klip_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int cs,int ki,int ay){if(*cnt>=mx)return -1;klip_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->cl_sp=cs;x->kl_idx=ki;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KLIP] Klipspringer ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" cs=");pi(cs);ps(" ki=");pi(ki);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int klipspringer_init(void){if(init)return -1;st.n_rocky=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_cl=0;st.t_kl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KLIP] Klipspringer initialized\n");return 0;}
int klipspringer_rocky(int lc,int bl,int bw,int hc,int cs,int ki,int ay){return add(svl,&st.n_rocky,&st.t_ln,N,lc,bl,bw,hc,cs,ki,ay);}
int klipspringer_feeding(int lc,int bl,int bw,int hc,int cs,int ki,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,cs,ki,ay);}
int klipspringer_breeding(int lc,int bl,int bw,int hc,int cs,int ki,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,cs,ki,ay);}
int klipspringer_health(int lc,int bl,int bw,int hc,int cs,int ki,int ay){return add(svh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,hc,cs,ki,ay);}
int klipspringer_market(int lc,int bl,int bw,int hc,int cs,int ki,int ay){return add(svm,&st.n_mkt,&st.t_kl,N-6,lc,bl,bw,hc,cs,ki,ay);}
void klipspringer_report(void){ps("[KLIP] Rocky: ");pi(st.n_rocky);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Kl=");pi(st.t_kl);ps("\n");}
void klipspringer_state(void){ps("[KLIP] Rocky=");pi(st.n_rocky);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Klipspringer Admin Demo ===\n\n");klipspringer_init();
ps("Klipspringer rocky...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=85+(i*4),bw=12+(i*1),hc=18+(i*2),cs=8+(i*2),ki=(i%8)+1,ay=(i%12)+1;klipspringer_rocky(lc,bl,bw,hc,cs,ki,ay);}
ps("\nKlipspringer feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=87+(i*3),bw=13+(i*1),hc=19+(i*1),cs=9+(i*1),ki=(i%6)+1,ay=(i%10)+1;klipspringer_feeding(lc,bl,bw,hc,cs,ki,ay);}
ps("\nKlipspringer breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=89+(i*2),bw=14+(i*1),hc=20+(i*1),cs=7+(i*2),ki=(i%5)+1,ay=(i%8)+1;klipspringer_breeding(lc,bl,bw,hc,cs,ki,ay);}
ps("\nKlipspringer health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=83+(i*5),bw=11+(i*2),hc=17+(i*2),cs=6+(i*3),ki=(i%10)+1,ay=(i%7)+1;klipspringer_health(lc,bl,bw,hc,cs,ki,ay);}
ps("\nKlipspringer market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=91+(i*2),bw=15+(i*1),hc=21+(i*1),cs=10+(i*1),ki=(i%4)+1,ay=(i%6)+1;klipspringer_market(lc,bl,bw,hc,cs,ki,ay);}
ps("\n");klipspringer_report();klipspringer_state();ps("\n=== Demo Complete ===\n");return 0;}
