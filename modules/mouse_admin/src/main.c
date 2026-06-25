/* mouse_admin: Mouse (Mus musculus) house mouse small rodent (v1.0)
 * Mouse nest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_len_cm, whisker_idx, nest_size, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,wh_idx,ns_sz,age_yr,active;} mous_t;
typedef struct{int n_nest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_wh,t_ns;} mous_state_t;
static mous_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mous_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mous_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int wi,int ns,int ay){if(*cnt>=mx)return -1;mous_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->wh_idx=wi;x->ns_sz=ns;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MOUS] Mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" wi=");pi(wi);ps(" ns=");pi(ns);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mous_init(void){if(init)return -1;st.n_nest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_wh=0;st.t_ns=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MOUS] Mouse initialized\n");return 0;}
int mous_nest(int lc,int bl,int bw,int tl,int wi,int ns,int ay){return add(apl,&st.n_nest,&st.t_ln,N,lc,bl,bw,tl,wi,ns,ay);}
int mous_feeding(int lc,int bl,int bw,int tl,int wi,int ns,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,wi,ns,ay);}
int mous_breeding(int lc,int bl,int bw,int tl,int wi,int ns,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,wi,ns,ay);}
int mous_health(int lc,int bl,int bw,int tl,int wi,int ns,int ay){return add(ahd,&st.n_health,&st.t_wh,N-6,lc,bl,bw,tl,wi,ns,ay);}
int mous_market(int lc,int bl,int bw,int tl,int wi,int ns,int ay){return add(amd,&st.n_mkt,&st.t_ns,N-6,lc,bl,bw,tl,wi,ns,ay);}
void mous_report(void){ps("[MOUS] Nest: ");pi(st.n_nest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Wh=");pi(st.t_wh);ps("\nMkt: ");pi(st.n_mkt);ps(" Ns=");pi(st.t_ns);ps("\n");}
void mous_state(void){ps("[MOUS] Nest=");pi(st.n_nest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mouse Admin Demo ===\n\n");mous_init();
ps("Mouse nest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=6+(i*2),bw=15+(i*3),tl=5+(i*2),wi=(i%6)+1,ns=3+(i*2),ay=1+(i%3);mous_nest(lc,bl,bw,tl,wi,ns,ay);}
ps("\nMouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=7+(i*2),bw=16+(i*3),tl=6+(i*2),wi=(i%5)+2,ns=4+(i*2),ay=1+(i%2);mous_feeding(lc,bl,bw,tl,wi,ns,ay);}
ps("\nMouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=8+(i*2),bw=18+(i*3),tl=7+(i*2),wi=(i%4)+1,ns=5+(i*2),ay=1+(i%2);mous_breeding(lc,bl,bw,tl,wi,ns,ay);}
ps("\nMouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=5+(i*3),bw=14+(i*4),tl=4+(i*3),wi=(i%3)+3,ns=2+(i*3),ay=1+(i%2);mous_health(lc,bl,bw,tl,wi,ns,ay);}
ps("\nMouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=10+(i*2),bw=20+(i*3),tl=8+(i*2),wi=(i%6)+1,ns=6+(i*2),ay=1+(i%2);mous_market(lc,bl,bw,tl,wi,ns,ay);}
ps("\n");mous_report();mous_state();ps("\n=== Demo Complete ===\n");return 0;}
