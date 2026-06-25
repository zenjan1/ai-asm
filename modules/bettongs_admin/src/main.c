/* bettongs_admin: Bettongs (Bettongia) rat-kangaroos (v1.0)
 * Bettongs forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, foot_cm, bt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,foot_cm,bt_idx,age_yr,active;} btng_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_foot,t_bt;} btng_state_t;
static btng_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static btng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(btng_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int fc,int bt,int ay){if(*cnt>=mx)return -1;btng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->foot_cm=fc;x->bt_idx=bt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BTNG] Bettong ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" fc=");pi(fc);ps(" bt=");pi(bt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bettongs_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_foot=0;st.t_bt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BTNG] Bettongs initialized\n");return 0;}
int bettongs_forest(int lc,int bl,int bw,int tc,int fc,int bt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,fc,bt,ay);}
int bettongs_feeding(int lc,int bl,int bw,int tc,int fc,int bt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,fc,bt,ay);}
int bettongs_breeding(int lc,int bl,int bw,int tc,int fc,int bt,int ay){return add(svb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,fc,bt,ay);}
int bettongs_health(int lc,int bl,int bw,int tc,int fc,int bt,int ay){return add(svh,&st.n_health,&st.t_foot,N-6,lc,bl,bw,tc,fc,bt,ay);}
int bettongs_market(int lc,int bl,int bw,int tc,int fc,int bt,int ay){return add(svm,&st.n_mkt,&st.t_bt,N-6,lc,bl,bw,tc,fc,bt,ay);}
void bettongs_report(void){ps("[BTNG] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Foot=");pi(st.t_foot);ps("\nMkt: ");pi(st.n_mkt);ps(" Bt=");pi(st.t_bt);ps("\n");}
void bettongs_state(void){ps("[BTNG] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bettongs Admin Demo ===\n\n");bettongs_init();
ps("Bettongs forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=300+(i*20),tc=20+(i*2),fc=8+(i%3),bt=(i%8)+1,ay=(i%5)+1;bettongs_forest(lc,bl,bw,tc,fc,bt,ay);}
ps("\nBettongs feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*1),bw=310+(i*15),tc=21+(i%2),fc=9+(i%2),bt=(i%6)+1,ay=(i%4)+1;bettongs_feeding(lc,bl,bw,tc,fc,bt,ay);}
ps("\nBettongs breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*1),bw=320+(i*10),tc=23+(i%2),fc=10+(i%2),bt=(i%5)+1,ay=(i%3)+1;bettongs_breeding(lc,bl,bw,tc,fc,bt,ay);}
ps("\nBettongs health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=280+(i*25),tc=18+(i*2),fc=7+(i%3),bt=(i%10)+1,ay=(i%5)+1;bettongs_health(lc,bl,bw,tc,fc,bt,ay);}
ps("\nBettongs market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=42+(i*1),bw=340+(i*8),tc=25+(i%2),fc=11+(i%2),bt=(i%4)+1,ay=(i%3)+1;bettongs_market(lc,bl,bw,tc,fc,bt,ay);}
ps("\n");bettongs_report();bettongs_state();ps("\n=== Demo Complete ===\n");return 0;}
