/* bongu_admin: Bongu (Dendrohyrax arboreus) tree-dwelling hyrax (v1.0)
 * Bongu tree, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, climb_speed, bg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,cl_sp,bg_idx,age_yr,active;} bong_t;
typedef struct{int n_tree,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_cl,t_bg;} bong_state_t;
static bong_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bong_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bong_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int cs,int bi,int ay){if(*cnt>=mx)return -1;bong_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->cl_sp=cs;x->bg_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BONG] Bongu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" cs=");pi(cs);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bongu_init(void){if(init)return -1;st.n_tree=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_cl=0;st.t_bg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BONG] Bongu initialized\n");return 0;}
int bongu_tree(int lc,int bl,int bw,int ec,int cs,int bi,int ay){return add(svl,&st.n_tree,&st.t_ln,N,lc,bl,bw,ec,cs,bi,ay);}
int bongu_feeding(int lc,int bl,int bw,int ec,int cs,int bi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,cs,bi,ay);}
int bongu_breeding(int lc,int bl,int bw,int ec,int cs,int bi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,cs,bi,ay);}
int bongu_health(int lc,int bl,int bw,int ec,int cs,int bi,int ay){return add(svh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ec,cs,bi,ay);}
int bongu_market(int lc,int bl,int bw,int ec,int cs,int bi,int ay){return add(svm,&st.n_mkt,&st.t_bg,N-6,lc,bl,bw,ec,cs,bi,ay);}
void bongu_report(void){ps("[BONG] Tree: ");pi(st.n_tree);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Bg=");pi(st.t_bg);ps("\n");}
void bongu_state(void){ps("[BONG] Tree=");pi(st.n_tree);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bongu Admin Demo ===\n\n");bongu_init();
ps("Bongu tree...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=4+(i*1),ec=5+(i*1),cs=6+(i*1),bi=(i%8)+1,ay=(i%10)+1;bongu_tree(lc,bl,bw,ec,cs,bi,ay);}
ps("\nBongu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=5+(i*1),ec=6+(i*1),cs=7+(i*1),bi=(i%6)+1,ay=(i%8)+1;bongu_feeding(lc,bl,bw,ec,cs,bi,ay);}
ps("\nBongu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=49+(i*2),bw=6+(i*1),ec=6+(i*1),cs=5+(i*1),bi=(i%5)+1,ay=(i%7)+1;bongu_breeding(lc,bl,bw,ec,cs,bi,ay);}
ps("\nBongu health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=43+(i*4),bw=3+(i*1),ec=4+(i*1),cs=4+(i*2),bi=(i%10)+1,ay=(i%6)+1;bongu_health(lc,bl,bw,ec,cs,bi,ay);}
ps("\nBongu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=51+(i*1),bw=7+(i*1),ec=7+(i*1),cs=8+(i*1),bi=(i%4)+1,ay=(i%5)+1;bongu_market(lc,bl,bw,ec,cs,bi,ay);}
ps("\n");bongu_report();bongu_state();ps("\n=== Demo Complete ===\n");return 0;}
