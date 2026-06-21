/* polemonium_admin: Polemonium management technology administration (v1.0)
 * Polemonium planning, polemonium execution, polemonium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pol_t;
typedef struct{int n_polp,n_pole,n_pol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pol_state_t;
static pol_t polps[N],polss[N-2],polvss[N-4],polac[N-6],polmks[N-6]; static pol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[POL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pol_init(void){if(init)return -1;st.n_polp=0;st.n_pole=0;st.n_pol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)polps[i].active=0;for(int i=0;i<N-2;i++)polss[i].active=0;for(int i=0;i<N-4;i++)polvss[i].active=0;for(int i=0;i<N-6;i++)polac[i].active=0;for(int i=0;i<N-6;i++)polmks[i].active=0;init=1;ps("[POL] Polemonium initialized\n");return 0;}
int pol_planning(int t,int c,int a,int b,int d,int e,int y){return add(polps,&st.n_polp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pol_execution(int t,int c,int a,int b,int d,int e,int y){return add(polss,&st.n_pole,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(polvss,&st.n_pol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(polac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pol_market(int t,int c,int a,int b,int d,int e,int y){return add(polmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pol_report(void){ps("[POL] Polp: ");pi(st.n_polp);ps(" PCS=");pi(st.t_f1);ps("\nPole: ");pi(st.n_pole);ps(" PCS=");pi(st.t_f2);ps("\nPolv: ");pi(st.n_pol2);ps(" PCS=");pi(st.t_f3);ps("\nPolc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pol_state(void){ps("[POL] Polp=");pi(st.n_polp);ps(" Pole=");pi(st.n_pole);ps(" Polv=");pi(st.n_pol2);ps(" Polc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Polemonium Admin Demo ===\n\n");pol_init();
ps("Polemonium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pol_planning(t,c,945+(i*17),934+(i*14),914+(i*10),896+(i*6),2020+(i%5));}
ps("\nPolemonium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pol_execution(t,c,934+(i*15),923+(i*12),905+(i*8),892+(i*5),2021+(i%4));}
ps("\nPolemonium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pol_evaluation(t,c,926+(i*13),915+(i*10),899+(i*7),888+(i*4),2022+(i%3));}
ps("\nPolemonium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pol_accessory(t,c,918+(i*11),909+(i*9),895+(i*6),885+(i*3),2023+(i%2));}
ps("\nPolemonium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pol_market(t,c,912+(i*9),903+(i*7),890+(i*5),882+(i*3),2024);}
ps("\n");pol_report();pol_state();ps("\n=== Demo Complete ===\n");return 0;}
