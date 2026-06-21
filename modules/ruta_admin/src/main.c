/* ruta_admin: Ruta management technology administration (v1.0)
 * Ruta planning, ruta execution, ruta evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rut_t;
typedef struct{int n_rutp,n_rute,n_rut2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rut_state_t;
static rut_t rutps[N],rutss[N-2],rutvss[N-4],rutas[N-6],rutmks[N-6]; static rut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rut_init(void){if(init)return -1;st.n_rutp=0;st.n_rute=0;st.n_rut2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rutps[i].active=0;for(int i=0;i<N-2;i++)rutss[i].active=0;for(int i=0;i<N-4;i++)rutvss[i].active=0;for(int i=0;i<N-6;i++)rutas[i].active=0;for(int i=0;i<N-6;i++)rutmks[i].active=0;init=1;ps("[RUT] Ruta initialized\n");return 0;}
int rut_planning(int t,int c,int a,int b,int d,int e,int y){return add(rutps,&st.n_rutp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rut_execution(int t,int c,int a,int b,int d,int e,int y){return add(rutss,&st.n_rute,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rut_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rutvss,&st.n_rut2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rutas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rut_market(int t,int c,int a,int b,int d,int e,int y){return add(rutmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rut_report(void){ps("[RUT] rutp: ");pi(st.n_rutp);ps(" PCS=");pi(st.t_f1);ps("\nrute: ");pi(st.n_rute);ps(" PCS=");pi(st.t_f2);ps("\nrutv: ");pi(st.n_rut2);ps(" PCS=");pi(st.t_f3);ps("\nrutc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rut_state(void){ps("[RUT] rutp=");pi(st.n_rutp);ps(" rute=");pi(st.n_rute);ps(" rutv=");pi(st.n_rut2);ps(" rutc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ruta Admin Demo ===\n\n");rut_init();
ps("Ruta planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rut_planning(t,c,1097+(i*17),1086+(i*14),1066+(i*10),1048+(i*6),2020+(i%5));}
ps("\nRuta execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rut_execution(t,c,1086+(i*15),1075+(i*12),1057+(i*8),1044+(i*5),2021+(i%4));}
ps("\nRuta evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rut_evaluation(t,c,1078+(i*13),1067+(i*10),1051+(i*7),1040+(i*4),2022+(i%3));}
ps("\nRuta accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rut_accessory(t,c,1070+(i*11),1061+(i*9),1047+(i*6),1037+(i*3),2023+(i%2));}
ps("\nRuta marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rut_market(t,c,1064+(i*9),1055+(i*7),1042+(i*5),1034+(i*3),2024);}
ps("\n");rut_report();rut_state();ps("\n=== Demo Complete ===\n");return 0;}
