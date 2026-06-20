/* spraytech_admin: Spray technology administration (v1.0)
 * Air spraying, electrostatic spraying, thermal spraying, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spr_t;
typedef struct{int n_ar,n_es,n_th,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spr_state_t;
static spr_t ars[N],ess[N-2],ths[N-4],acs[N-6],mks[N-6]; static spr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spr_init(void){if(init)return -1;st.n_ar=0;st.n_es=0;st.n_th=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ars[i].active=0;for(int i=0;i<N-2;i++)ess[i].active=0;for(int i=0;i<N-4;i++)ths[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPR] Spraytech initialized\n");return 0;}
int spr_air(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f1,N,t,c,a,b,d,e,y);}
int spr_electrostatic(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spr_thermal(int t,int c,int a,int b,int d,int e,int y){return add(ths,&st.n_th,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spr_report(void){ps("[SPR] Ar: ");pi(st.n_ar);ps(" PCS=");pi(st.t_f1);ps("\nEs: ");pi(st.n_es);ps(" PCS=");pi(st.t_f2);ps("\nTh: ");pi(st.n_th);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spr_state(void){ps("[SPR] Ar=");pi(st.n_ar);ps(" Es=");pi(st.n_es);ps(" Th=");pi(st.n_th);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spray Tech Admin Demo ===\n\n");spr_init();
ps("Air spraying...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spr_air(t,c,339+(i*17),324+(i*14),304+(i*10),286+(i*6),2020+(i%5));}
ps("\nElectrostatic spraying...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spr_electrostatic(t,c,328+(i*15),314+(i*12),296+(i*8),283+(i*5),2021+(i%4));}
ps("\nThermal spraying...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spr_thermal(t,c,320+(i*13),306+(i*10),290+(i*7),279+(i*4),2022+(i%3));}
ps("\nSpray accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spr_accessory(t,c,312+(i*11),300+(i*9),286+(i*6),276+(i*3),2023+(i%2));}
ps("\nSpray marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spr_market(t,c,306+(i*9),295+(i*7),282+(i*5),274+(i*3),2024);}
ps("\n");spr_report();spr_state();ps("\n=== Demo Complete ===\n");return 0;}
