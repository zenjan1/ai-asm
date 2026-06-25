/* capybara_admin: Capybara (Hydrochoerus hydrochaeris) semi-aquatic largest rodent (v1.0)
 * Capybara wetland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, swim_speed, cp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,sw_sp,cp_idx,age_yr,active;} capy_t;
typedef struct{int n_wet,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_sw,t_cp;} capy_state_t;
static capy_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static capy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(capy_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ss,int ci,int ay){if(*cnt>=mx)return -1;capy_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->sw_sp=ss;x->cp_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CAPY] Capybara ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ss=");pi(ss);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int capybara_init(void){if(init)return -1;st.n_wet=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_sw=0;st.t_cp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[CAPY] Capybara initialized\n");return 0;}
int capybara_wetland(int lc,int bl,int bw,int ec,int ss,int ci,int ay){return add(svl,&st.n_wet,&st.t_ln,N,lc,bl,bw,ec,ss,ci,ay);}
int capybara_feeding(int lc,int bl,int bw,int ec,int ss,int ci,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ss,ci,ay);}
int capybara_breeding(int lc,int bl,int bw,int ec,int ss,int ci,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ss,ci,ay);}
int capybara_health(int lc,int bl,int bw,int ec,int ss,int ci,int ay){return add(svh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,ec,ss,ci,ay);}
int capybara_market(int lc,int bl,int bw,int ec,int ss,int ci,int ay){return add(svm,&st.n_mkt,&st.t_cp,N-6,lc,bl,bw,ec,ss,ci,ay);}
void capybara_report(void){ps("[CAPY] Wet: ");pi(st.n_wet);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Cp=");pi(st.t_cp);ps("\n");}
void capybara_state(void){ps("[CAPY] Wet=");pi(st.n_wet);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Capybara Admin Demo ===\n\n");capybara_init();
ps("Capybara wetland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*6),bw=40+(i*5),ec=6+(i*1),ss=5+(i*1),ci=(i%8)+1,ay=(i%12)+1;capybara_wetland(lc,bl,bw,ec,ss,ci,ay);}
ps("\nCapybara feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*5),bw=42+(i*4),ec=7+(i*1),ss=6+(i*1),ci=(i%6)+1,ay=(i%10)+1;capybara_feeding(lc,bl,bw,ec,ss,ci,ay);}
ps("\nCapybara breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*4),bw=45+(i*3),ec=7+(i*1),ss=4+(i*1),ci=(i%5)+1,ay=(i%8)+1;capybara_breeding(lc,bl,bw,ec,ss,ci,ay);}
ps("\nCapybara health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*8),bw=38+(i*6),ec=5+(i*1),ss=3+(i*2),ci=(i%10)+1,ay=(i%7)+1;capybara_health(lc,bl,bw,ec,ss,ci,ay);}
ps("\nCapybara market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*3),bw=48+(i*2),ec=8+(i*1),ss=7+(i*1),ci=(i%4)+1,ay=(i%6)+1;capybara_market(lc,bl,bw,ec,ss,ci,ay);}
ps("\n");capybara_report();capybara_state();ps("\n=== Demo Complete ===\n");return 0;}
