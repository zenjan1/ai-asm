/* sisyrinchium_admin: Sisyrinchium management technology administration (v1.0)
 * Sisyrinchium planning, sisyrinchium execution, sisyrinchium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sisy_t;
typedef struct{int n_sisyp,n_sisye,n_sisy2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sisy_state_t;
static sisy_t sisys[N],sisyess[N-2],sisyvss[N-4],sisyas[N-6],sisymks[N-6]; static sisy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(sisy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sisy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SISY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sisy_init(void){if(init)return -1;st.n_sisyp=0;st.n_sisye=0;st.n_sisy2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sisys[i].active=0;for(int i=0;i<N-2;i++)sisyess[i].active=0;for(int i=0;i<N-4;i++)sisyvss[i].active=0;for(int i=0;i<N-6;i++)sisyas[i].active=0;for(int i=0;i<N-6;i++)sisymks[i].active=0;init=1;ps("[SISY] Sisyrinchium initialized\n");return 0;}
int sisy_planning(int t,int c,int a,int b,int d,int e,int y){return add(sisys,&st.n_sisyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sisy_execution(int t,int c,int a,int b,int d,int e,int y){return add(sisyess,&st.n_sisye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sisy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sisyvss,&st.n_sisy2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sisy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sisyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sisy_market(int t,int c,int a,int b,int d,int e,int y){return add(sisymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sisy_report(void){ps("[SISY] Sisyp: ");pi(st.n_sisyp);ps(" PCS=");pi(st.t_f1);ps("\nSisye: ");pi(st.n_sisye);ps(" PCS=");pi(st.t_f2);ps("\nSisyv: ");pi(st.n_sisy2);ps(" PCS=");pi(st.t_f3);ps("\nSisyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sisy_state(void){ps("[SISY] Sisyp=");pi(st.n_sisyp);ps(" Sisye=");pi(st.n_sisye);ps(" Sisyv=");pi(st.n_sisy2);ps(" Sisyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sisyrinchium Admin Demo ===\n\n");sisy_init();
ps("Sisyrinchium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sisy_planning(t,c,1338+(i*17),1327+(i*14),1307+(i*10),1289+(i*6),2020+(i%5));}
ps("\nSisyrinchium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sisy_execution(t,c,1327+(i*15),1316+(i*12),1298+(i*8),1285+(i*5),2021+(i%4));}
ps("\nSisyrinchium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sisy_evaluation(t,c,1319+(i*13),1308+(i*10),1292+(i*7),1281+(i*4),2022+(i%3));}
ps("\nSisyrinchium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sisy_accessory(t,c,1311+(i*11),1302+(i*9),1288+(i*6),1278+(i*3),2023+(i%2));}
ps("\nSisyrinchium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sisy_market(t,c,1305+(i*9),1296+(i*7),1283+(i*5),1275+(i*3),2024);}
ps("\n");sisy_report();sisy_state();ps("\n=== Demo Complete ===\n");return 0;}
