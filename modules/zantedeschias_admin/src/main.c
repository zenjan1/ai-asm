/* zantedeschias_admin: Zantedeschias management technology administration (v1.0)
 * Zantedeschias planning, zantedeschias execution, zantedeschias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zant_t;
typedef struct{int n_zantp,n_zant,n_zant2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zant_state_t;
static zant_t zants[N],zante[N-2],zantv[N-4],zantac[N-6],zantm[N-6]; static zant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zant_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zant_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZANT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zant_init(void){if(init)return -1;st.n_zantp=0;st.n_zant=0;st.n_zant2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zants[i].active=0;for(int i=0;i<N-2;i++)zante[i].active=0;for(int i=0;i<N-4;i++)zantv[i].active=0;for(int i=0;i<N-6;i++)zantac[i].active=0;for(int i=0;i<N-6;i++)zantm[i].active=0;init=1;ps("[ZANT] Zantedeschias initialized\n");return 0;}
int zant_planning(int t,int c,int a,int b,int d,int e,int y){return add(zants,&st.n_zantp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zant_execution(int t,int c,int a,int b,int d,int e,int y){return add(zante,&st.n_zant,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zant_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zantv,&st.n_zant2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zant_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zantac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zant_market(int t,int c,int a,int b,int d,int e,int y){return add(zantm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zant_report(void){ps("[ZANT] Zantp: ");pi(st.n_zantp);ps(" PCS=");pi(st.t_f1);ps("\nZante: ");pi(st.n_zant);ps(" PCS=");pi(st.t_f2);ps("\nZantv: ");pi(st.n_zant2);ps(" PCS=");pi(st.t_f3);ps("\nZantac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zant_state(void){ps("[ZANT] Zantp=");pi(st.n_zantp);ps(" Zante=");pi(st.n_zant);ps(" Zantv=");pi(st.n_zant2);ps(" Zantac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zantedeschias Admin Demo ===\n\n");zant_init();
ps("Zantedeschias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zant_planning(t,c,1378+(i*17),1367+(i*14),1347+(i*10),1329+(i*6),2020+(i%5));}
ps("\nZantedeschias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zant_execution(t,c,1367+(i*15),1356+(i*12),1338+(i*8),1325+(i*5),2021+(i%4));}
ps("\nZantedeschias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zant_evaluation(t,c,1359+(i*13),1348+(i*10),1332+(i*7),1321+(i*4),2022+(i%3));}
ps("\nZantedeschias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zant_accessory(t,c,1351+(i*11),1342+(i*9),1328+(i*6),1318+(i*3),2023+(i%2));}
ps("\nZantedeschias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zant_market(t,c,1345+(i*9),1336+(i*7),1323+(i*5),1315+(i*3),2024);}
ps("\n");zant_report();zant_state();ps("\n=== Demo Complete ===\n");return 0;}
