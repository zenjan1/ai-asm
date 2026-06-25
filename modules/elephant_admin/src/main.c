/* elephant_admin: Elephant (Loxodonta africana) African large mammal (v1.0)
 * Elephant savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, walk_speed, ear_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,wk_sp,er_idx,age_yr,active;} elph_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_wk,t_er;} elph_state_t;
static elph_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static elph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elph_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ws,int ei,int ay){if(*cnt>=mx)return -1;elph_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->wk_sp=ws;x->er_idx=ei;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ELPH] Elephant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ws=");pi(ws);ps(" ei=");pi(ei);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int elephant_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_wk=0;st.t_er=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ELPH] Elephant initialized\n");return 0;}
int elephant_savanna(int lc,int bl,int bw,int tc,int ws,int ei,int ay){return add(svl,&st.n_savan,&st.t_ln,N,lc,bl,bw,tc,ws,ei,ay);}
int elephant_feeding(int lc,int bl,int bw,int tc,int ws,int ei,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ws,ei,ay);}
int elephant_breeding(int lc,int bl,int bw,int tc,int ws,int ei,int ay){return add(svb,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,ws,ei,ay);}
int elephant_health(int lc,int bl,int bw,int tc,int ws,int ei,int ay){return add(svh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,tc,ws,ei,ay);}
int elephant_market(int lc,int bl,int bw,int tc,int ws,int ei,int ay){return add(svm,&st.n_mkt,&st.t_er,N-6,lc,bl,bw,tc,ws,ei,ay);}
void elephant_report(void){ps("[ELPH] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Er=");pi(st.t_er);ps("\n");}
void elephant_state(void){ps("[ELPH] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Elephant Admin Demo ===\n\n");elephant_init();
ps("Elephant savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=400+(i*20),bw=3000+(i*300),tc=100+(i*15),ws=3+(i*2),ei=(i%8)+1,ay=(i%30)+1;elephant_savanna(lc,bl,bw,tc,ws,ei,ay);}
ps("\nElephant feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=410+(i*15),bw=3200+(i*250),tc=105+(i*12),ws=4+(i*1),ei=(i%6)+1,ay=(i%25)+1;elephant_feeding(lc,bl,bw,tc,ws,ei,ay);}
ps("\nElephant breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=420+(i*10),bw=3400+(i*200),tc=110+(i*10),ws=2+(i*3),ei=(i%5)+1,ay=(i%20)+1;elephant_breeding(lc,bl,bw,tc,ws,ei,ay);}
ps("\nElephant health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=390+(i*25),bw=2900+(i*400),tc=95+(i*18),ws=2+(i*4),ei=(i%10)+1,ay=(i%15)+1;elephant_health(lc,bl,bw,tc,ws,ei,ay);}
ps("\nElephant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=430+(i*8),bw=3600+(i*150),tc=115+(i*8),ws=5+(i*2),ei=(i%4)+1,ay=(i%12)+1;elephant_market(lc,bl,bw,tc,ws,ei,ay);}
ps("\n");elephant_report();elephant_state();ps("\n=== Demo Complete ===\n");return 0;}
