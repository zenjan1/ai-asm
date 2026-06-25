/* gazelle_admin: Gazelle (Gazella gazella) mountain swift antelope (v1.0)
 * Gazelle steppe, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, coat_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,rn_sp,ct_idx,age_yr,active;} gzl_t;
typedef struct{int n_steppe,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_rn,t_ct;} gzl_state_t;
static gzl_t stp[N],stf[N-2],stb[N-4],sth[N-6],stm[N-6]; static gzl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gzl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int ci,int ay){if(*cnt>=mx)return -1;gzl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->rn_sp=rs;x->ct_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GZL] Gazelle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gazelle_init(void){if(init)return -1;st.n_steppe=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_rn=0;st.t_ct=0;for(int i=0;i<N;i++)stp[i].active=0;for(int i=0;i<N-2;i++)stf[i].active=0;for(int i=0;i<N-4;i++)stb[i].active=0;for(int i=0;i<N-6;i++)sth[i].active=0;for(int i=0;i<N-6;i++)stm[i].active=0;init=1;ps("[GZL] Gazelle initialized\n");return 0;}
int gazelle_steppe(int lc,int bl,int bw,int hc,int rs,int ci,int ay){return add(stp,&st.n_steppe,&st.t_ln,N,lc,bl,bw,hc,rs,ci,ay);}
int gazelle_feeding(int lc,int bl,int bw,int hc,int rs,int ci,int ay){return add(stf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,ci,ay);}
int gazelle_breeding(int lc,int bl,int bw,int hc,int rs,int ci,int ay){return add(stb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,rs,ci,ay);}
int gazelle_health(int lc,int bl,int bw,int hc,int rs,int ci,int ay){return add(sth,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,ci,ay);}
int gazelle_market(int lc,int bl,int bw,int hc,int rs,int ci,int ay){return add(stm,&st.n_mkt,&st.t_ct,N-6,lc,bl,bw,hc,rs,ci,ay);}
void gazelle_report(void){ps("[GZL] Steppe: ");pi(st.n_steppe);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Ct=");pi(st.t_ct);ps("\n");}
void gazelle_state(void){ps("[GZL] Steppe=");pi(st.n_steppe);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gazelle Admin Demo ===\n\n");gazelle_init();
ps("Gazelle steppe...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*6),bw=25+(i*3),hc=20+(i*4),rs=18+(i*4),ci=(i%8)+1,ay=(i%12)+1;gazelle_steppe(lc,bl,bw,hc,rs,ci,ay);}
ps("\nGazelle feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=103+(i*5),bw=26+(i*2),hc=22+(i*3),rs=19+(i*3),ci=(i%7)+1,ay=(i%10)+1;gazelle_feeding(lc,bl,bw,hc,rs,ci,ay);}
ps("\nGazelle breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=106+(i*4),bw=27+(i*2),hc=24+(i*2),rs=17+(i*5),ci=(i%6)+1,ay=(i%8)+1;gazelle_breeding(lc,bl,bw,hc,rs,ci,ay);}
ps("\nGazelle health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*7),bw=24+(i*4),hc=18+(i*5),rs=16+(i*6),ci=(i%9)+1,ay=(i%7)+1;gazelle_health(lc,bl,bw,hc,rs,ci,ay);}
ps("\nGazelle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=108+(i*3),bw=28+(i*2),hc=26+(i*2),rs=20+(i*3),ci=(i%5)+1,ay=(i%6)+1;gazelle_market(lc,bl,bw,hc,rs,ci,ay);}
ps("\n");gazelle_report();gazelle_state();ps("\n=== Demo Complete ===\n");return 0;}
