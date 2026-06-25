/* turtle_admin: Turtle (Chelonia mydas) sea turtle (v1.0)
 * Turtle ocean, feeding, breeding, health, market
 * Features: shell_cm, body_wt_kg, flipper_cm, swim_speed, scute_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,shl_cm,bdy_wt,flpr_cm,sw_sp,sc_idx,age_yr,active;} trtl_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_sh,t_wt,t_fl,t_sw,t_sc;} trtl_state_t;
static trtl_t tpl[N],tfd[N-2],tbr[N-4],thl[N-6],tmk[N-6]; static trtl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trtl_t*a,int*cnt,int*sum,int mx,int lc,int sc,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;trtl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->shl_cm=sc;x->bdy_wt=bw;x->flpr_cm=fc;x->sw_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=sc;(*cnt)++;ps("[TRTL] Turtle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int trtl_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_sh=0;st.t_wt=0;st.t_fl=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)tpl[i].active=0;for(int i=0;i<N-2;i++)tfd[i].active=0;for(int i=0;i<N-4;i++)tbr[i].active=0;for(int i=0;i<N-6;i++)thl[i].active=0;for(int i=0;i<N-6;i++)tmk[i].active=0;init=1;ps("[TRTL] Turtle initialized\n");return 0;}
int trtl_ocean(int lc,int sc,int bw,int fc,int ss,int si,int ay){return add(tpl,&st.n_ocean,&st.t_sh,N,lc,sc,bw,fc,ss,si,ay);}
int trtl_feeding(int lc,int sc,int bw,int fc,int ss,int si,int ay){return add(tfd,&st.n_feed,&st.t_wt,N-2,lc,sc,bw,fc,ss,si,ay);}
int trtl_breeding(int lc,int sc,int bw,int fc,int ss,int si,int ay){return add(tbr,&st.n_breed,&st.t_fl,N-4,lc,sc,bw,fc,ss,si,ay);}
int trtl_health(int lc,int sc,int bw,int fc,int ss,int si,int ay){return add(thl,&st.n_health,&st.t_sw,N-6,lc,sc,bw,fc,ss,si,ay);}
int trtl_market(int lc,int sc,int bw,int fc,int ss,int si,int ay){return add(tmk,&st.n_mkt,&st.t_sc,N-6,lc,sc,bw,fc,ss,si,ay);}
void trtl_report(void){ps("[TRTL] Ocean: ");pi(st.n_ocean);ps(" Sh=");pi(st.t_sh);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fl=");pi(st.t_fl);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void trtl_state(void){ps("[TRTL] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Turtle Admin Demo ===\n\n");trtl_init();
ps("Turtle ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sc=60+(i*8),bw=80+(i*20),fc=25+(i*4),ss=5+(i*2),si=(i%6)+1,ay=(i%8)+5;trtl_ocean(lc,sc,bw,fc,ss,si,ay);}
ps("\nTurtle feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,sc=70+(i*6),bw=100+(i*15),fc=28+(i*3),ss=6+(i*2),si=(i%5)+1,ay=(i%6)+4;trtl_feeding(lc,sc,bw,fc,ss,si,ay);}
ps("\nTurtle breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sc=50+(i*10),bw=60+(i*25),fc=22+(i*5),ss=4+(i*3),si=(i%4)+1,ay=(i%5)+3;trtl_breeding(lc,sc,bw,fc,ss,si,ay);}
ps("\nTurtle health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sc=80+(i*5),bw=120+(i*10),fc=30+(i*2),ss=7+(i),si=(i%7)+1,ay=(i%4)+6;trtl_health(lc,sc,bw,fc,ss,si,ay);}
ps("\nTurtle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sc=90+(i*4),bw=140+(i*8),fc=32+(i*2),ss=3+(i*2),si=(i%3)+3,ay=(i%3)+8;trtl_market(lc,sc,bw,fc,ss,si,ay);}
ps("\n");trtl_report();trtl_state();ps("\n=== Demo Complete ===\n");return 0;}
