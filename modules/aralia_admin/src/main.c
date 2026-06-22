/* aralia_admin: Aralia management technology administration (v1.0)
 * Aralia planning, aralia execution, aralia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aral_t;
typedef struct{int n_aralp,n_aral,n_aral2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aral_state_t;
static aral_t arals[N],arale[N-2],aral2[N-4],aralac[N-6],aralam[N-6]; static aral_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(aral_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aral_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aral_init(void){if(init)return -1;st.n_aralp=0;st.n_aral=0;st.n_aral2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arals[i].active=0;for(int i=0;i<N-2;i++)arale[i].active=0;for(int i=0;i<N-4;i++)aral2[i].active=0;for(int i=0;i<N-6;i++)aralac[i].active=0;for(int i=0;i<N-6;i++)aralam[i].active=0;init=1;ps("[ARAL] Aralia initialized\n");return 0;}
int aral_planning(int t,int c,int a,int b,int d,int e,int y){return add(arals,&st.n_aralp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aral_execution(int t,int c,int a,int b,int d,int e,int y){return add(arale,&st.n_aral,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aral_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aral2,&st.n_aral2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aral_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aralac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aral_market(int t,int c,int a,int b,int d,int e,int y){return add(aralam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aral_report(void){ps("[ARAL] Ralp: ");pi(st.n_aralp);ps(" PCS=");pi(st.t_f1);ps("\nRale: ");pi(st.n_aral);ps(" PCS=");pi(st.t_f2);ps("\nRal2: ");pi(st.n_aral2);ps(" PCS=");pi(st.t_f3);ps("\nRalac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aral_state(void){ps("[ARAL] Ralp=");pi(st.n_aralp);ps(" Rale=");pi(st.n_aral);ps(" Ral2=");pi(st.n_aral2);ps(" Ralac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aralia Admin Demo ===\n\n");aral_init();
ps("Aralia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aral_planning(t,c,1411+(i*17),1400+(i*14),1380+(i*10),1362+(i*6),2020+(i%5));}
ps("\nAralia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aral_execution(t,c,1400+(i*15),1389+(i*12),1371+(i*8),1358+(i*5),2021+(i%4));}
ps("\nAralia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aral_evaluation(t,c,1392+(i*13),1381+(i*10),1365+(i*7),1354+(i*4),2022+(i%3));}
ps("\nAralia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aral_accessory(t,c,1384+(i*11),1375+(i*9),1361+(i*6),1351+(i*3),2023+(i%2));}
ps("\nAralia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aral_market(t,c,1378+(i*9),1369+(i*7),1356+(i*5),1348+(i*3),2024);}
ps("\n");aral_report();aral_state();ps("\n=== Demo Complete ===\n");return 0;}
