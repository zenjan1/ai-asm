/* ixia_admin: Ixia management technology administration (v1.0)
 * Ixia planning, ixia execution, ixia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ixi_t;
typedef struct{int n_ixip,n_ixie,n_ixi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ixi_state_t;
static ixi_t ixips[N],ixiss[N-2],ixivss[N-4],ixias[N-6],iximks[N-6]; static ixi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ixi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ixi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IXI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ixi_init(void){if(init)return -1;st.n_ixip=0;st.n_ixie=0;st.n_ixi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ixips[i].active=0;for(int i=0;i<N-2;i++)ixiss[i].active=0;for(int i=0;i<N-4;i++)ixivss[i].active=0;for(int i=0;i<N-6;i++)ixias[i].active=0;for(int i=0;i<N-6;i++)iximks[i].active=0;init=1;ps("[IXI] Ixia initialized\n");return 0;}
int ixi_planning(int t,int c,int a,int b,int d,int e,int y){return add(ixips,&st.n_ixip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ixi_execution(int t,int c,int a,int b,int d,int e,int y){return add(ixiss,&st.n_ixie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ixi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ixivss,&st.n_ixi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ixi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ixias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ixi_market(int t,int c,int a,int b,int d,int e,int y){return add(iximks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ixi_report(void){ps("[IXI] Ixipp: ");pi(st.n_ixip);ps(" PCS=");pi(st.t_f1);ps("\nIxie: ");pi(st.n_ixie);ps(" PCS=");pi(st.t_f2);ps("\nIxiv: ");pi(st.n_ixi2);ps(" PCS=");pi(st.t_f3);ps("\nIxic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ixi_state(void){ps("[IXI] Ixipp=");pi(st.n_ixip);ps(" Ixie=");pi(st.n_ixie);ps(" Ixiv=");pi(st.n_ixi2);ps(" Ixic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ixia Admin Demo ===\n\n");ixi_init();
ps("Ixia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ixi_planning(t,c,1151+(i*17),1140+(i*14),1120+(i*10),1102+(i*6),2020+(i%5));}
ps("\nIxia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ixi_execution(t,c,1140+(i*15),1129+(i*12),1111+(i*8),1098+(i*5),2021+(i%4));}
ps("\nIxia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ixi_evaluation(t,c,1132+(i*13),1121+(i*10),1105+(i*7),1094+(i*4),2022+(i%3));}
ps("\nIxia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ixi_accessory(t,c,1124+(i*11),1115+(i*9),1101+(i*6),1091+(i*3),2023+(i%2));}
ps("\nIxia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ixi_market(t,c,1118+(i*9),1109+(i*7),1096+(i*5),1088+(i*3),2024);}
ps("\n");ixi_report();ixi_state();ps("\n=== Demo Complete ===\n");return 0;}
