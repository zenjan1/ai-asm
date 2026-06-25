/* jaguar_admin: Jaguar (Panthera onca) big cat predator (v1.0)
 * Jaguar jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, run_speed, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,rn_sp,sp_idx,age_yr,active;} jag_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_rn,t_sp;} jag_state_t;
static jag_t jng[N],jnf[N-2],jnb[N-4],jnh[N-6],jnm[N-6]; static jag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jag_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int rs,int si,int ay){if(*cnt>=mx)return -1;jag_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->rn_sp=rs;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[JAG] Jaguar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int jaguar_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_rn=0;st.t_sp=0;for(int i=0;i<N;i++)jng[i].active=0;for(int i=0;i<N-2;i++)jnf[i].active=0;for(int i=0;i<N-4;i++)jnb[i].active=0;for(int i=0;i<N-6;i++)jnh[i].active=0;for(int i=0;i<N-6;i++)jnm[i].active=0;init=1;ps("[JAG] Jaguar initialized\n");return 0;}
int jaguar_jungle(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(jng,&st.n_jungle,&st.t_ln,N,lc,bl,bw,cc,rs,si,ay);}
int jaguar_feeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(jnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,rs,si,ay);}
int jaguar_breeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(jnb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,rs,si,ay);}
int jaguar_health(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(jnh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,cc,rs,si,ay);}
int jaguar_market(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(jnm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,cc,rs,si,ay);}
void jaguar_report(void){ps("[JAG] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void jaguar_state(void){ps("[JAG] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jaguar Admin Demo ===\n\n");jaguar_init();
ps("Jaguar jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*7),bw=70+(i*8),cc=6+(i*1),rs=16+(i*3),si=(i%10)+1,ay=(i%18)+1;jaguar_jungle(lc,bl,bw,cc,rs,si,ay);}
ps("\nJaguar feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=153+(i*6),bw=73+(i*6),cc=6+(i*1),rs=17+(i*2),si=(i%9)+1,ay=(i%15)+1;jaguar_feeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nJaguar breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=156+(i*5),bw=76+(i*5),cc=7+(i*1),rs=15+(i*4),si=(i%8)+1,ay=(i%12)+1;jaguar_breeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nJaguar health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=148+(i*8),bw=68+(i*10),cc=5+(i*2),rs=14+(i*5),si=(i%11)+1,ay=(i%10)+1;jaguar_health(lc,bl,bw,cc,rs,si,ay);}
ps("\nJaguar market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=158+(i*4),bw=78+(i*4),cc=7+(i*1),rs=18+(i*2),si=(i%6)+1,ay=(i%8)+1;jaguar_market(lc,bl,bw,cc,rs,si,ay);}
ps("\n");jaguar_report();jaguar_state();ps("\n=== Demo Complete ===\n");return 0;}
