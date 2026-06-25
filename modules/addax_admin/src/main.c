/* addax_admin: Addax (Addax nasomaculatus) spiral-horned desert antelope (v1.0)
 * Addax desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, ad_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,ad_idx,age_yr,active;} addx_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_ad;} addx_state_t;
static addx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static addx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(addx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int ai,int ay){if(*cnt>=mx)return -1;addx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->ad_idx=ai;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ADDX] Addax ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" ai=");pi(ai);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int addax_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_ad=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ADDX] Addax initialized\n");return 0;}
int addax_desert(int lc,int bl,int bw,int hc,int rs,int ai,int ay){return add(svl,&st.n_desert,&st.t_ln,N,lc,bl,bw,hc,rs,ai,ay);}
int addax_feeding(int lc,int bl,int bw,int hc,int rs,int ai,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,ai,ay);}
int addax_breeding(int lc,int bl,int bw,int hc,int rs,int ai,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,ai,ay);}
int addax_health(int lc,int bl,int bw,int hc,int rs,int ai,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,ai,ay);}
int addax_market(int lc,int bl,int bw,int hc,int rs,int ai,int ay){return add(svm,&st.n_mkt,&st.t_ad,N-6,lc,bl,bw,hc,rs,ai,ay);}
void addax_report(void){ps("[ADDX] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Ad=");pi(st.t_ad);ps("\n");}
void addax_state(void){ps("[ADDX] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Addax Admin Demo ===\n\n");addax_init();
ps("Addax desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*6),bw=70+(i*8),hc=65+(i*4),rs=18+(i*2),ai=(i%8)+1,ay=(i%12)+1;addax_desert(lc,bl,bw,hc,rs,ai,ay);}
ps("\nAddax feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=133+(i*5),bw=73+(i*6),hc=67+(i*3),rs=19+(i*2),ai=(i%6)+1,ay=(i%10)+1;addax_feeding(lc,bl,bw,hc,rs,ai,ay);}
ps("\nAddax breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=136+(i*4),bw=76+(i*5),hc=70+(i*3),rs=17+(i*2),ai=(i%5)+1,ay=(i%8)+1;addax_breeding(lc,bl,bw,hc,rs,ai,ay);}
ps("\nAddax health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=128+(i*8),bw=68+(i*10),hc=63+(i*5),rs=16+(i*3),ai=(i%10)+1,ay=(i%7)+1;addax_health(lc,bl,bw,hc,rs,ai,ay);}
ps("\nAddax market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=140+(i*3),bw=78+(i*4),hc=72+(i*2),rs=21+(i*1),ai=(i%4)+1,ay=(i%6)+1;addax_market(lc,bl,bw,hc,rs,ai,ay);}
ps("\n");addax_report();addax_state();ps("\n=== Demo Complete ===\n");return 0;}
