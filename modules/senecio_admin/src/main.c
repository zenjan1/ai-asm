/* senecio_admin: Senecio management technology administration (v1.0)
 * Senecio planning, senecio execution, senecio evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sene_t;
typedef struct{int n_senep,n_senee,n_sene2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sene_state_t;
static sene_t senes[N],senesses[N-2],senevss[N-4],seneas[N-6],senemarks[N-6]; static sene_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(sene_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sene_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SENE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sene_init(void){if(init)return -1;st.n_senep=0;st.n_senee=0;st.n_sene2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)senes[i].active=0;for(int i=0;i<N-2;i++)senesses[i].active=0;for(int i=0;i<N-4;i++)senevss[i].active=0;for(int i=0;i<N-6;i++)seneas[i].active=0;for(int i=0;i<N-6;i++)senemarks[i].active=0;init=1;ps("[SENE] Senecio initialized\n");return 0;}
int sene_planning(int t,int c,int a,int b,int d,int e,int y){return add(senes,&st.n_senep,&st.t_f1,N,t,c,a,b,d,e,y);}
int sene_execution(int t,int c,int a,int b,int d,int e,int y){return add(senesses,&st.n_senee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sene_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(senevss,&st.n_sene2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sene_accessory(int t,int c,int a,int b,int d,int e,int y){return add(seneas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sene_market(int t,int c,int a,int b,int d,int e,int y){return add(senemarks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sene_report(void){ps("[SENE] Senep: ");pi(st.n_senep);ps(" PCS=");pi(st.t_f1);ps("\nSenee: ");pi(st.n_senee);ps(" PCS=");pi(st.t_f2);ps("\nSenev: ");pi(st.n_sene2);ps(" PCS=");pi(st.t_f3);ps("\nSenec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sene_state(void){ps("[SENE] Senep=");pi(st.n_senep);ps(" Senee=");pi(st.n_senee);ps(" Senev=");pi(st.n_sene2);ps(" Senec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Senecio Admin Demo ===\n\n");sene_init();
ps("Senecio planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sene_planning(t,c,1334+(i*17),1323+(i*14),1303+(i*10),1285+(i*6),2020+(i%5));}
ps("\nSenecio execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sene_execution(t,c,1323+(i*15),1312+(i*12),1294+(i*8),1281+(i*5),2021+(i%4));}
ps("\nSenecio evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sene_evaluation(t,c,1315+(i*13),1304+(i*10),1288+(i*7),1277+(i*4),2022+(i%3));}
ps("\nSenecio accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sene_accessory(t,c,1307+(i*11),1298+(i*9),1284+(i*6),1274+(i*3),2023+(i%2));}
ps("\nSenecio marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sene_market(t,c,1301+(i*9),1292+(i*7),1279+(i*5),1271+(i*3),2024);}
ps("\n");sene_report();sene_state();ps("\n=== Demo Complete ===\n");return 0;}
