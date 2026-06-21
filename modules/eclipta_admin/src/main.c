/* eclipta_admin: Eclipta management technology administration (v1.0)
 * Eclipta planning, eclipta execution, eclipta evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ecl_t;
typedef struct{int n_eclp,n_eclc,n_ecl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ecl_state_t;
static ecl_t eclps[N],eclss[N-2],eclvss[N-4],eclas[N-6],eclmks[N-6]; static ecl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ecl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ecl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ecl_init(void){if(init)return -1;st.n_eclp=0;st.n_eclc=0;st.n_ecl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eclps[i].active=0;for(int i=0;i<N-2;i++)eclss[i].active=0;for(int i=0;i<N-4;i++)eclvss[i].active=0;for(int i=0;i<N-6;i++)eclas[i].active=0;for(int i=0;i<N-6;i++)eclmks[i].active=0;init=1;ps("[ECL] Eclipta initialized\n");return 0;}
int ecl_planning(int t,int c,int a,int b,int d,int e,int y){return add(eclps,&st.n_eclp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ecl_execution(int t,int c,int a,int b,int d,int e,int y){return add(eclss,&st.n_eclc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ecl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eclvss,&st.n_ecl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ecl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eclas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ecl_market(int t,int c,int a,int b,int d,int e,int y){return add(eclmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ecl_report(void){ps("[ECL] Eclp: ");pi(st.n_eclp);ps(" PCS=");pi(st.t_f1);ps("\nEclc: ");pi(st.n_eclc);ps(" PCS=");pi(st.t_f2);ps("\nEclv: ");pi(st.n_ecl2);ps(" PCS=");pi(st.t_f3);ps("\nEclc2: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ecl_state(void){ps("[ECL] Eclp=");pi(st.n_eclp);ps(" Eclc=");pi(st.n_eclc);ps(" Eclv=");pi(st.n_ecl2);ps(" Eclc2=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eclipta Admin Demo ===\n\n");ecl_init();
ps("Eclipta planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ecl_planning(t,c,919+(i*17),908+(i*14),888+(i*10),870+(i*6),2020+(i%5));}
ps("\nEclipta execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ecl_execution(t,c,908+(i*15),897+(i*12),879+(i*8),866+(i*5),2021+(i%4));}
ps("\nEclipta evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ecl_evaluation(t,c,900+(i*13),889+(i*10),873+(i*7),862+(i*4),2022+(i%3));}
ps("\nEclipta accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecl_accessory(t,c,892+(i*11),883+(i*9),869+(i*6),859+(i*3),2023+(i%2));}
ps("\nEclipta marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecl_market(t,c,886+(i*9),877+(i*7),864+(i*5),856+(i*3),2024);}
ps("\n");ecl_report();ecl_state();ps("\n=== Demo Complete ===\n");return 0;}
