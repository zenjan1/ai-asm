/* sophora_admin: Sophora management technology administration (v1.0)
 * Sophora planning, sophora execution, sophora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sop_t;
typedef struct{int n_sopp,n_sope,n_sop2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sop_state_t;
static sop_t sopps[N],sopss[N-2],sopvss[N-4],sopas[N-6],sopmks[N-6]; static sop_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sop_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sop_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SOP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sop_init(void){if(init)return -1;st.n_sopp=0;st.n_sope=0;st.n_sop2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sopps[i].active=0;for(int i=0;i<N-2;i++)sopss[i].active=0;for(int i=0;i<N-4;i++)sopvss[i].active=0;for(int i=0;i<N-6;i++)sopas[i].active=0;for(int i=0;i<N-6;i++)sopmks[i].active=0;init=1;ps("[SOP] Sophora initialized\n");return 0;}
int sop_planning(int t,int c,int a,int b,int d,int e,int y){return add(sopps,&st.n_sopp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sop_execution(int t,int c,int a,int b,int d,int e,int y){return add(sopss,&st.n_sope,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sop_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sopvss,&st.n_sop2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sop_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sopas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sop_market(int t,int c,int a,int b,int d,int e,int y){return add(sopmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sop_report(void){ps("[SOP] Soppp: ");pi(st.n_sopp);ps(" PCS=");pi(st.t_f1);ps("\nSope: ");pi(st.n_sope);ps(" PCS=");pi(st.t_f2);ps("\nSopv: ");pi(st.n_sop2);ps(" PCS=");pi(st.t_f3);ps("\nSopc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sop_state(void){ps("[SOP] Soppp=");pi(st.n_sopp);ps(" Sope=");pi(st.n_sope);ps(" Sopv=");pi(st.n_sop2);ps(" Sopc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sophora Admin Demo ===\n\n");sop_init();
ps("Sophora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sop_planning(t,c,1194+(i*17),1183+(i*14),1163+(i*10),1145+(i*6),2020+(i%5));}
ps("\nSophora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sop_execution(t,c,1183+(i*15),1172+(i*12),1154+(i*8),1141+(i*5),2021+(i%4));}
ps("\nSophora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sop_evaluation(t,c,1175+(i*13),1164+(i*10),1148+(i*7),1137+(i*4),2022+(i%3));}
ps("\nSophora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sop_accessory(t,c,1167+(i*11),1158+(i*9),1144+(i*6),1134+(i*3),2023+(i%2));}
ps("\nSophora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sop_market(t,c,1161+(i*9),1152+(i*7),1139+(i*5),1131+(i*3),2024);}
ps("\n");sop_report();sop_state();ps("\n=== Demo Complete ===\n");return 0;}
