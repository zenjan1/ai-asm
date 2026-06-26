/* rosemary_mouse_admin: Rosemary-mouse herb rodents (v1.0)
 * Rosemary-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, root_dep, tail_cm, rsmmx_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,root_dep,tail_cm,rsmmx_idx,age_yr,active;} rsmmx_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_rd,t_tail,t_ri;} rsmmx_state_t;
static rsmmx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static rsmmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rsmmx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int rd,int tc,int ri,int ay){if(*cnt>=mx)return -1;rsmmx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->root_dep=rd;x->tail_cm=tc;x->rsmmx_idx=ri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RSMMX] Rosemary-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" rd=");pi(rd);ps(" tc=");pi(tc);ps(" ri=");pi(ri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rosemary_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_rd=0;st.t_tail=0;st.t_ri=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[RSMMX] Rosemary-mouse initialized\n");return 0;}
int rosemary_mouse_forest(int lc,int bl,int bw,int rd,int tc,int ri,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,rd,tc,ri,ay);}
int rosemary_mouse_feeding(int lc,int bl,int bw,int rd,int tc,int ri,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,rd,tc,ri,ay);}
int rosemary_mouse_breeding(int lc,int bl,int bw,int rd,int tc,int ri,int ay){return add(svb,&st.n_breed,&st.t_rd,N-4,lc,bl,bw,rd,tc,ri,ay);}
int rosemary_mouse_health(int lc,int bl,int bw,int rd,int tc,int ri,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,rd,tc,ri,ay);}
int rosemary_mouse_market(int lc,int bl,int bw,int rd,int tc,int ri,int ay){return add(svm,&st.n_mkt,&st.t_ri,N-6,lc,bl,bw,rd,tc,ri,ay);}
void rosemary_mouse_report(void){ps("[RSMMX] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Rd=");pi(st.t_rd);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Ri=");pi(st.t_ri);ps("\n");}
void rosemary_mouse_state(void){ps("[RSMMX] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rosemary-Mouse Admin Demo ===\n\n");rosemary_mouse_init();
ps("Rosemary-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=18+(i*3),rd=5+(i%10),tc=8+(i*1),ri=(i%7)+1,ay=(i%5)+1;rosemary_mouse_forest(lc,bl,bw,rd,tc,ri,ay);}
ps("\nRosemary-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=20+(i*3),rd=5+(i%10),tc=8+(i*1),ri=(i%6)+1,ay=(i%4)+1;rosemary_mouse_feeding(lc,bl,bw,rd,tc,ri,ay);}
ps("\nRosemary-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=22+(i*3),rd=6+(i%10),tc=9+(i*1),ri=(i%5)+1,ay=(i%3)+1;rosemary_mouse_breeding(lc,bl,bw,rd,tc,ri,ay);}
ps("\nRosemary-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=16+(i*3),rd=4+(i%10),tc=7+(i*1),ri=(i%9)+1,ay=(i%5)+1;rosemary_mouse_health(lc,bl,bw,rd,tc,ri,ay);}
ps("\nRosemary-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=24+(i*3),rd=7+(i%10),tc=9+(i*1),ri=(i%4)+1,ay=(i%3)+1;rosemary_mouse_market(lc,bl,bw,rd,tc,ri,ay);}
ps("\n");rosemary_mouse_report();rosemary_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
