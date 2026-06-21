/* oemleria_admin: Oemleria management technology administration (v1.0)
 * Oemleria planning, oemleria execution, oemleria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oem_t;
typedef struct{int n_oemp,n_oeme,n_oem2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oem_state_t;
static oem_t oemps[N],oemss[N-2],oemvss[N-4],oemas[N-6],oemmks[N-6]; static oem_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oem_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oem_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OEM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oem_init(void){if(init)return -1;st.n_oemp=0;st.n_oeme=0;st.n_oem2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oemps[i].active=0;for(int i=0;i<N-2;i++)oemss[i].active=0;for(int i=0;i<N-4;i++)oemvss[i].active=0;for(int i=0;i<N-6;i++)oemas[i].active=0;for(int i=0;i<N-6;i++)oemmks[i].active=0;init=1;ps("[OEM] Oemleria initialized\n");return 0;}
int oem_planning(int t,int c,int a,int b,int d,int e,int y){return add(oemps,&st.n_oemp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oem_execution(int t,int c,int a,int b,int d,int e,int y){return add(oemss,&st.n_oeme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oem_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oemvss,&st.n_oem2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oem_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oemas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oem_market(int t,int c,int a,int b,int d,int e,int y){return add(oemmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oem_report(void){ps("[OEM] Oemp: ");pi(st.n_oemp);ps(" PCS=");pi(st.t_f1);ps("\nOeme: ");pi(st.n_oeme);ps(" PCS=");pi(st.t_f2);ps("\nOemv: ");pi(st.n_oem2);ps(" PCS=");pi(st.t_f3);ps("\nOemc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oem_state(void){ps("[OEM] Oemp=");pi(st.n_oemp);ps(" Oeme=");pi(st.n_oeme);ps(" Oemv=");pi(st.n_oem2);ps(" Oemc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oemleria Admin Demo ===\n\n");oem_init();
ps("Oemleria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oem_planning(t,c,1074+(i*17),1063+(i*14),1043+(i*10),1025+(i*6),2020+(i%5));}
ps("\nOemleria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oem_execution(t,c,1063+(i*15),1052+(i*12),1034+(i*8),1021+(i*5),2021+(i%4));}
ps("\nOemleria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oem_evaluation(t,c,1055+(i*13),1044+(i*10),1028+(i*7),1017+(i*4),2022+(i%3));}
ps("\nOemleria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oem_accessory(t,c,1047+(i*11),1038+(i*9),1024+(i*6),1014+(i*3),2023+(i%2));}
ps("\nOemleria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oem_market(t,c,1041+(i*9),1032+(i*7),1019+(i*5),1011+(i*3),2024);}
ps("\n");oem_report();oem_state();ps("\n=== Demo Complete ===\n");return 0;}
