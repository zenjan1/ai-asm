/* bongo_admin: Bongo (Tragelaphus eurycerus) large forest antelope (v1.0)
 * Bongo forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, sprint_speed, bg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,sp_sp,bg_idx,age_yr,active;} bngo_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_sp,t_bg;} bngo_state_t;
static bngo_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bngo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bngo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int bi,int ay){if(*cnt>=mx)return -1;bngo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->sp_sp=ss;x->bg_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BNGO] Bongo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bongo_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_sp=0;st.t_bg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BNGO] Bongo initialized\n");return 0;}
int bongo_forest(int lc,int bl,int bw,int hc,int ss,int bi,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,hc,ss,bi,ay);}
int bongo_feeding(int lc,int bl,int bw,int hc,int ss,int bi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,bi,ay);}
int bongo_breeding(int lc,int bl,int bw,int hc,int ss,int bi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ss,bi,ay);}
int bongo_health(int lc,int bl,int bw,int hc,int ss,int bi,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,hc,ss,bi,ay);}
int bongo_market(int lc,int bl,int bw,int hc,int ss,int bi,int ay){return add(svm,&st.n_mkt,&st.t_bg,N-6,lc,bl,bw,hc,ss,bi,ay);}
void bongo_report(void){ps("[BNGO] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Bg=");pi(st.t_bg);ps("\n");}
void bongo_state(void){ps("[BNGO] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bongo Admin Demo ===\n\n");bongo_init();
ps("Bongo forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=220+(i*10),bw=250+(i*20),hc=100+(i*6),ss=22+(i*3),bi=(i%8)+1,ay=(i%15)+1;bongo_forest(lc,bl,bw,hc,ss,bi,ay);}
ps("\nBongo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=225+(i*8),bw=260+(i*16),hc=105+(i*5),ss=24+(i*2),bi=(i%6)+1,ay=(i%12)+1;bongo_feeding(lc,bl,bw,hc,ss,bi,ay);}
ps("\nBongo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=230+(i*6),bw=270+(i*12),hc=110+(i*4),ss=21+(i*3),bi=(i%5)+1,ay=(i%10)+1;bongo_breeding(lc,bl,bw,hc,ss,bi,ay);}
ps("\nBongo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=215+(i*12),bw=240+(i*25),hc=98+(i*7),ss=19+(i*4),bi=(i%10)+1,ay=(i%8)+1;bongo_health(lc,bl,bw,hc,ss,bi,ay);}
ps("\nBongo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=238+(i*5),bw=280+(i*10),hc=115+(i*3),ss=26+(i*2),bi=(i%4)+1,ay=(i%7)+1;bongo_market(lc,bl,bw,hc,ss,bi,ay);}
ps("\n");bongo_report();bongo_state();ps("\n=== Demo Complete ===\n");return 0;}
