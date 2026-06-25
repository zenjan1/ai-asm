/* urchin_admin: Urchin (Paracentrotus lividus) purple sea urchin marine invertebrate (v1.0)
 * Urchin seabed, feeding, breeding, health, market
 * Features: body_diam_cm, body_wt_g, spine_cm, swim_speed, sp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bd_dm,bdy_wt,spn_cm,sw_sp,sp_idx,age_yr,active;} urch_t;
typedef struct{int n_seabed,n_feed,n_breed,n_health,n_mkt,t_dm,t_wt,t_spn,t_sw,t_sp;} urch_state_t;
static urch_t arr_s[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static urch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(urch_t*a,int*cnt,int*sum,int mx,int lc,int dm,int bw,int snc,int ss,int spi,int ay){if(*cnt>=mx)return -1;urch_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bd_dm=dm;x->bdy_wt=bw;x->spn_cm=snc;x->sw_sp=ss;x->sp_idx=spi;x->age_yr=ay;x->active=1;*sum+=dm;(*cnt)++;ps("[URCH] Urchin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" dm=");pi(dm);ps(" bw=");pi(bw);ps(" snc=");pi(snc);ps(" ss=");pi(ss);ps(" spi=");pi(spi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int urchin_init(void){if(init)return -1;st.n_seabed=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_dm=0;st.t_wt=0;st.t_spn=0;st.t_sw=0;st.t_sp=0;for(int i=0;i<N;i++)arr_s[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[URCH] Urchin initialized\n");return 0;}
int urchin_seabed(int lc,int dm,int bw,int snc,int ss,int spi,int ay){return add(arr_s,&st.n_seabed,&st.t_dm,N,lc,dm,bw,snc,ss,spi,ay);}
int urchin_feeding(int lc,int dm,int bw,int snc,int ss,int spi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,dm,bw,snc,ss,spi,ay);}
int urchin_breeding(int lc,int dm,int bw,int snc,int ss,int spi,int ay){return add(arr_b,&st.n_breed,&st.t_spn,N-4,lc,dm,bw,snc,ss,spi,ay);}
int urchin_health(int lc,int dm,int bw,int snc,int ss,int spi,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,dm,bw,snc,ss,spi,ay);}
int urchin_market(int lc,int dm,int bw,int snc,int ss,int spi,int ay){return add(arr_m,&st.n_mkt,&st.t_sp,N-6,lc,dm,bw,snc,ss,spi,ay);}
void urchin_report(void){ps("[URCH] Seabed: ");pi(st.n_seabed);ps(" Dm=");pi(st.t_dm);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Spn=");pi(st.t_spn);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void urchin_state(void){ps("[URCH] Seabed=");pi(st.n_seabed);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Urchin Admin Demo ===\n\n");urchin_init();
ps("Urchin seabed...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,dm=5+(i*1),bw=50+(i*8),snc=2+(i*1),ss=0+(i%2),spi=(i%6)+1,ay=(i%5)+1;urchin_seabed(lc,dm,bw,snc,ss,spi,ay);}
ps("\nUrchin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,dm=6+(i*1),bw=55+(i*6),snc=2+(i*1),ss=0+(i%2),spi=(i%5)+1,ay=(i%4)+1;urchin_feeding(lc,dm,bw,snc,ss,spi,ay);}
ps("\nUrchin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,dm=7+(i*1),bw=60+(i*5),snc=3+(i*1),ss=0+(i%2),spi=(i%4)+1,ay=(i%3)+1;urchin_breeding(lc,dm,bw,snc,ss,spi,ay);}
ps("\nUrchin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,dm=4+(i*1),bw=45+(i*9),snc=1+(i*1),ss=0+(i%2),spi=(i%7)+1,ay=(i%3)+1;urchin_health(lc,dm,bw,snc,ss,spi,ay);}
ps("\nUrchin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,dm=8+(i*1),bw=65+(i*4),snc=3+(i*1),ss=0+(i%2),spi=(i%3)+1,ay=(i%2)+1;urchin_market(lc,dm,bw,snc,ss,spi,ay);}
ps("\n");urchin_report();urchin_state();ps("\n=== Demo Complete ===\n");return 0;}
