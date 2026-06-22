/* xylosmopsis_admin: Xylosmopsis management technology administration (v1.0)
 * Xylosmopsis planning, xylosmopsis execution, xylosmopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xylo_t;
typedef struct{int n_xylop,n_xylo,n_xylo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xylo_state_t;
static xylo_t xylos[N],xyloe[N-2],xylov[N-4],xyloac[N-6],xylom[N-6]; static xylo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(xylo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xylo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XYLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xylo_init(void){if(init)return -1;st.n_xylop=0;st.n_xylo=0;st.n_xylo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xylos[i].active=0;for(int i=0;i<N-2;i++)xyloe[i].active=0;for(int i=0;i<N-4;i++)xylov[i].active=0;for(int i=0;i<N-6;i++)xyloac[i].active=0;for(int i=0;i<N-6;i++)xylom[i].active=0;init=1;ps("[XYLO] Xylosmopsis initialized\n");return 0;}
int xylo_planning(int t,int c,int a,int b,int d,int e,int y){return add(xylos,&st.n_xylop,&st.t_f1,N,t,c,a,b,d,e,y);}
int xylo_execution(int t,int c,int a,int b,int d,int e,int y){return add(xyloe,&st.n_xylo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xylo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xylov,&st.n_xylo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xylo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xyloac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xylo_market(int t,int c,int a,int b,int d,int e,int y){return add(xylom,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xylo_report(void){ps("[XYLO] Xylop: ");pi(st.n_xylop);ps(" PCS=");pi(st.t_f1);ps("\nXyloe: ");pi(st.n_xylo);ps(" PCS=");pi(st.t_f2);ps("\nXylov: ");pi(st.n_xylo2);ps(" PCS=");pi(st.t_f3);ps("\nXyloc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xylo_state(void){ps("[XYLO] Xylop=");pi(st.n_xylop);ps(" Xyloe=");pi(st.n_xylo);ps(" Xylov=");pi(st.n_xylo2);ps(" Xyloc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xylosmopsis Admin Demo ===\n\n");xylo_init();
ps("Xylosmopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xylo_planning(t,c,1376+(i*17),1365+(i*14),1345+(i*10),1327+(i*6),2020+(i%5));}
ps("\nXylosmopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xylo_execution(t,c,1365+(i*15),1354+(i*12),1336+(i*8),1323+(i*5),2021+(i%4));}
ps("\nXylosmopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xylo_evaluation(t,c,1357+(i*13),1346+(i*10),1330+(i*7),1319+(i*4),2022+(i%3));}
ps("\nXylosmopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xylo_accessory(t,c,1349+(i*11),1340+(i*9),1326+(i*6),1316+(i*3),2023+(i%2));}
ps("\nXylosmopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xylo_market(t,c,1343+(i*9),1334+(i*7),1321+(i*5),1313+(i*3),2024);}
ps("\n");xylo_report();xylo_state();ps("\n=== Demo Complete ===\n");return 0;}
