/* goat_admin: Goat (Capra aegagrus hircus) domesticated horned mammal (v1.0)
 * Goat mountain, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, climb_speed, beard_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,cl_sp,bd_idx,age_yr,active;} goat_t;
typedef struct{int n_mountain,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_cl,t_bd;} goat_state_t;
static goat_t mtn[N],mtf[N-2],mtb[N-4],mth[N-6],mtm[N-6]; static goat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(goat_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int cs,int bi,int ay){if(*cnt>=mx)return -1;goat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->cl_sp=cs;x->bd_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GOAT] Goat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" cs=");pi(cs);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int goat_init(void){if(init)return -1;st.n_mountain=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_cl=0;st.t_bd=0;for(int i=0;i<N;i++)mtn[i].active=0;for(int i=0;i<N-2;i++)mtf[i].active=0;for(int i=0;i<N-4;i++)mtb[i].active=0;for(int i=0;i<N-6;i++)mth[i].active=0;for(int i=0;i<N-6;i++)mtm[i].active=0;init=1;ps("[GOAT] Goat initialized\n");return 0;}
int goat_mountain(int lc,int bl,int bw,int hc,int cs,int bi,int ay){return add(mtn,&st.n_mountain,&st.t_ln,N,lc,bl,bw,hc,cs,bi,ay);}
int goat_feeding(int lc,int bl,int bw,int hc,int cs,int bi,int ay){return add(mtf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,cs,bi,ay);}
int goat_breeding(int lc,int bl,int bw,int hc,int cs,int bi,int ay){return add(mtb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,cs,bi,ay);}
int goat_health(int lc,int bl,int bw,int hc,int cs,int bi,int ay){return add(mth,&st.n_health,&st.t_cl,N-6,lc,bl,bw,hc,cs,bi,ay);}
int goat_market(int lc,int bl,int bw,int hc,int cs,int bi,int ay){return add(mtm,&st.n_mkt,&st.t_bd,N-6,lc,bl,bw,hc,cs,bi,ay);}
void goat_report(void){ps("[GOAT] Mountain: ");pi(st.n_mountain);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Bd=");pi(st.t_bd);ps("\n");}
void goat_state(void){ps("[GOAT] Mountain=");pi(st.n_mountain);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Goat Admin Demo ===\n\n");goat_init();
ps("Goat mountain...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*5),bw=45+(i*5),hc=15+(i*3),cs=5+(i*2),bi=(i%8)+1,ay=(i%12)+1;goat_mountain(lc,bl,bw,hc,cs,bi,ay);}
ps("\nGoat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=93+(i*4),bw=47+(i*4),hc=16+(i*2),cs=6+(i*2),bi=(i%7)+1,ay=(i%10)+1;goat_feeding(lc,bl,bw,hc,cs,bi,ay);}
ps("\nGoat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=96+(i*3),bw=49+(i*3),hc=17+(i*2),cs=4+(i*3),bi=(i%6)+1,ay=(i%8)+1;goat_breeding(lc,bl,bw,hc,cs,bi,ay);}
ps("\nGoat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=88+(i*6),bw=43+(i*6),hc=14+(i*4),cs=3+(i*4),bi=(i%9)+1,ay=(i%7)+1;goat_health(lc,bl,bw,hc,cs,bi,ay);}
ps("\nGoat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=98+(i*3),bw=51+(i*3),hc=18+(i*2),cs=7+(i*2),bi=(i%5)+1,ay=(i%6)+1;goat_market(lc,bl,bw,hc,cs,bi,ay);}
ps("\n");goat_report();goat_state();ps("\n=== Demo Complete ===\n");return 0;}
