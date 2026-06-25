/* porcupine_admin: Porcupine (Erethizon dorsatum) North American porcupine (v1.0)
 * Porcupine forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, quill_ct, tail_len, climb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,qu_ct,tail_ln,cl_idx,age_yr,active;} prcp_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_qu,t_tail,t_cl;} prcp_state_t;
static prcp_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static prcp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prcp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int qc,int tl,int ci,int ay){if(*cnt>=mx)return -1;prcp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->qu_ct=qc;x->tail_ln=tl;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PRCP] Porcupine ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" qc=");pi(qc);ps(" tl=");pi(tl);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int prcp_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_qu=0;st.t_tail=0;st.t_cl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PRCP] Porcupine initialized\n");return 0;}
int prcp_forest(int lc,int bl,int bw,int qc,int tl,int ci,int ay){return add(apl,&st.n_forest,&st.t_ln,N,lc,bl,bw,qc,tl,ci,ay);}
int prcp_feeding(int lc,int bl,int bw,int qc,int tl,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,qc,tl,ci,ay);}
int prcp_breeding(int lc,int bl,int bw,int qc,int tl,int ci,int ay){return add(abd,&st.n_breed,&st.t_qu,N-4,lc,bl,bw,qc,tl,ci,ay);}
int prcp_health(int lc,int bl,int bw,int qc,int tl,int ci,int ay){return add(ahd,&st.n_health,&st.t_tail,N-6,lc,bl,bw,qc,tl,ci,ay);}
int prcp_market(int lc,int bl,int bw,int qc,int tl,int ci,int ay){return add(amd,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,qc,tl,ci,ay);}
void prcp_report(void){ps("[PRCP] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Qu=");pi(st.t_qu);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void prcp_state(void){ps("[PRCP] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Porcupine Admin Demo ===\n\n");prcp_init();
ps("Porcupine forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*4),bw=8+(i*2),qc=5000+(i*500),tl=15+(i*2),ci=(i%7)+1,ay=2+(i%12);prcp_forest(lc,bl,bw,qc,tl,ci,ay);}
ps("\nPorcupine feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=62+(i*4),bw=9+(i*2),qc=5200+(i*500),tl=16+(i*2),ci=(i%6)+2,ay=3+(i%10);prcp_feeding(lc,bl,bw,qc,tl,ci,ay);}
ps("\nPorcupine breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=65+(i*4),bw=9+(i*2),qc=5500+(i*500),tl=17+(i*2),ci=(i%5)+1,ay=4+(i%8);prcp_breeding(lc,bl,bw,qc,tl,ci,ay);}
ps("\nPorcupine health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=58+(i*5),bw=7+(i*3),qc=4800+(i*600),tl=14+(i*3),ci=(i%4)+3,ay=5+(i%6);prcp_health(lc,bl,bw,qc,tl,ci,ay);}
ps("\nPorcupine market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=70+(i*4),bw=10+(i*2),qc=6000+(i*500),tl=18+(i*2),ci=(i%3)+4,ay=6+(i%5);prcp_market(lc,bl,bw,qc,tl,ci,ay);}
ps("\n");prcp_report();prcp_state();ps("\n=== Demo Complete ===\n");return 0;}
