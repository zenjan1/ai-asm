/* quercus_admin: Quercus management technology administration (v1.0)
 * Quercus planning, quercus execution, quercus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} qrc_t;
typedef struct{int n_qrp,n_qre,n_qrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} qrc_state_t;
static qrc_t qrcps[N],qrcss[N-2],qrcvss[N-4],qrcas[N-6],qrcmks[N-6]; static qrc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qrc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;qrc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[QRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int qrc_init(void){if(init)return -1;st.n_qrp=0;st.n_qre=0;st.n_qrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)qrcps[i].active=0;for(int i=0;i<N-2;i++)qrcss[i].active=0;for(int i=0;i<N-4;i++)qrcvss[i].active=0;for(int i=0;i<N-6;i++)qrcas[i].active=0;for(int i=0;i<N-6;i++)qrcmks[i].active=0;init=1;ps("[QRC] Quercus initialized\n");return 0;}
int qrc_planning(int t,int c,int a,int b,int d,int e,int y){return add(qrcps,&st.n_qrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int qrc_execution(int t,int c,int a,int b,int d,int e,int y){return add(qrcss,&st.n_qre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int qrc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(qrcvss,&st.n_qrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int qrc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(qrcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int qrc_market(int t,int c,int a,int b,int d,int e,int y){return add(qrcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void qrc_report(void){ps("[QRC] Qrp: ");pi(st.n_qrp);ps(" PCS=");pi(st.t_f1);ps("\nQre: ");pi(st.n_qre);ps(" PCS=");pi(st.t_f2);ps("\nQrv: ");pi(st.n_qrv);ps(" PCS=");pi(st.t_f3);ps("\nQrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void qrc_state(void){ps("[QRC] Qrp=");pi(st.n_qrp);ps(" Qre=");pi(st.n_qre);ps(" Qrv=");pi(st.n_qrv);ps(" Qrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Quercus Admin Demo ===\n\n");qrc_init();
ps("Quercus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;qrc_planning(t,c,854+(i*17),843+(i*14),823+(i*10),805+(i*6),2020+(i%5));}
ps("\nQuercus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;qrc_execution(t,c,843+(i*15),832+(i*12),814+(i*8),801+(i*5),2021+(i%4));}
ps("\nQuercus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;qrc_evaluation(t,c,835+(i*13),824+(i*10),808+(i*7),797+(i*4),2022+(i%3));}
ps("\nQuercus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qrc_accessory(t,c,827+(i*11),818+(i*9),804+(i*6),794+(i*3),2023+(i%2));}
ps("\nQuercus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qrc_market(t,c,821+(i*9),812+(i*7),799+(i*5),791+(i*3),2024);}
ps("\n");qrc_report();qrc_state();ps("\n=== Demo Complete ===\n");return 0;}
