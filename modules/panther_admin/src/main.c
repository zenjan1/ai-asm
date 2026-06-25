/* panther_admin: Panther (Panthera pardus) black panther big cat (v1.0)
 * Panther jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, roar_vol, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,rr_vl,st_idx,age_yr,active;} pnth_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rr,t_st;} pnth_state_t;
static pnth_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static pnth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnth_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int rv,int si,int ay){if(*cnt>=mx)return -1;pnth_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->rr_vl=rv;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PNTH] Panther ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" rv=");pi(rv);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pnth_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rr=0;st.t_st=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PNTH] Panther initialized\n");return 0;}
int pnth_jungle(int lc,int bl,int bw,int tl,int rv,int si,int ay){return add(apl,&st.n_jungle,&st.t_ln,N,lc,bl,bw,tl,rv,si,ay);}
int pnth_feeding(int lc,int bl,int bw,int tl,int rv,int si,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,rv,si,ay);}
int pnth_breeding(int lc,int bl,int bw,int tl,int rv,int si,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,rv,si,ay);}
int pnth_health(int lc,int bl,int bw,int tl,int rv,int si,int ay){return add(ahd,&st.n_health,&st.t_rr,N-6,lc,bl,bw,tl,rv,si,ay);}
int pnth_market(int lc,int bl,int bw,int tl,int rv,int si,int ay){return add(amd,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,tl,rv,si,ay);}
void pnth_report(void){ps("[PNTH] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rr=");pi(st.t_rr);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void pnth_state(void){ps("[PNTH] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Panther Admin Demo ===\n\n");pnth_init();
ps("Panther jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=140+(i*10),bw=50+(i*8),tl=80+(i*5),rv=60+(i*5),si=(i%7)+1,ay=2+(i%12);pnth_jungle(lc,bl,bw,tl,rv,si,ay);}
ps("\nPanther feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=145+(i*10),bw=55+(i*8),tl=82+(i*5),rv=62+(i*5),si=(i%6)+2,ay=3+(i%10);pnth_feeding(lc,bl,bw,tl,rv,si,ay);}
ps("\nPanther breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=150+(i*10),bw=60+(i*8),tl=85+(i*5),rv=65+(i*5),si=(i%5)+1,ay=4+(i%8);pnth_breeding(lc,bl,bw,tl,rv,si,ay);}
ps("\nPanther health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=135+(i*12),bw=45+(i*10),tl=78+(i*6),rv=58+(i*6),si=(i%4)+3,ay=5+(i%6);pnth_health(lc,bl,bw,tl,rv,si,ay);}
ps("\nPanther market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=160+(i*10),bw=70+(i*8),tl=90+(i*5),rv=70+(i*5),si=(i%3)+4,ay=6+(i%5);pnth_market(lc,bl,bw,tl,rv,si,ay);}
ps("\n");pnth_report();pnth_state();ps("\n=== Demo Complete ===\n");return 0;}
