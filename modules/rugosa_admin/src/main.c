/* rugosa_admin: Rugosa management technology administration (v1.0)
 * Rugosa planning, rugosa execution, rugosa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rug_t;
typedef struct{int n_rugp,n_ruge,n_rug2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rug_state_t;
static rug_t rugps[N],rugss[N-2],rugvss[N-4],rugas[N-6],rugmks[N-6]; static rug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rug_init(void){if(init)return -1;st.n_rugp=0;st.n_ruge=0;st.n_rug2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rugps[i].active=0;for(int i=0;i<N-2;i++)rugss[i].active=0;for(int i=0;i<N-4;i++)rugvss[i].active=0;for(int i=0;i<N-6;i++)rugas[i].active=0;for(int i=0;i<N-6;i++)rugmks[i].active=0;init=1;ps("[RUG] Rugosa initialized\n");return 0;}
int rug_planning(int t,int c,int a,int b,int d,int e,int y){return add(rugps,&st.n_rugp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rug_execution(int t,int c,int a,int b,int d,int e,int y){return add(rugss,&st.n_ruge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rugvss,&st.n_rug2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rugas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rug_market(int t,int c,int a,int b,int d,int e,int y){return add(rugmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rug_report(void){ps("[RUG] Rugpp: ");pi(st.n_rugp);ps(" PCS=");pi(st.t_f1);ps("\nRuge: ");pi(st.n_ruge);ps(" PCS=");pi(st.t_f2);ps("\nRugv: ");pi(st.n_rug2);ps(" PCS=");pi(st.t_f3);ps("\nRugc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rug_state(void){ps("[RUG] Rugpp=");pi(st.n_rugp);ps(" Ruge=");pi(st.n_ruge);ps(" Rugv=");pi(st.n_rug2);ps(" Rugc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rugosa Admin Demo ===\n\n");rug_init();
ps("Rugosa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rug_planning(t,c,1183+(i*17),1172+(i*14),1152+(i*10),1134+(i*6),2020+(i%5));}
ps("\nRugosa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rug_execution(t,c,1172+(i*15),1161+(i*12),1143+(i*8),1130+(i*5),2021+(i%4));}
ps("\nRugosa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rug_evaluation(t,c,1164+(i*13),1153+(i*10),1137+(i*7),1126+(i*4),2022+(i%3));}
ps("\nRugosa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rug_accessory(t,c,1156+(i*11),1147+(i*9),1133+(i*6),1123+(i*3),2023+(i%2));}
ps("\nRugosa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rug_market(t,c,1150+(i*9),1141+(i*7),1128+(i*5),1120+(i*3),2024);}
ps("\n");rug_report();rug_state();ps("\n=== Demo Complete ===\n");return 0;}
