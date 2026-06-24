/* firefly_admin: Firefly (Lampyris noctiluca) bioluminescent beetle management (v1.0)
 * Firefly habitat, feeding, breeding, display, market
 * Features: body_len_mm, body_wt_mg, glow_idx, wing_idx, flash_rate, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,glw_idx,wing_idx,fl_rt,age_dy,active;} firf_t;
typedef struct{int n_hab,n_feed,n_breed,n_disp,n_mkt,t_ln,t_wt,t_gl,t_wg,t_fl;} firf_state_t;
static firf_t fhab[N],ffd[N-2],fbr[N-4],fds[N-6],fmk[N-6]; static firf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(firf_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int gi,int wi,int fr,int ad){if(*cnt>=mx)return -1;firf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->glw_idx=gi;x->wing_idx=wi;x->fl_rt=fr;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[FIRF] Firefly ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" gi=");pi(gi);ps(" wi=");pi(wi);ps(" fr=");pi(fr);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int firf_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_breed=0;st.n_disp=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_gl=0;st.t_wg=0;st.t_fl=0;for(int i=0;i<N;i++)fhab[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fds[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FIRF] Firefly initialized\n");return 0;}
int firf_habitat(int lc,int bl,int bw,int gi,int wi,int fr,int ad){return add(fhab,&st.n_hab,&st.t_ln,N,lc,bl,bw,gi,wi,fr,ad);}
int firf_feeding(int lc,int bl,int bw,int gi,int wi,int fr,int ad){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,gi,wi,fr,ad);}
int firf_breeding(int lc,int bl,int bw,int gi,int wi,int fr,int ad){return add(fbr,&st.n_breed,&st.t_gl,N-4,lc,bl,bw,gi,wi,fr,ad);}
int firf_display(int lc,int bl,int bw,int gi,int wi,int fr,int ad){return add(fds,&st.n_disp,&st.t_wg,N-6,lc,bl,bw,gi,wi,fr,ad);}
int firf_market(int lc,int bl,int bw,int gi,int wi,int fr,int ad){return add(fmk,&st.n_mkt,&st.t_fl,N-6,lc,bl,bw,gi,wi,fr,ad);}
void firf_report(void){ps("[FIRF] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Gl=");pi(st.t_gl);ps("\nDisp: ");pi(st.n_disp);ps(" Wg=");pi(st.t_wg);ps("\nMkt: ");pi(st.n_mkt);ps(" Fl=");pi(st.t_fl);ps("\n");}
void firf_state(void){ps("[FIRF] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Disp=");pi(st.n_disp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Firefly Admin Demo ===\n\n");firf_init();
/* 1=meadow 2=garden 3=forest 4=jar 5=market */
ps("Firefly habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i%3),bw=15+(i*3),gi=(i%8)+1,wi=(i%4)+1,fr=5+(i*2),ad=3+(i*4);firf_habitat(lc,bl,bw,gi,wi,fr,ad);}
ps("\nFirefly feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i%2),bw=18+(i*2),gi=(i%7)+1,wi=(i%3)+1,fr=6+(i*2),ad=5+(i*3);firf_feeding(lc,bl,bw,gi,wi,fr,ad);}
ps("\nFirefly breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=9+(i%3),bw=12+(i*3),gi=(i%6)+1,wi=(i%5)+1,fr=4+(i*3),ad=7+(i*3);firf_breeding(lc,bl,bw,gi,wi,fr,ad);}
ps("\nFirefly display...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i%2),bw=20+(i*3),gi=(i%9)+1,wi=(i%6)+1,fr=8+(i*2),ad=2+(i*5);firf_display(lc,bl,bw,gi,wi,fr,ad);}
ps("\nFirefly market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=11+(i%2),bw=22+(i*2),gi=(i%4)+4,wi=(i%3)+3,fr=10+(i*2),ad=10+(i*3);firf_market(lc,bl,bw,gi,wi,fr,ad);}
ps("\n");firf_report();firf_state();ps("\n=== Demo Complete ===\n");return 0;}
