/* caladium_admin: Caladium management technology administration (v1.0)
 * Caladium planning, caladium execution, caladium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cad_t;
typedef struct{int n_cadp,n_cade,n_cad2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cad_state_t;
static cad_t cadps[N],cadss[N-2],cadvss[N-4],cadas[N-6],cadmks[N-6]; static cad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cad_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cad_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cad_init(void){if(init)return -1;st.n_cadp=0;st.n_cade=0;st.n_cad2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cadps[i].active=0;for(int i=0;i<N-2;i++)cadss[i].active=0;for(int i=0;i<N-4;i++)cadvss[i].active=0;for(int i=0;i<N-6;i++)cadas[i].active=0;for(int i=0;i<N-6;i++)cadmks[i].active=0;init=1;ps("[CAD] Caladium initialized\n");return 0;}
int cad_planning(int t,int c,int a,int b,int d,int e,int y){return add(cadps,&st.n_cadp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cad_execution(int t,int c,int a,int b,int d,int e,int y){return add(cadss,&st.n_cade,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cad_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cadvss,&st.n_cad2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cad_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cadas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cad_market(int t,int c,int a,int b,int d,int e,int y){return add(cadmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cad_report(void){ps("[CAD] Cadp: ");pi(st.n_cadp);ps(" PCS=");pi(st.t_f1);ps("\nCade: ");pi(st.n_cade);ps(" PCS=");pi(st.t_f2);ps("\nCadv: ");pi(st.n_cad2);ps(" PCS=");pi(st.t_f3);ps("\nCadc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cad_state(void){ps("[CAD] Cadp=");pi(st.n_cadp);ps(" Cade=");pi(st.n_cade);ps(" Cadv=");pi(st.n_cad2);ps(" Cadc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Caladium Admin Demo ===\n\n");cad_init();
ps("Caladium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cad_planning(t,c,903+(i*17),892+(i*14),872+(i*10),854+(i*6),2020+(i%5));}
ps("\nCaladium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cad_execution(t,c,892+(i*15),881+(i*12),863+(i*8),850+(i*5),2021+(i%4));}
ps("\nCaladium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cad_evaluation(t,c,884+(i*13),873+(i*10),857+(i*7),846+(i*4),2022+(i%3));}
ps("\nCaladium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cad_accessory(t,c,876+(i*11),867+(i*9),853+(i*6),843+(i*3),2023+(i%2));}
ps("\nCaladium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cad_market(t,c,870+(i*9),861+(i*7),848+(i*5),840+(i*3),2024);}
ps("\n");cad_report();cad_state();ps("\n=== Demo Complete ===\n");return 0;}
