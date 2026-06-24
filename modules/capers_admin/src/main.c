/* capers_admin: Capers (Capparis spinosa) Mediterranean shrub management (v1.0)
 * Capers cultivation, picking, brining, drying, market
 * Features: bush width, bud count, berry kg, thorn density, soil pH, pick week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bush_wd,bud_ct,berry_kg,thorn_dn,soil_ph,pick_wk,active;} cap_t;
typedef struct{int n_cult,n_pick,n_brine,n_dry,n_mkt,t_bush,t_bud,t_berry,t_thorn,t_soil;} cap_state_t;
static cap_t capcs[N],cappks[N-2],capbs[N-4],capds[N-6],capms[N-6]; static cap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cap_t*a,int*cnt,int*sum,int mx,int lc,int bw,int bc,int bk,int td,int sp,int pw){if(*cnt>=mx)return -1;cap_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bush_wd=bw;x->bud_ct=bc;x->berry_kg=bk;x->thorn_dn=td;x->soil_ph=sp;x->pick_wk=pw;x->active=1;*sum+=bw;(*cnt)++;ps("[CAP] Cap ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" bk=");pi(bk);ps(" td=");pi(td);ps(" sp=");pi(sp);ps(" pw=");pi(pw);ps("\n");return *cnt-1;}
int cap_init(void){if(init)return -1;st.n_cult=0;st.n_pick=0;st.n_brine=0;st.n_dry=0;st.n_mkt=0;st.t_bush=0;st.t_bud=0;st.t_berry=0;st.t_thorn=0;st.t_soil=0;for(int i=0;i<N;i++)capcs[i].active=0;for(int i=0;i<N-2;i++)cappks[i].active=0;for(int i=0;i<N-4;i++)capbs[i].active=0;for(int i=0;i<N-6;i++)capds[i].active=0;for(int i=0;i<N-6;i++)capms[i].active=0;init=1;ps("[CAP] Capers initialized\n");return 0;}
int cap_cultivation(int lc,int bw,int bc,int bk,int td,int sp,int pw){return add(capcs,&st.n_cult,&st.t_bush,N,lc,bw,bc,bk,td,sp,pw);}
int cap_picking(int lc,int bw,int bc,int bk,int td,int sp,int pw){return add(cappks,&st.n_pick,&st.t_bud,N-2,lc,bw,bc,bk,td,sp,pw);}
int cap_brining(int lc,int bw,int bc,int bk,int td,int sp,int pw){return add(capbs,&st.n_brine,&st.t_berry,N-4,lc,bw,bc,bk,td,sp,pw);}
int cap_drying(int lc,int bw,int bc,int bk,int td,int sp,int pw){return add(capds,&st.n_dry,&st.t_thorn,N-6,lc,bw,bc,bk,td,sp,pw);}
int cap_market(int lc,int bw,int bc,int bk,int td,int sp,int pw){return add(capms,&st.n_mkt,&st.t_soil,N-6,lc,bw,bc,bk,td,sp,pw);}
void cap_report(void){ps("[CAP] Cult: ");pi(st.n_cult);ps(" Bush=");pi(st.t_bush);ps("\nPick: ");pi(st.n_pick);ps(" Bud=");pi(st.t_bud);ps("\nBrine: ");pi(st.n_brine);ps(" Berry=");pi(st.t_berry);ps("\nDry: ");pi(st.n_dry);ps(" Thorn=");pi(st.t_thorn);ps("\nMkt: ");pi(st.n_mkt);ps(" Soil=");pi(st.t_soil);ps("\n");}
void cap_state(void){ps("[CAP] Cult=");pi(st.n_cult);ps(" Pick=");pi(st.n_pick);ps(" Brine=");pi(st.n_brine);ps(" Dry=");pi(st.n_dry);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Capers Admin Demo ===\n\n");cap_init();
/* 1=mediterranean 2=rock_wall 3=dry_slope 4=container 5=coastal */
ps("Capers cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bw=40+(i*8),bc=10+(i*5),bk=1+(i%5),td=3+(i%4),sp=7+(i%2),pw=8+(i%4);cap_cultivation(lc,bw,bc,bk,td,sp,pw);}
ps("\nCapers picking...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bw=45+(i*6),bc=12+(i*4),bk=2+(i%4),td=4+(i%3),sp=7+(i%2),pw=10+(i%3);cap_picking(lc,bw,bc,bk,td,sp,pw);}
ps("\nCapers brining...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bw=50+(i*5),bc=15+(i*3),bk=3+(i%3),td=5+(i%2),sp=8+(i%1),pw=12+(i%2);cap_brining(lc,bw,bc,bk,td,sp,pw);}
ps("\nCapers drying...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bw=35+(i*9),bc=8+(i*6),bk=1+(i%5),td=2+(i%4),sp=6+(i%3),pw=6+(i%5);cap_drying(lc,bw,bc,bk,td,sp,pw);}
ps("\nCapers market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bw=55+(i*4),bc=18+(i*3),bk=4+(i%2),td=6+(i%2),sp=8+(i%1),pw=14+(i%2);cap_market(lc,bw,bc,bk,td,sp,pw);}
ps("\n");cap_report();cap_state();ps("\n=== Demo Complete ===\n");return 0;}
