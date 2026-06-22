/* coeloglossus_admin: Coeloglossus management technology administration (v1.0)
 * Coeloglossus planning, coeloglossus execution, coeloglossus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ogl_t;
typedef struct{int n_oglp,n_ogle,n_ogl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ogl_state_t;
static ogl_t oglps[N],oglss[N-2],oglvss[N-4],oglas[N-6],oglmks[N-6]; static ogl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ogl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ogl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OGL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ogl_init(void){if(init)return -1;st.n_oglp=0;st.n_ogle=0;st.n_ogl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oglps[i].active=0;for(int i=0;i<N-2;i++)oglss[i].active=0;for(int i=0;i<N-4;i++)oglvss[i].active=0;for(int i=0;i<N-6;i++)oglas[i].active=0;for(int i=0;i<N-6;i++)oglmks[i].active=0;init=1;ps("[OGL] Coeloglossus initialized\n");return 0;}
int ogl_planning(int t,int c,int a,int b,int d,int e,int y){return add(oglps,&st.n_oglp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ogl_execution(int t,int c,int a,int b,int d,int e,int y){return add(oglss,&st.n_ogle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ogl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oglvss,&st.n_ogl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ogl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oglas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ogl_market(int t,int c,int a,int b,int d,int e,int y){return add(oglmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ogl_report(void){ps("[OGL] Coepp: ");pi(st.n_oglp);ps(" PCS=");pi(st.t_f1);ps("\nCoepe: ");pi(st.n_ogle);ps(" PCS=");pi(st.t_f2);ps("\nCoev: ");pi(st.n_ogl2);ps(" PCS=");pi(st.t_f3);ps("\nCoec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ogl_state(void){ps("[OGL] Coepp=");pi(st.n_oglp);ps(" Coepe=");pi(st.n_ogle);ps(" Coev=");pi(st.n_ogl2);ps(" Coec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coeloglossus Admin Demo ===\n\n");ogl_init();
ps("Coeloglossus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ogl_planning(t,c,1241+(i*17),1230+(i*14),1210+(i*10),1192+(i*6),2020+(i%5));}
ps("\nCoeloglossus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ogl_execution(t,c,1230+(i*15),1219+(i*12),1201+(i*8),1188+(i*5),2021+(i%4));}
ps("\nCoeloglossus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ogl_evaluation(t,c,1222+(i*13),1211+(i*10),1195+(i*7),1184+(i*4),2022+(i%3));}
ps("\nCoeloglossus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ogl_accessory(t,c,1214+(i*11),1205+(i*9),1191+(i*6),1181+(i*3),2023+(i%2));}
ps("\nCoeloglossus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ogl_market(t,c,1208+(i*9),1199+(i*7),1186+(i*5),1178+(i*3),2024);}
ps("\n");ogl_report();ogl_state();ps("\n=== Demo Complete ===\n");return 0;}
