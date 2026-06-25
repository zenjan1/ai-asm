/* saiga_admin: Saiga (Saiga tatarica) critically endangered steppe antelope (v1.0)
 * Saiga steppe, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, sg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,sg_idx,age_yr,active;} saig_t;
typedef struct{int n_steppe,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_sg;} saig_state_t;
static saig_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static saig_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(saig_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int si,int ay){if(*cnt>=mx)return -1;saig_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->sg_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SAIG] Saiga ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int saiga_init(void){if(init)return -1;st.n_steppe=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_sg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SAIG] Saiga initialized\n");return 0;}
int saiga_steppe(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(svl,&st.n_steppe,&st.t_ln,N,lc,bl,bw,hc,rs,si,ay);}
int saiga_feeding(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,si,ay);}
int saiga_breeding(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,si,ay);}
int saiga_health(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,si,ay);}
int saiga_market(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(svm,&st.n_mkt,&st.t_sg,N-6,lc,bl,bw,hc,rs,si,ay);}
void saiga_report(void){ps("[SAIG] Steppe: ");pi(st.n_steppe);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Sg=");pi(st.t_sg);ps("\n");}
void saiga_state(void){ps("[SAIG] Steppe=");pi(st.n_steppe);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Saiga Admin Demo ===\n\n");saiga_init();
ps("Saiga steppe...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=110+(i*5),bw=30+(i*4),hc=20+(i*2),rs=28+(i*3),si=(i%8)+1,ay=(i%10)+1;saiga_steppe(lc,bl,bw,hc,rs,si,ay);}
ps("\nSaiga feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=113+(i*4),bw=32+(i*3),hc=21+(i*2),rs=30+(i*2),si=(i%6)+1,ay=(i%9)+1;saiga_feeding(lc,bl,bw,hc,rs,si,ay);}
ps("\nSaiga breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=116+(i*3),bw=34+(i*3),hc=23+(i*2),rs=26+(i*3),si=(i%5)+1,ay=(i%8)+1;saiga_breeding(lc,bl,bw,hc,rs,si,ay);}
ps("\nSaiga health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=108+(i*7),bw=28+(i*5),hc=18+(i*3),rs=24+(i*4),si=(i%10)+1,ay=(i%6)+1;saiga_health(lc,bl,bw,hc,rs,si,ay);}
ps("\nSaiga market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=120+(i*2),bw=36+(i*2),hc=25+(i*1),rs=32+(i*2),si=(i%4)+1,ay=(i%5)+1;saiga_market(lc,bl,bw,hc,rs,si,ay);}
ps("\n");saiga_report();saiga_state();ps("\n=== Demo Complete ===\n");return 0;}
