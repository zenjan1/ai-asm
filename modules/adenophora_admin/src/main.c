/* adenophora_admin: Adenophora management technology administration (v1.0)
 * Adenophora planning, adenophora execution, adenophora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} adn_t;
typedef struct{int n_adnp,n_adne,n_adn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} adn_state_t;
static adn_t adnps[N],adnss[N-2],adnvss[N-4],adnas[N-6],adnmks[N-6]; static adn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(adn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;adn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ADN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int adn_init(void){if(init)return -1;st.n_adnp=0;st.n_adne=0;st.n_adn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)adnps[i].active=0;for(int i=0;i<N-2;i++)adnss[i].active=0;for(int i=0;i<N-4;i++)adnvss[i].active=0;for(int i=0;i<N-6;i++)adnas[i].active=0;for(int i=0;i<N-6;i++)adnmks[i].active=0;init=1;ps("[ADN] Adenophora initialized\n");return 0;}
int adn_planning(int t,int c,int a,int b,int d,int e,int y){return add(adnps,&st.n_adnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int adn_execution(int t,int c,int a,int b,int d,int e,int y){return add(adnss,&st.n_adne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int adn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(adnvss,&st.n_adn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int adn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(adnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int adn_market(int t,int c,int a,int b,int d,int e,int y){return add(adnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void adn_report(void){ps("[ADN] Adepp: ");pi(st.n_adnp);ps(" PCS=");pi(st.t_f1);ps("\nAdepe: ");pi(st.n_adne);ps(" PCS=");pi(st.t_f2);ps("\nAdev: ");pi(st.n_adn2);ps(" PCS=");pi(st.t_f3);ps("\nAdec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void adn_state(void){ps("[ADN] Adepp=");pi(st.n_adnp);ps(" Adepe=");pi(st.n_adne);ps(" Adev=");pi(st.n_adn2);ps(" Adec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Adenophora Admin Demo ===\n\n");adn_init();
ps("Adenophora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;adn_planning(t,c,1214+(i*17),1203+(i*14),1183+(i*10),1165+(i*6),2020+(i%5));}
ps("\nAdenophora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;adn_execution(t,c,1203+(i*15),1192+(i*12),1174+(i*8),1161+(i*5),2021+(i%4));}
ps("\nAdenophora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;adn_evaluation(t,c,1195+(i*13),1184+(i*10),1168+(i*7),1157+(i*4),2022+(i%3));}
ps("\nAdenophora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adn_accessory(t,c,1187+(i*11),1178+(i*9),1164+(i*6),1154+(i*3),2023+(i%2));}
ps("\nAdenophora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adn_market(t,c,1181+(i*9),1172+(i*7),1159+(i*5),1151+(i*3),2024);}
ps("\n");adn_report();adn_state();ps("\n=== Demo Complete ===\n");return 0;}
