/* gliders_admin: Gliders (Petaurus) flying marsupials (v1.0)
 * Gliders glide, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, glide_m, gl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,glide_m,gl_idx,age_yr,active;} glds_t;
typedef struct{int n_glide,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_gld,t_gl;} glds_state_t;
static glds_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static glds_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(glds_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int gm,int gl,int ay){if(*cnt>=mx)return -1;glds_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->glide_m=gm;x->gl_idx=gl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GLDS] Glider ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" gm=");pi(gm);ps(" gl=");pi(gl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gliders_init(void){if(init)return -1;st.n_glide=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_gld=0;st.t_gl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[GLDS] Gliders initialized\n");return 0;}
int gliders_glide(int lc,int bl,int bw,int wc,int gm,int gl,int ay){return add(svl,&st.n_glide,&st.t_ln,N,lc,bl,bw,wc,gm,gl,ay);}
int gliders_feeding(int lc,int bl,int bw,int wc,int gm,int gl,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,gm,gl,ay);}
int gliders_breeding(int lc,int bl,int bw,int wc,int gm,int gl,int ay){return add(svb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,gm,gl,ay);}
int gliders_health(int lc,int bl,int bw,int wc,int gm,int gl,int ay){return add(svh,&st.n_health,&st.t_gld,N-6,lc,bl,bw,wc,gm,gl,ay);}
int gliders_market(int lc,int bl,int bw,int wc,int gm,int gl,int ay){return add(svm,&st.n_mkt,&st.t_gl,N-6,lc,bl,bw,wc,gm,gl,ay);}
void gliders_report(void){ps("[GLDS] Glide: ");pi(st.n_glide);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Gld=");pi(st.t_gld);ps("\nMkt: ");pi(st.n_mkt);ps(" Gl=");pi(st.t_gl);ps("\n");}
void gliders_state(void){ps("[GLDS] Glide=");pi(st.n_glide);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gliders Admin Demo ===\n\n");gliders_init();
ps("Gliders glide...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*1),bw=100+(i*10),wc=8+(i*1),gm=20+(i*3),gl=(i%8)+1,ay=(i%5)+1;gliders_glide(lc,bl,bw,wc,gm,gl,ay);}
ps("\nGliders feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i*1),bw=110+(i*8),wc=9+(i%2),gm=22+(i*2),gl=(i%6)+1,ay=(i%4)+1;gliders_feeding(lc,bl,bw,wc,gm,gl,ay);}
ps("\nGliders breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i*1),bw=120+(i*6),wc=9+(i%2),gm=24+(i*2),gl=(i%5)+1,ay=(i%4)+1;gliders_breeding(lc,bl,bw,wc,gm,gl,ay);}
ps("\nGliders health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*2),bw=95+(i*12),wc=7+(i%3),gm=18+(i*3),gl=(i%10)+1,ay=(i%5)+1;gliders_health(lc,bl,bw,wc,gm,gl,ay);}
ps("\nGliders market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i*1),bw=130+(i*5),wc=10+(i%2),gm=26+(i*2),gl=(i%4)+1,ay=(i%3)+1;gliders_market(lc,bl,bw,wc,gm,gl,ay);}
ps("\n");gliders_report();gliders_state();ps("\n=== Demo Complete ===\n");return 0;}
