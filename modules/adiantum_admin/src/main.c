/* adiantum_admin: Adiantum management technology administration (v1.0)
 * Adiantum planning, adiantum execution, adiantum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} adi_t;
typedef struct{int n_adip,n_adie,n_adi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} adi_state_t;
static adi_t adips[N],adiss[N-2],adivss[N-4],adias[N-6],adimks[N-6]; static adi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(adi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;adi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ADI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int adi_init(void){if(init)return -1;st.n_adip=0;st.n_adie=0;st.n_adi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)adips[i].active=0;for(int i=0;i<N-2;i++)adiss[i].active=0;for(int i=0;i<N-4;i++)adivss[i].active=0;for(int i=0;i<N-6;i++)adias[i].active=0;for(int i=0;i<N-6;i++)adimks[i].active=0;init=1;ps("[ADI] Adiantum initialized\n");return 0;}
int adi_planning(int t,int c,int a,int b,int d,int e,int y){return add(adips,&st.n_adip,&st.t_f1,N,t,c,a,b,d,e,y);}
int adi_execution(int t,int c,int a,int b,int d,int e,int y){return add(adiss,&st.n_adie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int adi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(adivss,&st.n_adi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int adi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(adias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int adi_market(int t,int c,int a,int b,int d,int e,int y){return add(adimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void adi_report(void){ps("[ADI] Adipp: ");pi(st.n_adip);ps(" PCS=");pi(st.t_f1);ps("\nAdipe: ");pi(st.n_adie);ps(" PCS=");pi(st.t_f2);ps("\nAdiv: ");pi(st.n_adi2);ps(" PCS=");pi(st.t_f3);ps("\nAdic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void adi_state(void){ps("[ADI] Adipp=");pi(st.n_adip);ps(" Adipe=");pi(st.n_adie);ps(" Adiv=");pi(st.n_adi2);ps(" Adic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Adiantum Admin Demo ===\n\n");adi_init();
ps("Adiantum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;adi_planning(t,c,1213+(i*17),1202+(i*14),1182+(i*10),1164+(i*6),2020+(i%5));}
ps("\nAdiantum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;adi_execution(t,c,1202+(i*15),1191+(i*12),1173+(i*8),1160+(i*5),2021+(i%4));}
ps("\nAdiantum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;adi_evaluation(t,c,1194+(i*13),1183+(i*10),1167+(i*7),1156+(i*4),2022+(i%3));}
ps("\nAdiantum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adi_accessory(t,c,1186+(i*11),1177+(i*9),1163+(i*6),1153+(i*3),2023+(i%2));}
ps("\nAdiantum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adi_market(t,c,1180+(i*9),1171+(i*7),1158+(i*5),1150+(i*3),2024);}
ps("\n");adi_report();adi_state();ps("\n=== Demo Complete ===\n");return 0;}
