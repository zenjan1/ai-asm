/* duiker_admin: Duiker (Sylvicapra grimmia) small forest antelope (v1.0)
 * Duiker forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, dk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,dk_idx,age_yr,active;} dukr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_dk;} dukr_state_t;
static dukr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static dukr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dukr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int di,int ay){if(*cnt>=mx)return -1;dukr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->dk_idx=di;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DUKR] Duiker ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" di=");pi(di);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int duiker_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_dk=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[DUKR] Duiker initialized\n");return 0;}
int duiker_forest(int lc,int bl,int bw,int hc,int rs,int di,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,hc,rs,di,ay);}
int duiker_feeding(int lc,int bl,int bw,int hc,int rs,int di,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,di,ay);}
int duiker_breeding(int lc,int bl,int bw,int hc,int rs,int di,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,di,ay);}
int duiker_health(int lc,int bl,int bw,int hc,int rs,int di,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,di,ay);}
int duiker_market(int lc,int bl,int bw,int hc,int rs,int di,int ay){return add(svm,&st.n_mkt,&st.t_dk,N-6,lc,bl,bw,hc,rs,di,ay);}
void duiker_report(void){ps("[DUKR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Dk=");pi(st.t_dk);ps("\n");}
void duiker_state(void){ps("[DUKR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Duiker Admin Demo ===\n\n");duiker_init();
ps("Duiker forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*3),bw=8+(i*1),hc=12+(i*1),rs=18+(i*2),di=(i%8)+1,ay=(i%10)+1;duiker_forest(lc,bl,bw,hc,rs,di,ay);}
ps("\nDuiker feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*2),bw=9+(i*1),hc=13+(i*1),rs=19+(i*1),di=(i%6)+1,ay=(i%8)+1;duiker_feeding(lc,bl,bw,hc,rs,di,ay);}
ps("\nDuiker breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=59+(i*2),bw=10+(i*1),hc=14+(i*1),rs=17+(i*2),di=(i%5)+1,ay=(i%7)+1;duiker_breeding(lc,bl,bw,hc,rs,di,ay);}
ps("\nDuiker health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=53+(i*4),bw=7+(i*2),hc=11+(i*1),rs=16+(i*3),di=(i%10)+1,ay=(i%6)+1;duiker_health(lc,bl,bw,hc,rs,di,ay);}
ps("\nDuiker market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=61+(i*1),bw=11+(i*1),hc=15+(i*1),rs=21+(i*1),di=(i%4)+1,ay=(i%5)+1;duiker_market(lc,bl,bw,hc,rs,di,ay);}
ps("\n");duiker_report();duiker_state();ps("\n=== Demo Complete ===\n");return 0;}
