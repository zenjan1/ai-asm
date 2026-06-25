/* kangaroo_admin: Kangaroo (Macropus rufus) red marsupial jumper (v1.0)
 * Kangaroo outback, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, jump_dist, pouch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,jp_dt,ph_idx,age_yr,active;} kang_t;
typedef struct{int n_outback,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_jp,t_ph;} kang_state_t;
static kang_t out[N],otf[N-2],otb[N-4],oth[N-6],otm[N-6]; static kang_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kang_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lg,int jd,int ph,int ay){if(*cnt>=mx)return -1;kang_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lg;x->jp_dt=jd;x->ph_idx=ph;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KANG] Kangaroo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lg=");pi(lg);ps(" jd=");pi(jd);ps(" ph=");pi(ph);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kangaroo_init(void){if(init)return -1;st.n_outback=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_jp=0;st.t_ph=0;for(int i=0;i<N;i++)out[i].active=0;for(int i=0;i<N-2;i++)otf[i].active=0;for(int i=0;i<N-4;i++)otb[i].active=0;for(int i=0;i<N-6;i++)oth[i].active=0;for(int i=0;i<N-6;i++)otm[i].active=0;init=1;ps("[KANG] Kangaroo initialized\n");return 0;}
int kangaroo_outback(int lc,int bl,int bw,int lg,int jd,int ph,int ay){return add(out,&st.n_outback,&st.t_ln,N,lc,bl,bw,lg,jd,ph,ay);}
int kangaroo_feeding(int lc,int bl,int bw,int lg,int jd,int ph,int ay){return add(otf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lg,jd,ph,ay);}
int kangaroo_breeding(int lc,int bl,int bw,int lg,int jd,int ph,int ay){return add(otb,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lg,jd,ph,ay);}
int kangaroo_health(int lc,int bl,int bw,int lg,int jd,int ph,int ay){return add(oth,&st.n_health,&st.t_jp,N-6,lc,bl,bw,lg,jd,ph,ay);}
int kangaroo_market(int lc,int bl,int bw,int lg,int jd,int ph,int ay){return add(otm,&st.n_mkt,&st.t_ph,N-6,lc,bl,bw,lg,jd,ph,ay);}
void kangaroo_report(void){ps("[KANG] Outback: ");pi(st.n_outback);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Ph=");pi(st.t_ph);ps("\n");}
void kangaroo_state(void){ps("[KANG] Outback=");pi(st.n_outback);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kangaroo Admin Demo ===\n\n");kangaroo_init();
ps("Kangaroo outback...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=140+(i*8),bw=60+(i*6),lg=80+(i*5),jd=300+(i*30),ph=(i%8)+1,ay=(i%15)+1;kangaroo_outback(lc,bl,bw,lg,jd,ph,ay);}
ps("\nKangaroo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=145+(i*6),bw=63+(i*5),lg=82+(i*4),jd=310+(i*25),ph=(i%7)+1,ay=(i%12)+1;kangaroo_feeding(lc,bl,bw,lg,jd,ph,ay);}
ps("\nKangaroo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=150+(i*5),bw=66+(i*4),lg=84+(i*3),jd=290+(i*35),ph=(i%6)+1,ay=(i%10)+1;kangaroo_breeding(lc,bl,bw,lg,jd,ph,ay);}
ps("\nKangaroo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=138+(i*9),bw=58+(i*7),lg=78+(i*6),jd=280+(i*40),ph=(i%9)+1,ay=(i%8)+1;kangaroo_health(lc,bl,bw,lg,jd,ph,ay);}
ps("\nKangaroo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=155+(i*4),bw=68+(i*4),lg=86+(i*3),jd=320+(i*20),ph=(i%5)+1,ay=(i%7)+1;kangaroo_market(lc,bl,bw,lg,jd,ph,ay);}
ps("\n");kangaroo_report();kangaroo_state();ps("\n=== Demo Complete ===\n");return 0;}
