/* hieracium_admin: Hieracium management technology administration (v1.0)
 * Hieracium planning, hieracium execution, hieracium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hr_t;
typedef struct{int n_hrp,n_hre,n_hrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hr_state_t;
static hr_t hrps[N],hrss[N-2],hrvss[N-4],hras[N-6],hrmks[N-6]; static hr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hrc_init(void){if(init)return -1;st.n_hrp=0;st.n_hre=0;st.n_hrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hrps[i].active=0;for(int i=0;i<N-2;i++)hrss[i].active=0;for(int i=0;i<N-4;i++)hrvss[i].active=0;for(int i=0;i<N-6;i++)hras[i].active=0;for(int i=0;i<N-6;i++)hrmks[i].active=0;init=1;ps("[HRC] Hieracium initialized\n");return 0;}
int hrc_planning(int t,int c,int a,int b,int d,int e,int y){return add(hrps,&st.n_hrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hrc_execution(int t,int c,int a,int b,int d,int e,int y){return add(hrss,&st.n_hre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hrc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hrvss,&st.n_hrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hrc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hrc_market(int t,int c,int a,int b,int d,int e,int y){return add(hrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hrc_report(void){ps("[HRC] Hrp: ");pi(st.n_hrp);ps(" PCS=");pi(st.t_f1);ps("\nHre: ");pi(st.n_hre);ps(" PCS=");pi(st.t_f2);ps("\nHrv: ");pi(st.n_hrv);ps(" PCS=");pi(st.t_f3);ps("\nHrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hrc_state(void){ps("[HRC] Hrp=");pi(st.n_hrp);ps(" Hre=");pi(st.n_hre);ps(" Hrv=");pi(st.n_hrv);ps(" Hrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hieracium Admin Demo ===\n\n");hrc_init();
ps("Hieracium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hrc_planning(t,c,785+(i*17),774+(i*14),754+(i*10),736+(i*6),2020+(i%5));}
ps("\nHieracium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hrc_execution(t,c,774+(i*15),763+(i*12),745+(i*8),732+(i*5),2021+(i%4));}
ps("\nHieracium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hrc_evaluation(t,c,766+(i*13),755+(i*10),741+(i*7),730+(i*4),2022+(i%3));}
ps("\nHieracium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrc_accessory(t,c,758+(i*11),749+(i*9),735+(i*6),725+(i*3),2023+(i%2));}
ps("\nHieracium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrc_market(t,c,752+(i*9),743+(i*7),730+(i*5),722+(i*3),2024);}
ps("\n");hrc_report();hrc_state();ps("\n=== Demo Complete ===\n");return 0;}
