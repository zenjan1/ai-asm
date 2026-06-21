/* oenothera_admin: Oenothera management technology administration (v1.0)
 * Oenothera planning, oenothera execution, oenothera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oc_t;
typedef struct{int n_ocp,n_oe,n_ocv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oc_state_t;
static oc_t ocps[N],ocss[N-2],ocvss[N-4],ocas[N-6],ocmks[N-6]; static oc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ocn_init(void){if(init)return -1;st.n_ocp=0;st.n_oe=0;st.n_ocv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ocps[i].active=0;for(int i=0;i<N-2;i++)ocss[i].active=0;for(int i=0;i<N-4;i++)ocvss[i].active=0;for(int i=0;i<N-6;i++)ocas[i].active=0;for(int i=0;i<N-6;i++)ocmks[i].active=0;init=1;ps("[OCN] Oenothera initialized\n");return 0;}
int ocn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ocps,&st.n_ocp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ocn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ocss,&st.n_oe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ocn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ocvss,&st.n_ocv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ocn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ocas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ocn_market(int t,int c,int a,int b,int d,int e,int y){return add(ocmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ocn_report(void){ps("[OCN] Ocp: ");pi(st.n_ocp);ps(" PCS=");pi(st.t_f1);ps("\nOe: ");pi(st.n_oe);ps(" PCS=");pi(st.t_f2);ps("\nOcv: ");pi(st.n_ocv);ps(" PCS=");pi(st.t_f3);ps("\nOcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ocn_state(void){ps("[OCN] Ocp=");pi(st.n_ocp);ps(" Oe=");pi(st.n_oe);ps(" Ocv=");pi(st.n_ocv);ps(" Occ=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oenothera Admin Demo ===\n\n");ocn_init();
ps("Oenothera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ocn_planning(t,c,748+(i*17),737+(i*14),717+(i*10),699+(i*6),2020+(i%5));}
ps("\nOenothera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ocn_execution(t,c,737+(i*15),726+(i*12),708+(i*8),695+(i*5),2021+(i%4));}
ps("\nOenothera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ocn_evaluation(t,c,729+(i*13),718+(i*10),702+(i*7),691+(i*4),2022+(i%3));}
ps("\nOenothera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ocn_accessory(t,c,721+(i*11),712+(i*9),698+(i*6),688+(i*3),2023+(i%2));}
ps("\nOenothera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ocn_market(t,c,715+(i*9),706+(i*7),693+(i*5),685+(i*3),2024);}
ps("\n");ocn_report();ocn_state();ps("\n=== Demo Complete ===\n");return 0;}
