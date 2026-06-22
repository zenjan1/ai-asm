/* cacalia_admin: Cacalia management technology administration (v1.0)
 * Cacalia planning, cacalia execution, cacalia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} caa_t;
typedef struct{int n_caap,n_caae,n_caa2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} caa_state_t;
static caa_t caaps[N],caass[N-2],caavss[N-4],caaas[N-6],caamks[N-6]; static caa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(caa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;caa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int caa_init(void){if(init)return -1;st.n_caap=0;st.n_caae=0;st.n_caa2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)caaps[i].active=0;for(int i=0;i<N-2;i++)caass[i].active=0;for(int i=0;i<N-4;i++)caavss[i].active=0;for(int i=0;i<N-6;i++)caaas[i].active=0;for(int i=0;i<N-6;i++)caamks[i].active=0;init=1;ps("[CAA] Cacalia initialized\n");return 0;}
int caa_planning(int t,int c,int a,int b,int d,int e,int y){return add(caaps,&st.n_caap,&st.t_f1,N,t,c,a,b,d,e,y);}
int caa_execution(int t,int c,int a,int b,int d,int e,int y){return add(caass,&st.n_caae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int caa_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(caavss,&st.n_caa2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int caa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(caaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int caa_market(int t,int c,int a,int b,int d,int e,int y){return add(caamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void caa_report(void){ps("[CAA] Cacpp: ");pi(st.n_caap);ps(" PCS=");pi(st.t_f1);ps("\nCacpe: ");pi(st.n_caae);ps(" PCS=");pi(st.t_f2);ps("\nCacv: ");pi(st.n_caa2);ps(" PCS=");pi(st.t_f3);ps("\nCacc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void caa_state(void){ps("[CAA] Cacpp=");pi(st.n_caap);ps(" Cacpe=");pi(st.n_caae);ps(" Cacv=");pi(st.n_caa2);ps(" Cacc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cacalia Admin Demo ===\n\n");caa_init();
ps("Cacalia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;caa_planning(t,c,1227+(i*17),1216+(i*14),1196+(i*10),1178+(i*6),2020+(i%5));}
ps("\nCacalia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;caa_execution(t,c,1216+(i*15),1205+(i*12),1187+(i*8),1174+(i*5),2021+(i%4));}
ps("\nCacalia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;caa_evaluation(t,c,1208+(i*13),1197+(i*10),1181+(i*7),1170+(i*4),2022+(i%3));}
ps("\nCacalia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;caa_accessory(t,c,1200+(i*11),1191+(i*9),1177+(i*6),1167+(i*3),2023+(i%2));}
ps("\nCacalia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;caa_market(t,c,1194+(i*9),1185+(i*7),1172+(i*5),1164+(i*3),2024);}
ps("\n");caa_report();caa_state();ps("\n=== Demo Complete ===\n");return 0;}
