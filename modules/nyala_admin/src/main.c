/* nyala_admin: Nyala (Tragelaphus angasii) bushveld antelope (v1.0)
 * Nyala bushveld, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, ny_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,ny_idx,age_yr,active;} nyala_t;
typedef struct{int n_bush,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_ny;} nyala_state_t;
static nyala_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static nyala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nyala_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int ni,int ay){if(*cnt>=mx)return -1;nyala_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->ny_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NYALA] Nyala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int nyala_init(void){if(init)return -1;st.n_bush=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_ny=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[NYALA] Nyala initialized\n");return 0;}
int nyala_bushveld(int lc,int bl,int bw,int hc,int rs,int ni,int ay){return add(svl,&st.n_bush,&st.t_ln,N,lc,bl,bw,hc,rs,ni,ay);}
int nyala_feeding(int lc,int bl,int bw,int hc,int rs,int ni,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,ni,ay);}
int nyala_breeding(int lc,int bl,int bw,int hc,int rs,int ni,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,ni,ay);}
int nyala_health(int lc,int bl,int bw,int hc,int rs,int ni,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,ni,ay);}
int nyala_market(int lc,int bl,int bw,int hc,int rs,int ni,int ay){return add(svm,&st.n_mkt,&st.t_ny,N-6,lc,bl,bw,hc,rs,ni,ay);}
void nyala_report(void){ps("[NYALA] Bush: ");pi(st.n_bush);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Ny=");pi(st.t_ny);ps("\n");}
void nyala_state(void){ps("[NYALA] Bush=");pi(st.n_bush);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Nyala Admin Demo ===\n\n");nyala_init();
ps("Nyala bushveld...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=160+(i*8),bw=100+(i*12),hc=60+(i*4),rs=22+(i*3),ni=(i%8)+1,ay=(i%14)+1;nyala_bushveld(lc,bl,bw,hc,rs,ni,ay);}
ps("\nNyala feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=165+(i*6),bw=105+(i*10),hc=62+(i*3),rs=24+(i*2),ni=(i%6)+1,ay=(i%12)+1;nyala_feeding(lc,bl,bw,hc,rs,ni,ay);}
ps("\nNyala breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=170+(i*5),bw=110+(i*8),hc=65+(i*3),rs=21+(i*4),ni=(i%5)+1,ay=(i%10)+1;nyala_breeding(lc,bl,bw,hc,rs,ni,ay);}
ps("\nNyala health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=155+(i*10),bw=95+(i*15),hc=58+(i*5),rs=19+(i*4),ni=(i%10)+1,ay=(i%8)+1;nyala_health(lc,bl,bw,hc,rs,ni,ay);}
ps("\nNyala market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=175+(i*4),bw=115+(i*6),hc=68+(i*2),rs=26+(i*2),ni=(i%4)+1,ay=(i%7)+1;nyala_market(lc,bl,bw,hc,rs,ni,ay);}
ps("\n");nyala_report();nyala_state();ps("\n=== Demo Complete ===\n");return 0;}
