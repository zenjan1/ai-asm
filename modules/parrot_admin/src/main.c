/* parrot_admin: Parrot (Amazona aestiva) turquoise-fronted parrot (v1.0)
 * Parrot aviary, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, talk_vol, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,tk_vl,cl_idx,age_yr,active;} prrt_t;
typedef struct{int n_aviary,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_tk,t_cl;} prrt_state_t;
static prrt_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static prrt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prrt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int tv,int ci,int ay){if(*cnt>=mx)return -1;prrt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->tk_vl=tv;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PRRT] Parrot ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" tv=");pi(tv);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int prrt_init(void){if(init)return -1;st.n_aviary=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_tk=0;st.t_cl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PRRT] Parrot initialized\n");return 0;}
int prrt_aviary(int lc,int bl,int bw,int wc,int tv,int ci,int ay){return add(apl,&st.n_aviary,&st.t_ln,N,lc,bl,bw,wc,tv,ci,ay);}
int prrt_feeding(int lc,int bl,int bw,int wc,int tv,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,tv,ci,ay);}
int prrt_breeding(int lc,int bl,int bw,int wc,int tv,int ci,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,tv,ci,ay);}
int prrt_health(int lc,int bl,int bw,int wc,int tv,int ci,int ay){return add(ahd,&st.n_health,&st.t_tk,N-6,lc,bl,bw,wc,tv,ci,ay);}
int prrt_market(int lc,int bl,int bw,int wc,int tv,int ci,int ay){return add(amd,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,wc,tv,ci,ay);}
void prrt_report(void){ps("[PRRT] Aviary: ");pi(st.n_aviary);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Tk=");pi(st.t_tk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void prrt_state(void){ps("[PRRT] Aviary=");pi(st.n_aviary);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Parrot Admin Demo ===\n\n");prrt_init();
ps("Parrot aviary...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=300+(i*30),wc=18+(i*3),tv=40+(i*5),ci=(i%6)+1,ay=1+(i%20);prrt_aviary(lc,bl,bw,wc,tv,ci,ay);}
ps("\nParrot feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=320+(i*30),wc=19+(i*3),tv=42+(i*5),ci=(i%5)+2,ay=2+(i%18);prrt_feeding(lc,bl,bw,wc,tv,ci,ay);}
ps("\nParrot breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=35+(i*2),bw=340+(i*30),wc=20+(i*3),tv=45+(i*5),ci=(i%4)+1,ay=3+(i%15);prrt_breeding(lc,bl,bw,wc,tv,ci,ay);}
ps("\nParrot health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=280+(i*35),wc=17+(i*4),tv=38+(i*6),ci=(i%3)+3,ay=4+(i%12);prrt_health(lc,bl,bw,wc,tv,ci,ay);}
ps("\nParrot market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=38+(i*2),bw=360+(i*30),wc=22+(i*3),tv=50+(i*5),ci=(i%6)+1,ay=5+(i%10);prrt_market(lc,bl,bw,wc,tv,ci,ay);}
ps("\n");prrt_report();prrt_state();ps("\n=== Demo Complete ===\n");return 0;}
