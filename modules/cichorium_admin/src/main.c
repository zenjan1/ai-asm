/* cichorium_admin: Cichorium management technology administration (v1.0)
 * Cichorium planning, cichorium execution, cichorium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cic_t;
typedef struct{int n_cicp,n_cice,n_cic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cic_state_t;
static cic_t cicps[N],cicss[N-2],cicvss[N-4],cicas[N-6],cicmks[N-6]; static cic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cic_init(void){if(init)return -1;st.n_cicp=0;st.n_cice=0;st.n_cic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cicps[i].active=0;for(int i=0;i<N-2;i++)cicss[i].active=0;for(int i=0;i<N-4;i++)cicvss[i].active=0;for(int i=0;i<N-6;i++)cicas[i].active=0;for(int i=0;i<N-6;i++)cicmks[i].active=0;init=1;ps("[CIC] Cichorium initialized\n");return 0;}
int cic_planning(int t,int c,int a,int b,int d,int e,int y){return add(cicps,&st.n_cicp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cic_execution(int t,int c,int a,int b,int d,int e,int y){return add(cicss,&st.n_cice,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cicvss,&st.n_cic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cicas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cic_market(int t,int c,int a,int b,int d,int e,int y){return add(cicmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cic_report(void){ps("[CIC] Cicpp: ");pi(st.n_cicp);ps(" PCS=");pi(st.t_f1);ps("\nCicpe: ");pi(st.n_cice);ps(" PCS=");pi(st.t_f2);ps("\nCicv: ");pi(st.n_cic2);ps(" PCS=");pi(st.t_f3);ps("\nCicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cic_state(void){ps("[CIC] Cicpp=");pi(st.n_cicp);ps(" Cicpe=");pi(st.n_cice);ps(" Cicv=");pi(st.n_cic2);ps(" Cicc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cichorium Admin Demo ===\n\n");cic_init();
ps("Cichorium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cic_planning(t,c,1236+(i*17),1225+(i*14),1205+(i*10),1187+(i*6),2020+(i%5));}
ps("\nCichorium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cic_execution(t,c,1225+(i*15),1214+(i*12),1196+(i*8),1183+(i*5),2021+(i%4));}
ps("\nCichorium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cic_evaluation(t,c,1217+(i*13),1206+(i*10),1190+(i*7),1179+(i*4),2022+(i%3));}
ps("\nCichorium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cic_accessory(t,c,1209+(i*11),1200+(i*9),1186+(i*6),1176+(i*3),2023+(i%2));}
ps("\nCichorium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cic_market(t,c,1203+(i*9),1194+(i*7),1181+(i*5),1173+(i*3),2024);}
ps("\n");cic_report();cic_state();ps("\n=== Demo Complete ===\n");return 0;}
