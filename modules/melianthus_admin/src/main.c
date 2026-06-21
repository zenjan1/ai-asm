/* melianthus_admin: Melianthus management technology administration (v1.0)
 * Melianthus planning, melianthus execution, melianthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mhs_t;
typedef struct{int n_msp,n_mse,n_msv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mhs_state_t;
static mhs_t mhsps[N],mhsss[N-2],mhs_vss[N-4],mhs_as[N-6],mhs_ms[N-6]; static mhs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mhs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mhs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MHS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mhs_init(void){if(init)return -1;st.n_msp=0;st.n_mse=0;st.n_msv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mhsps[i].active=0;for(int i=0;i<N-2;i++)mhsss[i].active=0;for(int i=0;i<N-4;i++)mhs_vss[i].active=0;for(int i=0;i<N-6;i++)mhs_as[i].active=0;for(int i=0;i<N-6;i++)mhs_ms[i].active=0;init=1;ps("[MHS] Melianthus initialized\n");return 0;}
int mhs_planning(int t,int c,int a,int b,int d,int e,int y){return add(mhsps,&st.n_msp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mhs_execution(int t,int c,int a,int b,int d,int e,int y){return add(mhsss,&st.n_mse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mhs_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mhs_vss,&st.n_msv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mhs_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mhs_as,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mhs_market(int t,int c,int a,int b,int d,int e,int y){return add(mhs_ms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mhs_report(void){ps("[MHS] Msp: ");pi(st.n_msp);ps(" PCS=");pi(st.t_f1);ps("\nMse: ");pi(st.n_mse);ps(" PCS=");pi(st.t_f2);ps("\nMsv: ");pi(st.n_msv);ps(" PCS=");pi(st.t_f3);ps("\nMsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mhs_state(void){ps("[MHS] Msp=");pi(st.n_msp);ps(" Mse=");pi(st.n_mse);ps(" Msv=");pi(st.n_msv);ps(" Msc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Melianthus Admin Demo ===\n\n");mhs_init();
ps("Melianthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mhs_planning(t,c,838+(i*17),827+(i*14),807+(i*10),789+(i*6),2020+(i%5));}
ps("\nMelianthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mhs_execution(t,c,827+(i*15),816+(i*12),798+(i*8),785+(i*5),2021+(i%4));}
ps("\nMelianthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mhs_evaluation(t,c,819+(i*13),808+(i*10),792+(i*7),781+(i*4),2022+(i%3));}
ps("\nMelianthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mhs_accessory(t,c,811+(i*11),802+(i*9),788+(i*6),778+(i*3),2023+(i%2));}
ps("\nMelianthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mhs_market(t,c,805+(i*9),796+(i*7),783+(i*5),775+(i*3),2024);}
ps("\n");mhs_report();mhs_state();ps("\n=== Demo Complete ===\n");return 0;}
