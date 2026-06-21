/* lavandula_admin: Lavandula management technology administration (v1.0)
 * Lavandula planning, lavandula execution, lavandula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lav_t;
typedef struct{int n_lavp,n_lave,n_lav2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lav_state_t;
static lav_t lavps[N],lavss[N-2],lavvss[N-4],lavas[N-6],lavmks[N-6]; static lav_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lav_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lav_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lav_init(void){if(init)return -1;st.n_lavp=0;st.n_lave=0;st.n_lav2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lavps[i].active=0;for(int i=0;i<N-2;i++)lavss[i].active=0;for(int i=0;i<N-4;i++)lavvss[i].active=0;for(int i=0;i<N-6;i++)lavas[i].active=0;for(int i=0;i<N-6;i++)lavmks[i].active=0;init=1;ps("[LAV] Lavandula initialized\n");return 0;}
int lav_planning(int t,int c,int a,int b,int d,int e,int y){return add(lavps,&st.n_lavp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lav_execution(int t,int c,int a,int b,int d,int e,int y){return add(lavss,&st.n_lave,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lav_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lavvss,&st.n_lav2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lav_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lavas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lav_market(int t,int c,int a,int b,int d,int e,int y){return add(lavmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lav_report(void){ps("[LAV] Lavp: ");pi(st.n_lavp);ps(" PCS=");pi(st.t_f1);ps("\nLave: ");pi(st.n_lave);ps(" PCS=");pi(st.t_f2);ps("\nLavv: ");pi(st.n_lav2);ps(" PCS=");pi(st.t_f3);ps("\nLavc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lav_state(void){ps("[LAV] Lavp=");pi(st.n_lavp);ps(" Lave=");pi(st.n_lave);ps(" Lavv=");pi(st.n_lav2);ps(" Lavc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lavandula Admin Demo ===\n\n");lav_init();
ps("Lavandula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lav_planning(t,c,1046+(i*17),1035+(i*14),1015+(i*10),997+(i*6),2020+(i%5));}
ps("\nLavandula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lav_execution(t,c,1035+(i*15),1024+(i*12),1006+(i*8),993+(i*5),2021+(i%4));}
ps("\nLavandula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lav_evaluation(t,c,1027+(i*13),1016+(i*10),1000+(i*7),989+(i*4),2022+(i%3));}
ps("\nLavandula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lav_accessory(t,c,1019+(i*11),1010+(i*9),996+(i*6),986+(i*3),2023+(i%2));}
ps("\nLavandula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lav_market(t,c,1013+(i*9),1004+(i*7),991+(i*5),983+(i*3),2024);}
ps("\n");lav_report();lav_state();ps("\n=== Demo Complete ===\n");return 0;}
