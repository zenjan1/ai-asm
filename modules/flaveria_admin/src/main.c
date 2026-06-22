/* flaveria_admin: Flaveria management technology administration (v1.0)
 * Flaveria planning, flaveria execution, flaveria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flav_t;
typedef struct{int n_flavp,n_flav,n_flav2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flav_state_t;
static flav_t flavs[N],flave[N-2],flav2[N-4],flavac[N-6],flavam[N-6]; static flav_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(flav_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flav_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLAV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flav_init(void){if(init)return -1;st.n_flavp=0;st.n_flav=0;st.n_flav2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)flavs[i].active=0;for(int i=0;i<N-2;i++)flave[i].active=0;for(int i=0;i<N-4;i++)flav2[i].active=0;for(int i=0;i<N-6;i++)flavac[i].active=0;for(int i=0;i<N-6;i++)flavam[i].active=0;init=1;ps("[FLAV] Flaveria initialized\n");return 0;}
int flav_planning(int t,int c,int a,int b,int d,int e,int y){return add(flavs,&st.n_flavp,&st.t_f1,N,t,c,a,b,d,e,y);}
int flav_execution(int t,int c,int a,int b,int d,int e,int y){return add(flave,&st.n_flav,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flav_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(flav2,&st.n_flav2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flav_accessory(int t,int c,int a,int b,int d,int e,int y){return add(flavac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flav_market(int t,int c,int a,int b,int d,int e,int y){return add(flavam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flav_report(void){ps("[FLAV] Avp: ");pi(st.n_flavp);ps(" PCS=");pi(st.t_f1);ps("\nAve: ");pi(st.n_flav);ps(" PCS=");pi(st.t_f2);ps("\nAv2: ");pi(st.n_flav2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flav_state(void){ps("[FLAV] Avp=");pi(st.n_flavp);ps(" Av=");pi(st.n_flav);ps(" Av2=");pi(st.n_flav2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flaveria Admin Demo ===\n\n");flav_init();
ps("Flaveria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flav_planning(t,c,1515+(i*17),1504+(i*14),1484+(i*10),1466+(i*6),2020+(i%5));}
ps("\nFlaveria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flav_execution(t,c,1504+(i*15),1493+(i*12),1475+(i*8),1462+(i*5),2021+(i%4));}
ps("\nFlaveria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flav_evaluation(t,c,1496+(i*13),1485+(i*10),1469+(i*7),1458+(i*4),2022+(i%3));}
ps("\nFlaveria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flav_accessory(t,c,1488+(i*11),1479+(i*9),1465+(i*6),1455+(i*3),2023+(i%2));}
ps("\nFlaveria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flav_market(t,c,1482+(i*9),1473+(i*7),1460+(i*5),1452+(i*3),2024);}
ps("\n");flav_report();flav_state();ps("\n=== Demo Complete ===\n");return 0;}
