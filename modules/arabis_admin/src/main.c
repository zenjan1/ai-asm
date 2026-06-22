/* arabis_admin: Arabis management technology administration (v1.0)
 * Arabis planning, arabis execution, arabis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arab_t;
typedef struct{int n_arabp,n_arab,n_arab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arab_state_t;
static arab_t arabs[N],arabes[N-2],arab2[N-4],arabac[N-6],arabam[N-6]; static arab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(arab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arab_init(void){if(init)return -1;st.n_arabp=0;st.n_arab=0;st.n_arab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arabs[i].active=0;for(int i=0;i<N-2;i++)arabes[i].active=0;for(int i=0;i<N-4;i++)arab2[i].active=0;for(int i=0;i<N-6;i++)arabac[i].active=0;for(int i=0;i<N-6;i++)arabam[i].active=0;init=1;ps("[ARAB] Arabis initialized\n");return 0;}
int arab_planning(int t,int c,int a,int b,int d,int e,int y){return add(arabs,&st.n_arabp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arab_execution(int t,int c,int a,int b,int d,int e,int y){return add(arabes,&st.n_arab,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arab2,&st.n_arab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(arabac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arab_market(int t,int c,int a,int b,int d,int e,int y){return add(arabam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arab_report(void){ps("[ARAB] Rabp: ");pi(st.n_arabp);ps(" PCS=");pi(st.t_f1);ps("\nRabe: ");pi(st.n_arab);ps(" PCS=");pi(st.t_f2);ps("\nRab2: ");pi(st.n_arab2);ps(" PCS=");pi(st.t_f3);ps("\nRabac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arab_state(void){ps("[ARAB] Rabp=");pi(st.n_arabp);ps(" Rabe=");pi(st.n_arab);ps(" Rab2=");pi(st.n_arab2);ps(" Rabac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arabis Admin Demo ===\n\n");arab_init();
ps("Arabis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arab_planning(t,c,1410+(i*17),1399+(i*14),1379+(i*10),1361+(i*6),2020+(i%5));}
ps("\nArabis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arab_execution(t,c,1399+(i*15),1388+(i*12),1370+(i*8),1357+(i*5),2021+(i%4));}
ps("\nArabis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arab_evaluation(t,c,1391+(i*13),1380+(i*10),1364+(i*7),1353+(i*4),2022+(i%3));}
ps("\nArabis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arab_accessory(t,c,1383+(i*11),1374+(i*9),1360+(i*6),1350+(i*3),2023+(i%2));}
ps("\nArabis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arab_market(t,c,1377+(i*9),1368+(i*7),1355+(i*5),1347+(i*3),2024);}
ps("\n");arab_report();arab_state();ps("\n=== Demo Complete ===\n");return 0;}
