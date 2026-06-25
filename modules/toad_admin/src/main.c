/* toad_admin: Toad (Bufo bufo) amphibian (v1.0)
 * Toad pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, leg_cm, jump_speed, wart_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,jp_sp,wt_idx,age_yr,active;} toad_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_jp,t_wt2;} toad_state_t;
static toad_t tpl[N],tfd[N-2],tbr[N-4],thl[N-6],tmk[N-6]; static toad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(toad_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int js,int wi,int ay){if(*cnt>=mx)return -1;toad_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lc2;x->jp_sp=js;x->wt_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TOAD] Toad ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" js=");pi(js);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int toad_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_jp=0;st.t_wt2=0;for(int i=0;i<N;i++)tpl[i].active=0;for(int i=0;i<N-2;i++)tfd[i].active=0;for(int i=0;i<N-4;i++)tbr[i].active=0;for(int i=0;i<N-6;i++)thl[i].active=0;for(int i=0;i<N-6;i++)tmk[i].active=0;init=1;ps("[TOAD] Toad initialized\n");return 0;}
int toad_pond(int lc,int bl,int bw,int lc2,int js,int wi,int ay){return add(tpl,&st.n_pond,&st.t_ln,N,lc,bl,bw,lc2,js,wi,ay);}
int toad_feeding(int lc,int bl,int bw,int lc2,int js,int wi,int ay){return add(tfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lc2,js,wi,ay);}
int toad_breeding(int lc,int bl,int bw,int lc2,int js,int wi,int ay){return add(tbr,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lc2,js,wi,ay);}
int toad_health(int lc,int bl,int bw,int lc2,int js,int wi,int ay){return add(thl,&st.n_health,&st.t_jp,N-6,lc,bl,bw,lc2,js,wi,ay);}
int toad_market(int lc,int bl,int bw,int lc2,int js,int wi,int ay){return add(tmk,&st.n_mkt,&st.t_wt2,N-6,lc,bl,bw,lc2,js,wi,ay);}
void toad_report(void){ps("[TOAD] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Wt2=");pi(st.t_wt2);ps("\n");}
void toad_state(void){ps("[TOAD] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Toad Admin Demo ===\n\n");toad_init();
ps("Toad pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*2),bw=50+(i*15),lg=4+(i%3),js=5+(i*3),wi=(i%6)+1,ay=(i%3)+1;toad_pond(lc,bl,bw,lg,js,wi,ay);}
ps("\nToad feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=10+(i*2),bw=60+(i*12),lg=5+(i%2),js=6+(i*2),wi=(i%5)+1,ay=(i%3)+1;toad_feeding(lc,bl,bw,lg,js,wi,ay);}
ps("\nToad breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=6+(i*3),bw=40+(i*18),lg=3+(i%4),js=4+(i*4),wi=(i%4)+1,ay=(i%2)+1;toad_breeding(lc,bl,bw,lg,js,wi,ay);}
ps("\nToad health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i),bw=70+(i*10),lg=6+(i%2),js=8+(i*2),wi=(i%7)+1,ay=(i%4)+2;toad_health(lc,bl,bw,lg,js,wi,ay);}
ps("\nToad market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=14+(i%3),bw=80+(i*8),lg=7+(i%2),js=3+(i*3),wi=(i%3)+3,ay=(i%2)+2;toad_market(lc,bl,bw,lg,js,wi,ay);}
ps("\n");toad_report();toad_state();ps("\n=== Demo Complete ===\n");return 0;}
