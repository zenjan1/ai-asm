/* cephalanthus_admin: Cephalanthus management technology administration (v1.0)
 * Cephalanthus planning, cephalanthus execution, cephalanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cph_t;
typedef struct{int n_cphp,n_cphe,n_cph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cph_state_t;
static cph_t cphps[N],cphss[N-2],cphvss[N-4],cphas[N-6],cphmks[N-6]; static cph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cph_init(void){if(init)return -1;st.n_cphp=0;st.n_cphe=0;st.n_cph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cphps[i].active=0;for(int i=0;i<N-2;i++)cphss[i].active=0;for(int i=0;i<N-4;i++)cphvss[i].active=0;for(int i=0;i<N-6;i++)cphas[i].active=0;for(int i=0;i<N-6;i++)cphmks[i].active=0;init=1;ps("[CPH] Cephalanthus initialized\n");return 0;}
int cph_planning(int t,int c,int a,int b,int d,int e,int y){return add(cphps,&st.n_cphp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cph_execution(int t,int c,int a,int b,int d,int e,int y){return add(cphss,&st.n_cphe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cphvss,&st.n_cph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cphas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cph_market(int t,int c,int a,int b,int d,int e,int y){return add(cphmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cph_report(void){ps("[CPH] Ceppp: ");pi(st.n_cphp);ps(" PCS=");pi(st.t_f1);ps("\nCeppe: ");pi(st.n_cphe);ps(" PCS=");pi(st.t_f2);ps("\nCepv: ");pi(st.n_cph2);ps(" PCS=");pi(st.t_f3);ps("\nCepc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cph_state(void){ps("[CPH] Ceppp=");pi(st.n_cphp);ps(" Ceppe=");pi(st.n_cphe);ps(" Cepv=");pi(st.n_cph2);ps(" Cepc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cephalanthus Admin Demo ===\n\n");cph_init();
ps("Cephalanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cph_planning(t,c,1232+(i*17),1221+(i*14),1201+(i*10),1183+(i*6),2020+(i%5));}
ps("\nCephalanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cph_execution(t,c,1221+(i*15),1210+(i*12),1192+(i*8),1179+(i*5),2021+(i%4));}
ps("\nCephalanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cph_evaluation(t,c,1213+(i*13),1202+(i*10),1186+(i*7),1175+(i*4),2022+(i%3));}
ps("\nCephalanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cph_accessory(t,c,1205+(i*11),1196+(i*9),1182+(i*6),1172+(i*3),2023+(i%2));}
ps("\nCephalanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cph_market(t,c,1199+(i*9),1190+(i*7),1177+(i*5),1169+(i*3),2024);}
ps("\n");cph_report();cph_state();ps("\n=== Demo Complete ===\n");return 0;}
