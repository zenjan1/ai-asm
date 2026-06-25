/* chiru_admin: Chiru (Pantholops hodgsonii) Tibetan plateau antelope (v1.0)
 * Chiru plateau, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, gallop_speed, ch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,gl_sp,ch_idx,age_yr,active;} chir_t;
typedef struct{int n_plateau,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_gl,t_ch;} chir_state_t;
static chir_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static chir_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chir_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int gs,int ci,int ay){if(*cnt>=mx)return -1;chir_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->gl_sp=gs;x->ch_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHIR] Chiru ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" gs=");pi(gs);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chiru_init(void){if(init)return -1;st.n_plateau=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_gl=0;st.t_ch=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[CHIR] Chiru initialized\n");return 0;}
int chiru_plateau(int lc,int bl,int bw,int hc,int gs,int ci,int ay){return add(svl,&st.n_plateau,&st.t_ln,N,lc,bl,bw,hc,gs,ci,ay);}
int chiru_feeding(int lc,int bl,int bw,int hc,int gs,int ci,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,gs,ci,ay);}
int chiru_breeding(int lc,int bl,int bw,int hc,int gs,int ci,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,gs,ci,ay);}
int chiru_health(int lc,int bl,int bw,int hc,int gs,int ci,int ay){return add(svh,&st.n_health,&st.t_gl,N-6,lc,bl,bw,hc,gs,ci,ay);}
int chiru_market(int lc,int bl,int bw,int hc,int gs,int ci,int ay){return add(svm,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,hc,gs,ci,ay);}
void chiru_report(void){ps("[CHIR] Plateau: ");pi(st.n_plateau);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Gl=");pi(st.t_gl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void chiru_state(void){ps("[CHIR] Plateau=");pi(st.n_plateau);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chiru Admin Demo ===\n\n");chiru_init();
ps("Chiru plateau...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*5),bw=35+(i*4),hc=30+(i*2),gs=25+(i*3),ci=(i%8)+1,ay=(i%12)+1;chiru_plateau(lc,bl,bw,hc,gs,ci,ay);}
ps("\nChiru feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=123+(i*4),bw=37+(i*3),hc=31+(i*2),gs=27+(i*2),ci=(i%6)+1,ay=(i%10)+1;chiru_feeding(lc,bl,bw,hc,gs,ci,ay);}
ps("\nChiru breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=126+(i*3),bw=39+(i*3),hc=33+(i*2),gs=23+(i*3),ci=(i%5)+1,ay=(i%8)+1;chiru_breeding(lc,bl,bw,hc,gs,ci,ay);}
ps("\nChiru health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*7),bw=33+(i*5),hc=28+(i*3),gs=21+(i*4),ci=(i%10)+1,ay=(i%7)+1;chiru_health(lc,bl,bw,hc,gs,ci,ay);}
ps("\nChiru market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=130+(i*2),bw=41+(i*2),hc=35+(i*1),gs=29+(i*2),ci=(i%4)+1,ay=(i%6)+1;chiru_market(lc,bl,bw,hc,gs,ci,ay);}
ps("\n");chiru_report();chiru_state();ps("\n=== Demo Complete ===\n");return 0;}
