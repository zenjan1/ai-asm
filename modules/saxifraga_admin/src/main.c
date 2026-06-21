/* saxifraga_admin: Saxifraga management technology administration (v1.0)
 * Saxifraga planning, saxifraga execution, saxifraga evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sax_t;
typedef struct{int n_saxp,n_saxe,n_sax2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sax_state_t;
static sax_t saxps[N],saxss[N-2],saxvss[N-4],saxas[N-6],saxmks[N-6]; static sax_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sax_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sax_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sax_init(void){if(init)return -1;st.n_saxp=0;st.n_saxe=0;st.n_sax2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)saxps[i].active=0;for(int i=0;i<N-2;i++)saxss[i].active=0;for(int i=0;i<N-4;i++)saxvss[i].active=0;for(int i=0;i<N-6;i++)saxas[i].active=0;for(int i=0;i<N-6;i++)saxmks[i].active=0;init=1;ps("[SAX] Saxifraga initialized\n");return 0;}
int sax_planning(int t,int c,int a,int b,int d,int e,int y){return add(saxps,&st.n_saxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sax_execution(int t,int c,int a,int b,int d,int e,int y){return add(saxss,&st.n_saxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sax_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(saxvss,&st.n_sax2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sax_accessory(int t,int c,int a,int b,int d,int e,int y){return add(saxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sax_market(int t,int c,int a,int b,int d,int e,int y){return add(saxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sax_report(void){ps("[SAX] Saxpp: ");pi(st.n_saxp);ps(" PCS=");pi(st.t_f1);ps("\nSaxe: ");pi(st.n_saxe);ps(" PCS=");pi(st.t_f2);ps("\nSaxv: ");pi(st.n_sax2);ps(" PCS=");pi(st.t_f3);ps("\nSaxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sax_state(void){ps("[SAX] Saxpp=");pi(st.n_saxp);ps(" Saxe=");pi(st.n_saxe);ps(" Saxv=");pi(st.n_sax2);ps(" Saxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Saxifraga Admin Demo ===\n\n");sax_init();
ps("Saxifraga planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sax_planning(t,c,1101+(i*17),1090+(i*14),1070+(i*10),1052+(i*6),2020+(i%5));}
ps("\nSaxifraga execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sax_execution(t,c,1090+(i*15),1079+(i*12),1061+(i*8),1048+(i*5),2021+(i%4));}
ps("\nSaxifraga evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sax_evaluation(t,c,1082+(i*13),1071+(i*10),1055+(i*7),1044+(i*4),2022+(i%3));}
ps("\nSaxifraga accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sax_accessory(t,c,1074+(i*11),1065+(i*9),1051+(i*6),1041+(i*3),2023+(i%2));}
ps("\nSaxifraga marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sax_market(t,c,1068+(i*9),1059+(i*7),1046+(i*5),1038+(i*3),2024);}
ps("\n");sax_report();sax_state();ps("\n=== Demo Complete ===\n");return 0;}
