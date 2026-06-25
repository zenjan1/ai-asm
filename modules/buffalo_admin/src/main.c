/* buffalo_admin: Buffalo (Syncerus caffer) African Cape large bovine (v1.0)
 * Buffalo savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,wk_sp,hd_idx,age_yr,active;} bffl_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_wk,t_hd;} bffl_state_t;
static bffl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bffl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bffl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int hi,int ay){if(*cnt>=mx)return -1;bffl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->wk_sp=ws;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BFFL] Buffalo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int buffalo_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_wk=0;st.t_hd=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BFFL] Buffalo initialized\n");return 0;}
int buffalo_savanna(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(svl,&st.n_savan,&st.t_ln,N,lc,bl,bw,hc,ws,hi,ay);}
int buffalo_feeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,hi,ay);}
int buffalo_breeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(svb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ws,hi,ay);}
int buffalo_health(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(svh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,hi,ay);}
int buffalo_market(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(svm,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,hc,ws,hi,ay);}
void buffalo_report(void){ps("[BFFL] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void buffalo_state(void){ps("[BFFL] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Buffalo Admin Demo ===\n\n");buffalo_init();
ps("Buffalo savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=220+(i*8),bw=500+(i*40),hc=80+(i*8),ws=3+(i*2),hi=(i%8)+1,ay=(i%15)+1;buffalo_savanna(lc,bl,bw,hc,ws,hi,ay);}
ps("\nBuffalo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=230+(i*6),bw=520+(i*30),hc=85+(i*6),ws=4+(i*1),hi=(i%6)+1,ay=(i%12)+1;buffalo_feeding(lc,bl,bw,hc,ws,hi,ay);}
ps("\nBuffalo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=240+(i*5),bw=550+(i*25),hc=90+(i*5),ws=2+(i*3),hi=(i%5)+1,ay=(i%10)+1;buffalo_breeding(lc,bl,bw,hc,ws,hi,ay);}
ps("\nBuffalo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=215+(i*10),bw=480+(i*50),hc=78+(i*10),ws=2+(i*4),hi=(i%10)+1,ay=(i%8)+1;buffalo_health(lc,bl,bw,hc,ws,hi,ay);}
ps("\nBuffalo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=250+(i*4),bw=580+(i*20),hc=95+(i*4),ws=5+(i*2),hi=(i%4)+1,ay=(i%7)+1;buffalo_market(lc,bl,bw,hc,ws,hi,ay);}
ps("\n");buffalo_report();buffalo_state();ps("\n=== Demo Complete ===\n");return 0;}
