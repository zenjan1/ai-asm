/* epimedium_admin: Epimedium management technology administration (v1.0)
 * Epimedium planning, epimedium execution, epimedium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ep_t;
typedef struct{int n_epp,n_epe,n_epv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ep_state_t;
static ep_t epps[N],epss[N-2],epvss[N-4],epas[N-6],epmks[N-6]; static ep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ep_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ep_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int epm_init(void){if(init)return -1;st.n_epp=0;st.n_epe=0;st.n_epv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epps[i].active=0;for(int i=0;i<N-2;i++)epss[i].active=0;for(int i=0;i<N-4;i++)epvss[i].active=0;for(int i=0;i<N-6;i++)epas[i].active=0;for(int i=0;i<N-6;i++)epmks[i].active=0;init=1;ps("[EPM] Epimedium initialized\n");return 0;}
int epm_planning(int t,int c,int a,int b,int d,int e,int y){return add(epps,&st.n_epp,&st.t_f1,N,t,c,a,b,d,e,y);}
int epm_execution(int t,int c,int a,int b,int d,int e,int y){return add(epss,&st.n_epe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int epm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(epvss,&st.n_epv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int epm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(epas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int epm_market(int t,int c,int a,int b,int d,int e,int y){return add(epmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void epm_report(void){ps("[EPM] Epp: ");pi(st.n_epp);ps(" PCS=");pi(st.t_f1);ps("\nEpe: ");pi(st.n_epe);ps(" PCS=");pi(st.t_f2);ps("\nEpv: ");pi(st.n_epv);ps(" PCS=");pi(st.t_f3);ps("\nEpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void epm_state(void){ps("[EPM] Epp=");pi(st.n_epp);ps(" Epe=");pi(st.n_epe);ps(" Epv=");pi(st.n_epv);ps(" Epc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Epimedium Admin Demo ===\n\n");epm_init();
ps("Epimedium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;epm_planning(t,c,768+(i*17),757+(i*14),737+(i*10),719+(i*6),2020+(i%5));}
ps("\nEpimedium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;epm_execution(t,c,757+(i*15),746+(i*12),728+(i*8),715+(i*5),2021+(i%4));}
ps("\nEpimedium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;epm_evaluation(t,c,749+(i*13),738+(i*10),724+(i*7),713+(i*4),2022+(i%3));}
ps("\nEpimedium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epm_accessory(t,c,741+(i*11),732+(i*9),718+(i*6),708+(i*3),2023+(i%2));}
ps("\nEpimedium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epm_market(t,c,735+(i*9),726+(i*7),713+(i*5),705+(i*3),2024);}
ps("\n");epm_report();epm_state();ps("\n=== Demo Complete ===\n");return 0;}
