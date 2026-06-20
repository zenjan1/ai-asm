/* aerotech_admin: Aerospace technology administration (v1.0)
 * Aircraft manufacturing, aviation engines, avionics, aviation services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aer_t;
typedef struct{int n_ac,n_ae,n_av,n_as,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aer_state_t;
static aer_t acs[N],aes[N-2],avs[N-4],ass[N-6],mks[N-6]; static aer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aer_init(void){if(init)return -1;st.n_ac=0;st.n_ae=0;st.n_av=0;st.n_as=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acs[i].active=0;for(int i=0;i<N-2;i++)aes[i].active=0;for(int i=0;i<N-4;i++)avs[i].active=0;for(int i=0;i<N-6;i++)ass[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AER] Aerotech initialized\n");return 0;}
int aer_aircraft(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f1,N,t,c,a,b,d,e,y);}
int aer_engine(int t,int c,int a,int b,int d,int e,int y){return add(aes,&st.n_ae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aer_avionics(int t,int c,int a,int b,int d,int e,int y){return add(avs,&st.n_av,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aer_service(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aer_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aer_report(void){ps("[AER] Ac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f1);ps("\nAe: ");pi(st.n_ae);ps(" PCS=");pi(st.t_f2);ps("\nAv: ");pi(st.n_av);ps(" PCS=");pi(st.t_f3);ps("\nAs: ");pi(st.n_as);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aer_state(void){ps("[AER] Ac=");pi(st.n_ac);ps(" Ae=");pi(st.n_ae);ps(" Av=");pi(st.n_av);ps(" As=");pi(st.n_as);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aerospace Tech Admin Demo ===\n\n");aer_init();
ps("Aircraft manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aer_aircraft(t,c,220+(i*17),205+(i*14),185+(i*10),167+(i*6),2020+(i%5));}
ps("\nAviation engines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aer_engine(t,c,209+(i*15),195+(i*12),177+(i*8),164+(i*5),2021+(i%4));}
ps("\nAvionics...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aer_avionics(t,c,201+(i*13),187+(i*10),171+(i*7),160+(i*4),2022+(i%3));}
ps("\nAviation services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aer_service(t,c,193+(i*11),181+(i*9),167+(i*6),157+(i*3),2023+(i%2));}
ps("\nAero marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aer_market(t,c,187+(i*9),176+(i*7),163+(i*5),155+(i*3),2024);}
ps("\n");aer_report();aer_state();ps("\n=== Demo Complete ===\n");return 0;}
