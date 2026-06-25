/* crocodile_admin: Crocodile (Crocodylus niloticus) Nile large reptile (v1.0)
 * Crocodile river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tooth_cm, swim_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tooth_cm,sw_sp,sc_idx,age_yr,active;} crc_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tooth,t_sw,t_sc;} crc_state_t;
static crc_t rvl[N],rvf[N-2],rvb[N-4],rvh[N-6],rvm[N-6]; static crc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crc_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int si,int ay){if(*cnt>=mx)return -1;crc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tooth_cm=tc;x->sw_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CRC] Croc ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int crocodile_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tooth=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)rvl[i].active=0;for(int i=0;i<N-2;i++)rvf[i].active=0;for(int i=0;i<N-4;i++)rvb[i].active=0;for(int i=0;i<N-6;i++)rvh[i].active=0;for(int i=0;i<N-6;i++)rvm[i].active=0;init=1;ps("[CRC] Crocodile initialized\n");return 0;}
int crocodile_river(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(rvl,&st.n_river,&st.t_ln,N,lc,bl,bw,tc,ss,si,ay);}
int crocodile_feeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(rvf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,si,ay);}
int crocodile_breeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(rvb,&st.n_breed,&st.t_tooth,N-4,lc,bl,bw,tc,ss,si,ay);}
int crocodile_health(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(rvh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,si,ay);}
int crocodile_market(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(rvm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,tc,ss,si,ay);}
void crocodile_report(void){ps("[CRC] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tooth=");pi(st.t_tooth);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void crocodile_state(void){ps("[CRC] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crocodile Admin Demo ===\n\n");crocodile_init();
ps("Crocodile river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*15),bw=200+(i*50),tc=5+(i*2),ss=8+(i*3),si=(i%10)+1,ay=(i%25)+1;crocodile_river(lc,bl,bw,tc,ss,si,ay);}
ps("\nCrocodile feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=260+(i*12),bw=220+(i*40),tc=6+(i*1),ss=9+(i*2),si=(i%8)+1,ay=(i%20)+1;crocodile_feeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nCrocodile breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=270+(i*10),bw=240+(i*30),tc=7+(i%2),ss=7+(i*4),si=(i%6)+1,ay=(i%15)+1;crocodile_breeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nCrocodile health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=245+(i*18),bw=190+(i*60),tc=4+(i*3),ss=5+(i*5),si=(i%12)+1,ay=(i%12)+1;crocodile_health(lc,bl,bw,tc,ss,si,ay);}
ps("\nCrocodile market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=280+(i*8),bw=260+(i*25),tc=8+(i%2),ss=10+(i*2),si=(i%5)+1,ay=(i%10)+1;crocodile_market(lc,bl,bw,tc,ss,si,ay);}
ps("\n");crocodile_report();crocodile_state();ps("\n=== Demo Complete ===\n");return 0;}
