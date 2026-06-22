/* tofieldia_admin: Tofieldia management technology administration (v1.0)
 * Tofieldia planning, tofieldia execution, tofieldia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tof_t;
typedef struct{int n_tofp,n_tofe,n_tof2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tof_state_t;
static tof_t tofps[N],tofss[N-2],tofvss[N-4],tofas[N-6],tofmks[N-6]; static tof_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tof_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tof_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tof_init(void){if(init)return -1;st.n_tofp=0;st.n_tofe=0;st.n_tof2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tofps[i].active=0;for(int i=0;i<N-2;i++)tofss[i].active=0;for(int i=0;i<N-4;i++)tofvss[i].active=0;for(int i=0;i<N-6;i++)tofas[i].active=0;for(int i=0;i<N-6;i++)tofmks[i].active=0;init=1;ps("[TOF] Tofieldia initialized\n");return 0;}
int tof_planning(int t,int c,int a,int b,int d,int e,int y){return add(tofps,&st.n_tofp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tof_execution(int t,int c,int a,int b,int d,int e,int y){return add(tofss,&st.n_tofe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tof_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tofvss,&st.n_tof2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tof_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tofas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tof_market(int t,int c,int a,int b,int d,int e,int y){return add(tofmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tof_report(void){ps("[TOF] Tofpp: ");pi(st.n_tofp);ps(" PCS=");pi(st.t_f1);ps("\nTofe: ");pi(st.n_tofe);ps(" PCS=");pi(st.t_f2);ps("\nTofv: ");pi(st.n_tof2);ps(" PCS=");pi(st.t_f3);ps("\nTofc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tof_state(void){ps("[TOF] Tofpp=");pi(st.n_tofp);ps(" Tofe=");pi(st.n_tofe);ps(" Tofv=");pi(st.n_tof2);ps(" Tofc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tofieldia Admin Demo ===\n\n");tof_init();
ps("Tofieldia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tof_planning(t,c,1107+(i*17),1096+(i*14),1076+(i*10),1058+(i*6),2020+(i%5));}
ps("\nTofieldia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tof_execution(t,c,1096+(i*15),1085+(i*12),1067+(i*8),1054+(i*5),2021+(i%4));}
ps("\nTofieldia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tof_evaluation(t,c,1088+(i*13),1077+(i*10),1061+(i*7),1050+(i*4),2022+(i%3));}
ps("\nTofieldia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tof_accessory(t,c,1080+(i*11),1071+(i*9),1057+(i*6),1047+(i*3),2023+(i%2));}
ps("\nTofieldia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tof_market(t,c,1074+(i*9),1065+(i*7),1052+(i*5),1044+(i*3),2024);}
ps("\n");tof_report();tof_state();ps("\n=== Demo Complete ===\n");return 0;}
