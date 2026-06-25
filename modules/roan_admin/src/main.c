/* roan_admin: Roan (Hippotragus equinus) large horse-like antelope (v1.0)
 * Roan grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, gallop_speed, ro_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,gl_sp,ro_idx,age_yr,active;} roan_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_gl,t_ro;} roan_state_t;
static roan_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static roan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(roan_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int gs,int ri,int ay){if(*cnt>=mx)return -1;roan_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->gl_sp=gs;x->ro_idx=ri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ROAN] Roan ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" gs=");pi(gs);ps(" ri=");pi(ri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int roan_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_gl=0;st.t_ro=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ROAN] Roan initialized\n");return 0;}
int roan_grassland(int lc,int bl,int bw,int hc,int gs,int ri,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,hc,gs,ri,ay);}
int roan_feeding(int lc,int bl,int bw,int hc,int gs,int ri,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,gs,ri,ay);}
int roan_breeding(int lc,int bl,int bw,int hc,int gs,int ri,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,gs,ri,ay);}
int roan_health(int lc,int bl,int bw,int hc,int gs,int ri,int ay){return add(svh,&st.n_health,&st.t_gl,N-6,lc,bl,bw,hc,gs,ri,ay);}
int roan_market(int lc,int bl,int bw,int hc,int gs,int ri,int ay){return add(svm,&st.n_mkt,&st.t_ro,N-6,lc,bl,bw,hc,gs,ri,ay);}
void roan_report(void){ps("[ROAN] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Gl=");pi(st.t_gl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ro=");pi(st.t_ro);ps("\n");}
void roan_state(void){ps("[ROAN] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Roan Admin Demo ===\n\n");roan_init();
ps("Roan grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=210+(i*10),bw=220+(i*18),hc=95+(i*5),gs=30+(i*3),ri=(i%8)+1,ay=(i%15)+1;roan_grassland(lc,bl,bw,hc,gs,ri,ay);}
ps("\nRoan feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=215+(i*8),bw=225+(i*15),hc=98+(i*4),gs=32+(i*2),ri=(i%6)+1,ay=(i%12)+1;roan_feeding(lc,bl,bw,hc,gs,ri,ay);}
ps("\nRoan breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=230+(i*12),hc=100+(i*4),gs=28+(i*3),ri=(i%5)+1,ay=(i%10)+1;roan_breeding(lc,bl,bw,hc,gs,ri,ay);}
ps("\nRoan health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=205+(i*12),bw=215+(i*22),hc=92+(i*6),gs=26+(i*4),ri=(i%10)+1,ay=(i%8)+1;roan_health(lc,bl,bw,hc,gs,ri,ay);}
ps("\nRoan market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=228+(i*5),bw=238+(i*10),hc=105+(i*3),gs=34+(i*2),ri=(i%4)+1,ay=(i%7)+1;roan_market(lc,bl,bw,hc,gs,ri,ay);}
ps("\n");roan_report();roan_state();ps("\n=== Demo Complete ===\n");return 0;}
