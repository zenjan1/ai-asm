/* lapageria_admin: Lapageria management technology administration (v1.0)
 * Lapageria planning, lapageria execution, lapageria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lap_t;
typedef struct{int n_lapp,n_lape,n_lap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lap_state_t;
static lap_t lapss[N],lapess[N-2],lapvss[N-4],lapas[N-6],lapmks[N-6]; static lap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lap_init(void){if(init)return -1;st.n_lapp=0;st.n_lape=0;st.n_lap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lapss[i].active=0;for(int i=0;i<N-2;i++)lapess[i].active=0;for(int i=0;i<N-4;i++)lapvss[i].active=0;for(int i=0;i<N-6;i++)lapas[i].active=0;for(int i=0;i<N-6;i++)lapmks[i].active=0;init=1;ps("[LAP] Lapageria initialized\n");return 0;}
int lap_planning(int t,int c,int a,int b,int d,int e,int y){return add(lapss,&st.n_lapp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lap_execution(int t,int c,int a,int b,int d,int e,int y){return add(lapess,&st.n_lape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lapvss,&st.n_lap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lapas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lap_market(int t,int c,int a,int b,int d,int e,int y){return add(lapmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lap_report(void){ps("[LAP] Lapp: ");pi(st.n_lapp);ps(" PCS=");pi(st.t_f1);ps("\nLape: ");pi(st.n_lape);ps(" PCS=");pi(st.t_f2);ps("\nLapv: ");pi(st.n_lap2);ps(" PCS=");pi(st.t_f3);ps("\nLapc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lap_state(void){ps("[LAP] Lapp=");pi(st.n_lapp);ps(" Lape=");pi(st.n_lape);ps(" Lapv=");pi(st.n_lap2);ps(" Lapc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lapageria Admin Demo ===\n\n");lap_init();
ps("Lapageria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lap_planning(t,c,1043+(i*17),1032+(i*14),1012+(i*10),994+(i*6),2020+(i%5));}
ps("\nLapageria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lap_execution(t,c,1032+(i*15),1021+(i*12),1003+(i*8),990+(i*5),2021+(i%4));}
ps("\nLapageria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lap_evaluation(t,c,1024+(i*13),1013+(i*10),997+(i*7),986+(i*4),2022+(i%3));}
ps("\nLapageria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lap_accessory(t,c,1016+(i*11),1007+(i*9),993+(i*6),983+(i*3),2023+(i%2));}
ps("\nLapageria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lap_market(t,c,1010+(i*9),1001+(i*7),988+(i*5),980+(i*3),2024);}
ps("\n");lap_report();lap_state();ps("\n=== Demo Complete ===\n");return 0;}
