/* philadelphus_admin: Philadelphus management technology administration (v1.0)
 * Philadelphus planning, philadelphus execution, philadelphus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phi_t;
typedef struct{int n_phip,n_phie,n_phi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phi_state_t;
static phi_t phips[N],phiss[N-2],phivss[N-4],phias[N-6],phimks[N-6]; static phi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(phi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phi_init(void){if(init)return -1;st.n_phip=0;st.n_phie=0;st.n_phi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)phips[i].active=0;for(int i=0;i<N-2;i++)phiss[i].active=0;for(int i=0;i<N-4;i++)phivss[i].active=0;for(int i=0;i<N-6;i++)phias[i].active=0;for(int i=0;i<N-6;i++)phimks[i].active=0;init=1;ps("[PHI] Philadelphus initialized\n");return 0;}
int phi_planning(int t,int c,int a,int b,int d,int e,int y){return add(phips,&st.n_phip,&st.t_f1,N,t,c,a,b,d,e,y);}
int phi_execution(int t,int c,int a,int b,int d,int e,int y){return add(phiss,&st.n_phie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(phivss,&st.n_phi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(phias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phi_market(int t,int c,int a,int b,int d,int e,int y){return add(phimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phi_report(void){ps("[PHI] Phip: ");pi(st.n_phip);ps(" PCS=");pi(st.t_f1);ps("\nPhie: ");pi(st.n_phie);ps(" PCS=");pi(st.t_f2);ps("\nPhiv: ");pi(st.n_phi2);ps(" PCS=");pi(st.t_f3);ps("\nPhic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phi_state(void){ps("[PHI] Phip=");pi(st.n_phip);ps(" Phie=");pi(st.n_phie);ps(" Phiv=");pi(st.n_phi2);ps(" Phic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Philadelphus Admin Demo ===\n\n");phi_init();
ps("Philadelphus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phi_planning(t,c,944+(i*17),933+(i*14),913+(i*10),895+(i*6),2020+(i%5));}
ps("\nPhiladelphus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phi_execution(t,c,933+(i*15),922+(i*12),904+(i*8),891+(i*5),2021+(i%4));}
ps("\nPhiladelphus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phi_evaluation(t,c,925+(i*13),914+(i*10),898+(i*7),887+(i*4),2022+(i%3));}
ps("\nPhiladelphus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phi_accessory(t,c,917+(i*11),908+(i*9),894+(i*6),884+(i*3),2023+(i%2));}
ps("\nPhiladelphus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phi_market(t,c,911+(i*9),902+(i*7),889+(i*5),881+(i*3),2024);}
ps("\n");phi_report();phi_state();ps("\n=== Demo Complete ===\n");return 0;}
