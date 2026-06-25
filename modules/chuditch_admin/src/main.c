/* chuditch_admin: Chuditch (Dasyurus geoffroii) western quoll (v1.0)
 * Chuditch forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, spot_ct, tail_cm, ch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,spot_ct,tail_cm,ch_idx,age_yr,active;} chud_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_spot,t_tail,t_ch;} chud_state_t;
static chud_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static chud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chud_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int tc,int ch,int ay){if(*cnt>=mx)return -1;chud_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->spot_ct=sc;x->tail_cm=tc;x->ch_idx=ch;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHUD] Chuditch ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" tc=");pi(tc);ps(" ch=");pi(ch);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chuditch_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_spot=0;st.t_tail=0;st.t_ch=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[CHUD] Chuditch initialized\n");return 0;}
int chuditch_forest(int lc,int bl,int bw,int sc,int tc,int ch,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,tc,ch,ay);}
int chuditch_feeding(int lc,int bl,int bw,int sc,int tc,int ch,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,tc,ch,ay);}
int chuditch_breeding(int lc,int bl,int bw,int sc,int tc,int ch,int ay){return add(svb,&st.n_breed,&st.t_spot,N-4,lc,bl,bw,sc,tc,ch,ay);}
int chuditch_health(int lc,int bl,int bw,int sc,int tc,int ch,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sc,tc,ch,ay);}
int chuditch_market(int lc,int bl,int bw,int sc,int tc,int ch,int ay){return add(svm,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,sc,tc,ch,ay);}
void chuditch_report(void){ps("[CHUD] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Spot=");pi(st.t_spot);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void chuditch_state(void){ps("[CHUD] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chuditch Admin Demo ===\n\n");chuditch_init();
ps("Chuditch forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=1200+(i*80),sc=15+(i%10),tc=30+(i*2),ch=(i%8)+1,ay=(i%5)+1;chuditch_forest(lc,bl,bw,sc,tc,ch,ay);}
ps("\nChuditch feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=1300+(i*60),sc=16+(i%10),tc=31+(i*2),ch=(i%6)+1,ay=(i%4)+1;chuditch_feeding(lc,bl,bw,sc,tc,ch,ay);}
ps("\nChuditch breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*2),bw=1400+(i*50),sc=18+(i%10),tc=32+(i*2),ch=(i%5)+1,ay=(i%3)+1;chuditch_breeding(lc,bl,bw,sc,tc,ch,ay);}
ps("\nChuditch health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=1100+(i*90),sc=12+(i%10),tc=28+(i*2),ch=(i%10)+1,ay=(i%5)+1;chuditch_health(lc,bl,bw,sc,tc,ch,ay);}
ps("\nChuditch market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=41+(i*2),bw=1500+(i*50),sc=20+(i%10),tc=33+(i*2),ch=(i%4)+1,ay=(i%3)+1;chuditch_market(lc,bl,bw,sc,tc,ch,ay);}
ps("\n");chuditch_report();chuditch_state();ps("\n=== Demo Complete ===\n");return 0;}
