/* epigaea_admin: Epigaea management technology administration (v1.0)
 * Epigaea planning, epigaea execution, epigaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} epi_t;
typedef struct{int n_epip,n_epie,n_epi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} epi_state_t;
static epi_t epips[N],episs[N-2],epivss[N-4],epias[N-6],epimks[N-6]; static epi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(epi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;epi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int epi_init(void){if(init)return -1;st.n_epip=0;st.n_epie=0;st.n_epi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epips[i].active=0;for(int i=0;i<N-2;i++)episs[i].active=0;for(int i=0;i<N-4;i++)epivss[i].active=0;for(int i=0;i<N-6;i++)epias[i].active=0;for(int i=0;i<N-6;i++)epimks[i].active=0;init=1;ps("[EPI] Epigaea initialized\n");return 0;}
int epi_planning(int t,int c,int a,int b,int d,int e,int y){return add(epips,&st.n_epip,&st.t_f1,N,t,c,a,b,d,e,y);}
int epi_execution(int t,int c,int a,int b,int d,int e,int y){return add(episs,&st.n_epie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int epi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(epivss,&st.n_epi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int epi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(epias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int epi_market(int t,int c,int a,int b,int d,int e,int y){return add(epimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void epi_report(void){ps("[EPI] Epip: ");pi(st.n_epip);ps(" PCS=");pi(st.t_f1);ps("\nEpie: ");pi(st.n_epie);ps(" PCS=");pi(st.t_f2);ps("\nEpiv: ");pi(st.n_epi2);ps(" PCS=");pi(st.t_f3);ps("\nEpic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void epi_state(void){ps("[EPI] Epip=");pi(st.n_epip);ps(" Epie=");pi(st.n_epie);ps(" Epiv=");pi(st.n_epi2);ps(" Epic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Epigaea Admin Demo ===\n\n");epi_init();
ps("Epigaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;epi_planning(t,c,921+(i*17),910+(i*14),890+(i*10),872+(i*6),2020+(i%5));}
ps("\nEpigaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;epi_execution(t,c,910+(i*15),899+(i*12),881+(i*8),868+(i*5),2021+(i%4));}
ps("\nEpigaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;epi_evaluation(t,c,902+(i*13),891+(i*10),875+(i*7),864+(i*4),2022+(i%3));}
ps("\nEpigaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epi_accessory(t,c,894+(i*11),885+(i*9),871+(i*6),861+(i*3),2023+(i%2));}
ps("\nEpigaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epi_market(t,c,888+(i*9),879+(i*7),866+(i*5),858+(i*3),2024);}
ps("\n");epi_report();epi_state();ps("\n=== Demo Complete ===\n");return 0;}
