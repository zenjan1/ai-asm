/* ape_admin: Ape (Gorilla gorilla) western gorilla great ape (v1.0)
 * Ape forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, arm_cm, climb_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_cm,cl_sp,fr_idx,age_yr,active;} ape_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_arm,t_cl,t_fr;} ape_state_t;
static ape_t apl[N],apf[N-2],apb[N-4],aph[N-6],apm[N-6]; static ape_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ape_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int cs,int fi,int ay){if(*cnt>=mx)return -1;ape_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_cm=ac;x->cl_sp=cs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[APE] Ape ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" cs=");pi(cs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ape_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)apf[i].active=0;for(int i=0;i<N-4;i++)apb[i].active=0;for(int i=0;i<N-6;i++)aph[i].active=0;for(int i=0;i<N-6;i++)apm[i].active=0;init=1;ps("[APE] Ape initialized\n");return 0;}
int ape_forest(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(apl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,cs,fi,ay);}
int ape_feeding(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(apf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,cs,fi,ay);}
int ape_breeding(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(apb,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,ac,cs,fi,ay);}
int ape_health(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(aph,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ac,cs,fi,ay);}
int ape_market(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(apm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,ac,cs,fi,ay);}
void ape_report(void){ps("[APE] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void ape_state(void){ps("[APE] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ape Admin Demo ===\n\n");ape_init();
ps("Ape forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=80+(i*15),ac=100+(i*6),cs=20+(i*3),fi=(i%10)+1,ay=(i%25)+1;ape_forest(lc,bl,bw,ac,cs,fi,ay);}
ps("\nApe feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*7),bw=85+(i*12),ac=105+(i*5),cs=22+(i*2),fi=(i%8)+1,ay=(i%20)+1;ape_feeding(lc,bl,bw,ac,cs,fi,ay);}
ps("\nApe breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*6),bw=90+(i*10),ac=110+(i*4),cs=18+(i*4),fi=(i%6)+1,ay=(i%15)+1;ape_breeding(lc,bl,bw,ac,cs,fi,ay);}
ps("\nApe health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*9),bw=78+(i*18),ac=98+(i*7),cs=15+(i*5),fi=(i%12)+1,ay=(i%12)+1;ape_health(lc,bl,bw,ac,cs,fi,ay);}
ps("\nApe market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*5),bw=95+(i*8),ac=115+(i*3),cs=25+(i*2),fi=(i%5)+1,ay=(i%10)+1;ape_market(lc,bl,bw,ac,cs,fi,ay);}
ps("\n");ape_report();ape_state();ps("\n=== Demo Complete ===\n");return 0;}
