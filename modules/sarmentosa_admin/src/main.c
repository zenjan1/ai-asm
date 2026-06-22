/* sarmentosa_admin: Sarmentosa management technology administration (v1.0)
 * Sarmentosa planning, sarmentosa execution, sarmentosa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sar_t;
typedef struct{int n_sarp,n_sare,n_sar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sar_state_t;
static sar_t sarps[N],sarss[N-2],sarvss[N-4],saras[N-6],sarmks[N-6]; static sar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sar_init(void){if(init)return -1;st.n_sarp=0;st.n_sare=0;st.n_sar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sarps[i].active=0;for(int i=0;i<N-2;i++)sarss[i].active=0;for(int i=0;i<N-4;i++)sarvss[i].active=0;for(int i=0;i<N-6;i++)saras[i].active=0;for(int i=0;i<N-6;i++)sarmks[i].active=0;init=1;ps("[SAR] Sarmentosa initialized\n");return 0;}
int sar_planning(int t,int c,int a,int b,int d,int e,int y){return add(sarps,&st.n_sarp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sar_execution(int t,int c,int a,int b,int d,int e,int y){return add(sarss,&st.n_sare,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sarvss,&st.n_sar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(saras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sar_market(int t,int c,int a,int b,int d,int e,int y){return add(sarmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sar_report(void){ps("[SAR] Sarpp: ");pi(st.n_sarp);ps(" PCS=");pi(st.t_f1);ps("\nSare: ");pi(st.n_sare);ps(" PCS=");pi(st.t_f2);ps("\nSarv: ");pi(st.n_sar2);ps(" PCS=");pi(st.t_f3);ps("\nSarc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sar_state(void){ps("[SAR] Sarpp=");pi(st.n_sarp);ps(" Sare=");pi(st.n_sare);ps(" Sarv=");pi(st.n_sar2);ps(" Sarc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sarmentosa Admin Demo ===\n\n");sar_init();
ps("Sarmentosa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sar_planning(t,c,1187+(i*17),1176+(i*14),1156+(i*10),1138+(i*6),2020+(i%5));}
ps("\nSarmentosa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sar_execution(t,c,1176+(i*15),1165+(i*12),1147+(i*8),1134+(i*5),2021+(i%4));}
ps("\nSarmentosa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sar_evaluation(t,c,1168+(i*13),1157+(i*10),1141+(i*7),1130+(i*4),2022+(i%3));}
ps("\nSarmentosa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sar_accessory(t,c,1160+(i*11),1151+(i*9),1137+(i*6),1127+(i*3),2023+(i%2));}
ps("\nSarmentosa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sar_market(t,c,1154+(i*9),1145+(i*7),1132+(i*5),1124+(i*3),2024);}
ps("\n");sar_report();sar_state();ps("\n=== Demo Complete ===\n");return 0;}
