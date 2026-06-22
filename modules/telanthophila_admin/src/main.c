/* telanthophila_admin: Telanthophila management technology administration (v1.0)
 * Telanthophila planning, telanthophila execution, telanthophila evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tel_t;
typedef struct{int n_telp,n_tele,n_tel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tel_state_t;
static tel_t telps[N],telss[N-2],telvss[N-4],telas[N-6],telmks[N-6]; static tel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tel_init(void){if(init)return -1;st.n_telp=0;st.n_tele=0;st.n_tel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)telps[i].active=0;for(int i=0;i<N-2;i++)telss[i].active=0;for(int i=0;i<N-4;i++)telvss[i].active=0;for(int i=0;i<N-6;i++)telas[i].active=0;for(int i=0;i<N-6;i++)telmks[i].active=0;init=1;ps("[TEL] Telanthophila initialized\n");return 0;}
int tel_planning(int t,int c,int a,int b,int d,int e,int y){return add(telps,&st.n_telp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tel_execution(int t,int c,int a,int b,int d,int e,int y){return add(telss,&st.n_tele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(telvss,&st.n_tel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(telas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tel_market(int t,int c,int a,int b,int d,int e,int y){return add(telmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tel_report(void){ps("[TEL] Telpp: ");pi(st.n_telp);ps(" PCS=");pi(st.t_f1);ps("\nTele: ");pi(st.n_tele);ps(" PCS=");pi(st.t_f2);ps("\nTelv: ");pi(st.n_tel2);ps(" PCS=");pi(st.t_f3);ps("\nTelc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tel_state(void){ps("[TEL] Telpp=");pi(st.n_telp);ps(" Tele=");pi(st.n_tele);ps(" Telv=");pi(st.n_tel2);ps(" Telc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Telanthophila Admin Demo ===\n\n");tel_init();
ps("Telanthophila planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tel_planning(t,c,1197+(i*17),1186+(i*14),1166+(i*10),1148+(i*6),2020+(i%5));}
ps("\nTelanthophila execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tel_execution(t,c,1186+(i*15),1175+(i*12),1157+(i*8),1144+(i*5),2021+(i%4));}
ps("\nTelanthophila evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tel_evaluation(t,c,1178+(i*13),1167+(i*10),1151+(i*7),1140+(i*4),2022+(i%3));}
ps("\nTelanthophila accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tel_accessory(t,c,1170+(i*11),1161+(i*9),1147+(i*6),1137+(i*3),2023+(i%2));}
ps("\nTelanthophila marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tel_market(t,c,1164+(i*9),1155+(i*7),1142+(i*5),1134+(i*3),2024);}
ps("\n");tel_report();tel_state();ps("\n=== Demo Complete ===\n");return 0;}
