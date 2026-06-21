/* cinnamon_admin: Cinnamon management technology administration (v1.0)
 * Cinnamon planning, cinnamon execution, cinnamon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cnn_t;
typedef struct{int n_cnnp,n_cnne,n_cnnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cnn_state_t;
static cnn_t cnnps[N],cnns[N-2],cnnvs[N-4],cnnas[N-6],cnnmks[N-6]; static cnn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cnn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CNN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cnn_init(void){if(init)return -1;st.n_cnnp=0;st.n_cnne=0;st.n_cnnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cnnps[i].active=0;for(int i=0;i<N-2;i++)cnns[i].active=0;for(int i=0;i<N-4;i++)cnnvs[i].active=0;for(int i=0;i<N-6;i++)cnnas[i].active=0;for(int i=0;i<N-6;i++)cnnmks[i].active=0;init=1;ps("[CNN] Cinnamon initialized\n");return 0;}
int cnn_planning(int t,int c,int a,int b,int d,int e,int y){return add(cnnps,&st.n_cnnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cnn_execution(int t,int c,int a,int b,int d,int e,int y){return add(cnns,&st.n_cnne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cnn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cnnvs,&st.n_cnnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cnn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cnnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cnn_market(int t,int c,int a,int b,int d,int e,int y){return add(cnnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cnn_report(void){ps("[CNN] Cnnp: ");pi(st.n_cnnp);ps(" PCS=");pi(st.t_f1);ps("\nCnne: ");pi(st.n_cnne);ps(" PCS=");pi(st.t_f2);ps("\nCnnv: ");pi(st.n_cnnv);ps(" PCS=");pi(st.t_f3);ps("\nCnnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cnn_state(void){ps("[CNN] Cnnp=");pi(st.n_cnnp);ps(" Cnne=");pi(st.n_cnne);ps(" Cnnv=");pi(st.n_cnnv);ps(" Cnnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cinnamon Admin Demo ===\n\n");cnn_init();
ps("Cinnamon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cnn_planning(t,c,670+(i*17),659+(i*14),639+(i*10),621+(i*6),2020+(i%5));}
ps("\nCinnamon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cnn_execution(t,c,659+(i*15),648+(i*12),630+(i*8),617+(i*5),2021+(i%4));}
ps("\nCinnamon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cnn_evaluation(t,c,651+(i*13),640+(i*10),624+(i*7),613+(i*4),2022+(i%3));}
ps("\nCinnamon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnn_accessory(t,c,643+(i*11),634+(i*9),620+(i*6),610+(i*3),2023+(i%2));}
ps("\nCinnamon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnn_market(t,c,637+(i*9),628+(i*7),615+(i*5),607+(i*3),2024);}
ps("\n");cnn_report();cnn_state();ps("\n=== Demo Complete ===\n");return 0;}
