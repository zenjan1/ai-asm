/* steenbok_admin: Steenbok (Raphicerus campestris) tiny bush antelope (v1.0)
 * Steenbok thicket, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, sprint_speed, st_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,sp_sp,st_idx,age_yr,active;} stnb_t;
typedef struct{int n_thick,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_sp,t_st;} stnb_state_t;
static stnb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static stnb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(stnb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ss,int si,int ay){if(*cnt>=mx)return -1;stnb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->sp_sp=ss;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[STNB] Steenbok ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int steenbok_init(void){if(init)return -1;st.n_thick=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_sp=0;st.t_st=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[STNB] Steenbok initialized\n");return 0;}
int steenbok_thicket(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svl,&st.n_thick,&st.t_ln,N,lc,bl,bw,ec,ss,si,ay);}
int steenbok_feeding(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ss,si,ay);}
int steenbok_breeding(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ss,si,ay);}
int steenbok_health(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,ec,ss,si,ay);}
int steenbok_market(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svm,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,ec,ss,si,ay);}
void steenbok_report(void){ps("[STNB] Thick: ");pi(st.n_thick);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void steenbok_state(void){ps("[STNB] Thick=");pi(st.n_thick);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Steenbok Admin Demo ===\n\n");steenbok_init();
ps("Steenbok thicket...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*3),bw=10+(i*1),ec=6+(i*1),ss=15+(i*2),si=(i%8)+1,ay=(i%10)+1;steenbok_thicket(lc,bl,bw,ec,ss,si,ay);}
ps("\nSteenbok feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=72+(i*2),bw=11+(i*1),ec=7+(i*1),ss=16+(i*1),si=(i%6)+1,ay=(i%8)+1;steenbok_feeding(lc,bl,bw,ec,ss,si,ay);}
ps("\nSteenbok breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=74+(i*2),bw=12+(i*1),ec=7+(i*1),ss=14+(i*2),si=(i%5)+1,ay=(i%7)+1;steenbok_breeding(lc,bl,bw,ec,ss,si,ay);}
ps("\nSteenbok health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=68+(i*4),bw=9+(i*2),ec=5+(i*1),ss=13+(i*3),si=(i%10)+1,ay=(i%6)+1;steenbok_health(lc,bl,bw,ec,ss,si,ay);}
ps("\nSteenbok market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=76+(i*1),bw=13+(i*1),ec=8+(i*1),ss=18+(i*1),si=(i%4)+1,ay=(i%5)+1;steenbok_market(lc,bl,bw,ec,ss,si,ay);}
ps("\n");steenbok_report();steenbok_state();ps("\n=== Demo Complete ===\n");return 0;}
