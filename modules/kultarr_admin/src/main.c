/* kultarr_admin: Kultarr (Antechinomys) tiny dasyurid marsupials (v1.0)
 * Kultarr forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fur_len, tail_cm, kt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fur_len,tail_cm,kt_idx,age_yr,active;} ktar_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fur,t_tail,t_kt;} ktar_state_t;
static ktar_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ktar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ktar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fl,int tc,int kt,int ay){if(*cnt>=mx)return -1;ktar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fur_len=fl;x->tail_cm=tc;x->kt_idx=kt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KTAR] Kultarr ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fl=");pi(fl);ps(" tc=");pi(tc);ps(" kt=");pi(kt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kultarr_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fur=0;st.t_tail=0;st.t_kt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KTAR] Kultarr initialized\n");return 0;}
int kultarr_forest(int lc,int bl,int bw,int fl,int tc,int kt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,fl,tc,kt,ay);}
int kultarr_feeding(int lc,int bl,int bw,int fl,int tc,int kt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fl,tc,kt,ay);}
int kultarr_breeding(int lc,int bl,int bw,int fl,int tc,int kt,int ay){return add(svb,&st.n_breed,&st.t_fur,N-4,lc,bl,bw,fl,tc,kt,ay);}
int kultarr_health(int lc,int bl,int bw,int fl,int tc,int kt,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,fl,tc,kt,ay);}
int kultarr_market(int lc,int bl,int bw,int fl,int tc,int kt,int ay){return add(svm,&st.n_mkt,&st.t_kt,N-6,lc,bl,bw,fl,tc,kt,ay);}
void kultarr_report(void){ps("[KTAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fur=");pi(st.t_fur);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Kt=");pi(st.t_kt);ps("\n");}
void kultarr_state(void){ps("[KTAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kultarr Admin Demo ===\n\n");kultarr_init();
ps("Kultarr forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=15+(i*2),fl=3+(i%4),tc=9+(i*1),kt=(i%8)+1,ay=(i%5)+1;kultarr_forest(lc,bl,bw,fl,tc,kt,ay);}
ps("\nKultarr feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=17+(i*2),fl=3+(i%4),tc=10+(i*1),kt=(i%6)+1,ay=(i%4)+1;kultarr_feeding(lc,bl,bw,fl,tc,kt,ay);}
ps("\nKultarr breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=19+(i*2),fl=4+(i%4),tc=10+(i*1),kt=(i%5)+1,ay=(i%3)+1;kultarr_breeding(lc,bl,bw,fl,tc,kt,ay);}
ps("\nKultarr health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*2),bw=13+(i*3),fl=2+(i%4),tc=8+(i*1),kt=(i%10)+1,ay=(i%5)+1;kultarr_health(lc,bl,bw,fl,tc,kt,ay);}
ps("\nKultarr market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=21+(i*2),fl=4+(i%4),tc=11+(i*1),kt=(i%4)+1,ay=(i%3)+1;kultarr_market(lc,bl,bw,fl,tc,kt,ay);}
ps("\n");kultarr_report();kultarr_state();ps("\n=== Demo Complete ===\n");return 0;}
