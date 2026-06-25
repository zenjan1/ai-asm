/* mulgara_admin: Mulgara (Dasycercus) small carnivorous marsupials (v1.0)
 * Mulgara forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tooth_ct, tail_cm, mg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tooth_ct,tail_cm,mg_idx,age_yr,active;} mgar_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tooth,t_tail,t_mg;} mgar_state_t;
static mgar_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static mgar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mgar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tt,int tc,int mg,int ay){if(*cnt>=mx)return -1;mgar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tooth_ct=tt;x->tail_cm=tc;x->mg_idx=mg;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MGAR] Mulgara ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tt=");pi(tt);ps(" tc=");pi(tc);ps(" mg=");pi(mg);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mulgara_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tooth=0;st.t_tail=0;st.t_mg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[MGAR] Mulgara initialized\n");return 0;}
int mulgara_forest(int lc,int bl,int bw,int tt,int tc,int mg,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tt,tc,mg,ay);}
int mulgara_feeding(int lc,int bl,int bw,int tt,int tc,int mg,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tt,tc,mg,ay);}
int mulgara_breeding(int lc,int bl,int bw,int tt,int tc,int mg,int ay){return add(svb,&st.n_breed,&st.t_tooth,N-4,lc,bl,bw,tt,tc,mg,ay);}
int mulgara_health(int lc,int bl,int bw,int tt,int tc,int mg,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,tt,tc,mg,ay);}
int mulgara_market(int lc,int bl,int bw,int tt,int tc,int mg,int ay){return add(svm,&st.n_mkt,&st.t_mg,N-6,lc,bl,bw,tt,tc,mg,ay);}
void mulgara_report(void){ps("[MGAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tooth=");pi(st.t_tooth);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Mg=");pi(st.t_mg);ps("\n");}
void mulgara_state(void){ps("[MGAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mulgara Admin Demo ===\n\n");mulgara_init();
ps("Mulgara forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*1),bw=40+(i*5),tt=30+(i%8),tc=12+(i*1),mg=(i%8)+1,ay=(i%5)+1;mulgara_forest(lc,bl,bw,tt,tc,mg,ay);}
ps("\nMulgara feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i*1),bw=45+(i*4),tt=32+(i%8),tc=13+(i*1),mg=(i%6)+1,ay=(i%4)+1;mulgara_feeding(lc,bl,bw,tt,tc,mg,ay);}
ps("\nMulgara breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i*1),bw=50+(i*3),tt=34+(i%8),tc=14+(i*1),mg=(i%5)+1,ay=(i%3)+1;mulgara_breeding(lc,bl,bw,tt,tc,mg,ay);}
ps("\nMulgara health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*2),bw=35+(i*6),tt=28+(i%8),tc=11+(i*1),mg=(i%10)+1,ay=(i%5)+1;mulgara_health(lc,bl,bw,tt,tc,mg,ay);}
ps("\nMulgara market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i*1),bw=55+(i*3),tt=36+(i%8),tc=15+(i*1),mg=(i%4)+1,ay=(i%3)+1;mulgara_market(lc,bl,bw,tt,tc,mg,ay);}
ps("\n");mulgara_report();mulgara_state();ps("\n=== Demo Complete ===\n");return 0;}
