/* calabash_admin: Calabash (Lagenaria) gourd vine cultivation management (v1.0)
 * Calabash planting, trellis, harvest, curing, market
 * Features: vine length, fruit count, fruit diameter, shell thickness, seed count, cure weeks
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vine_ln,frt_ct,frt_dia,shell_tk,seed_ct,cure_wk,active;} cala_t;
typedef struct{int n_plant,n_trell,n_harv,n_cure,n_mkt,t_vine,t_frt,t_dia,t_shell,t_seed;} cala_state_t;
static cala_t calaps[N],calats[N-2],calahs[N-4],calacrs[N-6],calams[N-6]; static cala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cala_t*a,int*cnt,int*sum,int mx,int lc,int vl,int fc,int fd,int sk,int sc,int cw){if(*cnt>=mx)return -1;cala_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vine_ln=vl;x->frt_ct=fc;x->frt_dia=fd;x->shell_tk=sk;x->seed_ct=sc;x->cure_wk=cw;x->active=1;*sum+=vl;(*cnt)++;ps("[CALA] Calabash ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" fc=");pi(fc);ps(" fd=");pi(fd);ps(" sk=");pi(sk);ps(" sc=");pi(sc);ps(" cw=");pi(cw);ps("\n");return *cnt-1;}
int cala_init(void){if(init)return -1;st.n_plant=0;st.n_trell=0;st.n_harv=0;st.n_cure=0;st.n_mkt=0;st.t_vine=0;st.t_frt=0;st.t_dia=0;st.t_shell=0;st.t_seed=0;for(int i=0;i<N;i++)calaps[i].active=0;for(int i=0;i<N-2;i++)calats[i].active=0;for(int i=0;i<N-4;i++)calahs[i].active=0;for(int i=0;i<N-6;i++)calacrs[i].active=0;for(int i=0;i<N-6;i++)calams[i].active=0;init=1;ps("[CALA] Calabash initialized\n");return 0;}
int cala_planting(int lc,int vl,int fc,int fd,int sk,int sc,int cw){return add(calaps,&st.n_plant,&st.t_vine,N,lc,vl,fc,fd,sk,sc,cw);}
int cala_trellis(int lc,int vl,int fc,int fd,int sk,int sc,int cw){return add(calats,&st.n_trell,&st.t_frt,N-2,lc,vl,fc,fd,sk,sc,cw);}
int cala_harvest(int lc,int vl,int fc,int fd,int sk,int sc,int cw){return add(calahs,&st.n_harv,&st.t_dia,N-4,lc,vl,fc,fd,sk,sc,cw);}
int cala_curing(int lc,int vl,int fc,int fd,int sk,int sc,int cw){return add(calacrs,&st.n_cure,&st.t_shell,N-6,lc,vl,fc,fd,sk,sc,cw);}
int cala_market(int lc,int vl,int fc,int fd,int sk,int sc,int cw){return add(calams,&st.n_mkt,&st.t_seed,N-6,lc,vl,fc,fd,sk,sc,cw);}
void cala_report(void){ps("[CALA] Plant: ");pi(st.n_plant);ps(" Vine=");pi(st.t_vine);ps("\nTrell: ");pi(st.n_trell);ps(" Frt=");pi(st.t_frt);ps("\nHarv: ");pi(st.n_harv);ps(" Dia=");pi(st.t_dia);ps("\nCure: ");pi(st.n_cure);ps(" Shell=");pi(st.t_shell);ps("\nMkt: ");pi(st.n_mkt);ps(" Seed=");pi(st.t_seed);ps("\n");}
void cala_state(void){ps("[CALA] Plant=");pi(st.n_plant);ps(" Trell=");pi(st.n_trell);ps(" Harv=");pi(st.n_harv);ps(" Cure=");pi(st.n_cure);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Calabash Admin Demo ===\n\n");cala_init();
/* 1=trellis 2=arbor 3=fence 4=ground 5=container */
ps("Calabash planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,vl=3+(i*2),fc=2+(i*3),fd=10+(i*5),sk=3+(i%5),sc=20+(i*10),cw=4+(i%4);cala_planting(lc,vl,fc,fd,sk,sc,cw);}
ps("\nCalabash trellis...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,vl=4+(i*2),fc=3+(i*2),fd=12+(i*4),sk=4+(i%4),sc=25+(i*8),cw=5+(i%3);cala_trellis(lc,vl,fc,fd,sk,sc,cw);}
ps("\nCalabash harvest...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,vl=5+(i*2),fc=4+(i*2),fd=15+(i*3),sk=5+(i%3),sc=30+(i*6),cw=6+(i%2);cala_harvest(lc,vl,fc,fd,sk,sc,cw);}
ps("\nCalabash curing...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,vl=2+(i*3),fc=1+(i*3),fd=8+(i*6),sk=2+(i%5),sc=15+(i*12),cw=3+(i%5);cala_curing(lc,vl,fc,fd,sk,sc,cw);}
ps("\nCalabash market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,vl=6+(i*2),fc=5+(i*2),fd=18+(i*2),sk=6+(i%2),sc=35+(i*5),cw=8+(i%2);cala_market(lc,vl,fc,fd,sk,sc,cw);}
ps("\n");cala_report();cala_state();ps("\n=== Demo Complete ===\n");return 0;}
