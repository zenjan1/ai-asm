/* takhi_admin: Takhi (Equus ferus przewalskii) wild horse species (v1.0)
 * Takhi grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, mane_cm, gallop_speed, tk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,mn_cm,gl_sp,tk_idx,age_yr,active;} takh_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_mn,t_gl,t_tk;} takh_state_t;
static takh_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static takh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(takh_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int mc,int gs,int ti,int ay){if(*cnt>=mx)return -1;takh_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->mn_cm=mc;x->gl_sp=gs;x->tk_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TAKH] Takhi ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" mc=");pi(mc);ps(" gs=");pi(gs);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int takhi_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_mn=0;st.t_gl=0;st.t_tk=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TAKH] Takhi initialized\n");return 0;}
int takhi_grassland(int lc,int bl,int bw,int mc,int gs,int ti,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,mc,gs,ti,ay);}
int takhi_feeding(int lc,int bl,int bw,int mc,int gs,int ti,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,mc,gs,ti,ay);}
int takhi_breeding(int lc,int bl,int bw,int mc,int gs,int ti,int ay){return add(svb,&st.n_breed,&st.t_mn,N-4,lc,bl,bw,mc,gs,ti,ay);}
int takhi_health(int lc,int bl,int bw,int mc,int gs,int ti,int ay){return add(svh,&st.n_health,&st.t_gl,N-6,lc,bl,bw,mc,gs,ti,ay);}
int takhi_market(int lc,int bl,int bw,int mc,int gs,int ti,int ay){return add(svm,&st.n_mkt,&st.t_tk,N-6,lc,bl,bw,mc,gs,ti,ay);}
void takhi_report(void){ps("[TAKH] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Mn=");pi(st.t_mn);ps("\nHealth: ");pi(st.n_health);ps(" Gl=");pi(st.t_gl);ps("\nMkt: ");pi(st.n_mkt);ps(" Tk=");pi(st.t_tk);ps("\n");}
void takhi_state(void){ps("[TAKH] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Takhi Admin Demo ===\n\n");takhi_init();
ps("Takhi grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=250+(i*20),mc=15+(i*2),gs=35+(i*3),ti=(i%8)+1,ay=(i%20)+1;takhi_grassland(lc,bl,bw,mc,gs,ti,ay);}
ps("\nTakhi feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=260+(i*16),mc=16+(i*1),gs=37+(i*2),ti=(i%6)+1,ay=(i%18)+1;takhi_feeding(lc,bl,bw,mc,gs,ti,ay);}
ps("\nTakhi breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=270+(i*12),mc=17+(i*2),gs=33+(i*3),ti=(i%5)+1,ay=(i%15)+1;takhi_breeding(lc,bl,bw,mc,gs,ti,ay);}
ps("\nTakhi health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=240+(i*25),mc=14+(i*3),gs=30+(i*4),ti=(i%10)+1,ay=(i%12)+1;takhi_health(lc,bl,bw,mc,gs,ti,ay);}
ps("\nTakhi market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=280+(i*10),mc=18+(i*1),gs=40+(i*2),ti=(i%4)+1,ay=(i%10)+1;takhi_market(lc,bl,bw,mc,gs,ti,ay);}
ps("\n");takhi_report();takhi_state();ps("\n=== Demo Complete ===\n");return 0;}
