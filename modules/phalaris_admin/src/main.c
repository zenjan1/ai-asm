/* phalaris_admin: Phalaris management technology administration (v1.0)
 * Phalaris planning, phalaris execution, phalaris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pha_t;
typedef struct{int n_phap,n_phoe,n_pha2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pha_state_t;
static pha_t phaps[N],phaess[N-2],phavss[N-4],phaas[N-6],phamks[N-6]; static pha_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(pha_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pha_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pha_init(void){if(init)return -1;st.n_phap=0;st.n_phoe=0;st.n_pha2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)phaps[i].active=0;for(int i=0;i<N-2;i++)phaess[i].active=0;for(int i=0;i<N-4;i++)phavss[i].active=0;for(int i=0;i<N-6;i++)phaas[i].active=0;for(int i=0;i<N-6;i++)phamks[i].active=0;init=1;ps("[PHA] Phalaris initialized\n");return 0;}
int pha_planning(int t,int c,int a,int b,int d,int e,int y){return add(phaps,&st.n_phap,&st.t_f1,N,t,c,a,b,d,e,y);}
int pha_execution(int t,int c,int a,int b,int d,int e,int y){return add(phaess,&st.n_phoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pha_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(phavss,&st.n_pha2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pha_accessory(int t,int c,int a,int b,int d,int e,int y){return add(phaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pha_market(int t,int c,int a,int b,int d,int e,int y){return add(phamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pha_report(void){ps("[PHA] Phap: ");pi(st.n_phap);ps(" PCS=");pi(st.t_f1);ps("\nPhoe: ");pi(st.n_phoe);ps(" PCS=");pi(st.t_f2);ps("\nPhav: ");pi(st.n_pha2);ps(" PCS=");pi(st.t_f3);ps("\nPhac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pha_state(void){ps("[PHA] Phap=");pi(st.n_phap);ps(" Phoe=");pi(st.n_phoe);ps(" Phav=");pi(st.n_pha2);ps(" Phac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phalaris Admin Demo ===\n\n");pha_init();
ps("Phalaris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pha_planning(t,c,1326+(i*17),1315+(i*14),1295+(i*10),1277+(i*6),2020+(i%5));}
ps("\nPhalaris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pha_execution(t,c,1315+(i*15),1304+(i*12),1286+(i*8),1273+(i*5),2021+(i%4));}
ps("\nPhalaris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pha_evaluation(t,c,1307+(i*13),1296+(i*10),1280+(i*7),1269+(i*4),2022+(i%3));}
ps("\nPhalaris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pha_accessory(t,c,1299+(i*11),1290+(i*9),1276+(i*6),1266+(i*3),2023+(i%2));}
ps("\nPhalaris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pha_market(t,c,1293+(i*9),1284+(i*7),1271+(i*5),1263+(i*3),2024);}
ps("\n");pha_report();pha_state();ps("\n=== Demo Complete ===\n");return 0;}
