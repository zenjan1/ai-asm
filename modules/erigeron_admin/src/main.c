/* erigeron_admin: Erigeron management technology administration (v1.0)
 * Erigeron planning, erigeron execution, erigeron evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} erg_t;
typedef struct{int n_ergp,n_erge,n_erg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} erg_state_t;
static erg_t ergps[N],ergss[N-2],ergvss[N-4],ergas[N-6],ergmks[N-6]; static erg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(erg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;erg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ERG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int erg_init(void){if(init)return -1;st.n_ergp=0;st.n_erge=0;st.n_erg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ergps[i].active=0;for(int i=0;i<N-2;i++)ergss[i].active=0;for(int i=0;i<N-4;i++)ergvss[i].active=0;for(int i=0;i<N-6;i++)ergas[i].active=0;for(int i=0;i<N-6;i++)ergmks[i].active=0;init=1;ps("[ERG] Erigeron initialized\n");return 0;}
int erg_planning(int t,int c,int a,int b,int d,int e,int y){return add(ergps,&st.n_ergp,&st.t_f1,N,t,c,a,b,d,e,y);}
int erg_execution(int t,int c,int a,int b,int d,int e,int y){return add(ergss,&st.n_erge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int erg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ergvss,&st.n_erg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int erg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ergas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int erg_market(int t,int c,int a,int b,int d,int e,int y){return add(ergmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void erg_report(void){ps("[ERG] Erip: ");pi(st.n_ergp);ps(" PCS=");pi(st.t_f1);ps("\nErie: ");pi(st.n_erge);ps(" PCS=");pi(st.t_f2);ps("\nEriv: ");pi(st.n_erg2);ps(" PCS=");pi(st.t_f3);ps("\nEric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void erg_state(void){ps("[ERG] Erip=");pi(st.n_ergp);ps(" Erie=");pi(st.n_erge);ps(" Eriv=");pi(st.n_erg2);ps(" Eric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Erigeron Admin Demo ===\n\n");erg_init();
ps("Erigeron planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;erg_planning(t,c,1279+(i*17),1268+(i*14),1248+(i*10),1230+(i*6),2020+(i%5));}
ps("\nErigeron execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;erg_execution(t,c,1268+(i*15),1257+(i*12),1239+(i*8),1226+(i*5),2021+(i%4));}
ps("\nErigeron evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;erg_evaluation(t,c,1260+(i*13),1249+(i*10),1233+(i*7),1222+(i*4),2022+(i%3));}
ps("\nErigeron accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;erg_accessory(t,c,1252+(i*11),1243+(i*9),1229+(i*6),1219+(i*3),2023+(i%2));}
ps("\nErigeron marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;erg_market(t,c,1246+(i*9),1237+(i*7),1224+(i*5),1216+(i*3),2024);}
ps("\n");erg_report();erg_state();ps("\n=== Demo Complete ===\n");return 0;}
