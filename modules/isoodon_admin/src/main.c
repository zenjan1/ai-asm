/* isoodon_admin: Isoodon (Isoodon) short-nosed bandicoots (v1.0)
 * Isoodon forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, tail_cm, is_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,tail_cm,is_idx,age_yr,active;} isod_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_tail,t_is;} isod_state_t;
static isod_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static isod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(isod_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int tc,int is,int ay){if(*cnt>=mx)return -1;isod_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->tail_cm=tc;x->is_idx=is;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ISOD] Isoodon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" tc=");pi(tc);ps(" is=");pi(is);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int isoodon_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_tail=0;st.t_is=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ISOD] Isoodon initialized\n");return 0;}
int isoodon_forest(int lc,int bl,int bw,int ec,int tc,int is,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ec,tc,is,ay);}
int isoodon_feeding(int lc,int bl,int bw,int ec,int tc,int is,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,tc,is,ay);}
int isoodon_breeding(int lc,int bl,int bw,int ec,int tc,int is,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,tc,is,ay);}
int isoodon_health(int lc,int bl,int bw,int ec,int tc,int is,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,ec,tc,is,ay);}
int isoodon_market(int lc,int bl,int bw,int ec,int tc,int is,int ay){return add(svm,&st.n_mkt,&st.t_is,N-6,lc,bl,bw,ec,tc,is,ay);}
void isoodon_report(void){ps("[ISOD] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Is=");pi(st.t_is);ps("\n");}
void isoodon_state(void){ps("[ISOD] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Isoodon Admin Demo ===\n\n");isoodon_init();
ps("Isoodon forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=28+(i*2),bw=1+(i*1),ec=3+(i%3),tc=8+(i*2),is=(i%8)+1,ay=(i%5)+1;isoodon_forest(lc,bl,bw,ec,tc,is,ay);}
ps("\nIsoodon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=30+(i*1),bw=2+(i*1),ec=4+(i%2),tc=9+(i%1),is=(i%6)+1,ay=(i%4)+1;isoodon_feeding(lc,bl,bw,ec,tc,is,ay);}
ps("\nIsoodon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=32+(i*1),bw=2+(i*1),ec=4+(i%2),tc=10+(i%1),is=(i%5)+1,ay=(i%3)+1;isoodon_breeding(lc,bl,bw,ec,tc,is,ay);}
ps("\nIsoodon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=26+(i*3),bw=1+(i*1),ec=2+(i%3),tc=7+(i*2),is=(i%10)+1,ay=(i%5)+1;isoodon_health(lc,bl,bw,ec,tc,is,ay);}
ps("\nIsoodon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=34+(i*1),bw=3+(i*1),ec=5+(i%2),tc=11+(i%1),is=(i%4)+1,ay=(i%3)+1;isoodon_market(lc,bl,bw,ec,tc,is,ay);}
ps("\n");isoodon_report();isoodon_state();ps("\n=== Demo Complete ===\n");return 0;}
