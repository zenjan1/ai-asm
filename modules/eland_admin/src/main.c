/* eland_admin: Eland (Taurotragus oryx) largest antelope species (v1.0)
 * Eland savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, el_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,wk_sp,el_idx,age_yr,active;} elnd_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_wk,t_el;} elnd_state_t;
static elnd_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static elnd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elnd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int ei,int ay){if(*cnt>=mx)return -1;elnd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->wk_sp=ws;x->el_idx=ei;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ELND] Eland ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" ei=");pi(ei);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int eland_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_wk=0;st.t_el=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ELND] Eland initialized\n");return 0;}
int eland_savanna(int lc,int bl,int bw,int hc,int ws,int ei,int ay){return add(svl,&st.n_savan,&st.t_ln,N,lc,bl,bw,hc,ws,ei,ay);}
int eland_feeding(int lc,int bl,int bw,int hc,int ws,int ei,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,ei,ay);}
int eland_breeding(int lc,int bl,int bw,int hc,int ws,int ei,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ws,ei,ay);}
int eland_health(int lc,int bl,int bw,int hc,int ws,int ei,int ay){return add(svh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,ei,ay);}
int eland_market(int lc,int bl,int bw,int hc,int ws,int ei,int ay){return add(svm,&st.n_mkt,&st.t_el,N-6,lc,bl,bw,hc,ws,ei,ay);}
void eland_report(void){ps("[ELND] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" El=");pi(st.t_el);ps("\n");}
void eland_state(void){ps("[ELND] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Eland Admin Demo ===\n\n");eland_init();
ps("Eland savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*12),bw=400+(i*40),hc=100+(i*6),ws=4+(i*1),ei=(i%8)+1,ay=(i%18)+1;eland_savanna(lc,bl,bw,hc,ws,ei,ay);}
ps("\nEland feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=260+(i*10),bw=420+(i*30),hc=105+(i*5),ws=5+(i*1),ei=(i%6)+1,ay=(i%15)+1;eland_feeding(lc,bl,bw,hc,ws,ei,ay);}
ps("\nEland breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=270+(i*8),bw=440+(i*25),hc=110+(i*4),ws=3+(i*2),ei=(i%5)+1,ay=(i%12)+1;eland_breeding(lc,bl,bw,hc,ws,ei,ay);}
ps("\nEland health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=245+(i*15),bw=390+(i*50),hc=98+(i*8),ws=3+(i*2),ei=(i%10)+1,ay=(i%10)+1;eland_health(lc,bl,bw,hc,ws,ei,ay);}
ps("\nEland market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=280+(i*6),bw=460+(i*20),hc=115+(i*3),ws=6+(i*1),ei=(i%4)+1,ay=(i%8)+1;eland_market(lc,bl,bw,hc,ws,ei,ay);}
ps("\n");eland_report();eland_state();ps("\n=== Demo Complete ===\n");return 0;}
