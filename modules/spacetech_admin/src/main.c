/* spacetech_admin: Space technology administration (v1.0)
 * Satellite operations, space exploration, space communication, space manufacturing, space tourism
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spc_t;
typedef struct{int n_sa,n_ex,n_co,n_ma,n_to,t_f1,t_f2,t_f3,t_f4,t_f5;} spc_state_t;
static spc_t sas[N],exs[N-2],cos[N-4],mas[N-6],tos[N-6]; static spc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spc_init(void){if(init)return -1;st.n_sa=0;st.n_ex=0;st.n_co=0;st.n_ma=0;st.n_to=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sas[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)cos[i].active=0;for(int i=0;i<N-6;i++)mas[i].active=0;for(int i=0;i<N-6;i++)tos[i].active=0;init=1;ps("[SPC] Spacetech initialized\n");return 0;}
int spc_satellite(int t,int c,int a,int b,int d,int e,int y){return add(sas,&st.n_sa,&st.t_f1,N,t,c,a,b,d,e,y);}
int spc_explore(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spc_comm(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spc_manufact(int t,int c,int a,int b,int d,int e,int y){return add(mas,&st.n_ma,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spc_tourism(int t,int c,int a,int b,int d,int e,int y){return add(tos,&st.n_to,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spc_report(void){ps("[SPC] Sat: ");pi(st.n_sa);ps(" Orbit=");pi(st.t_f1);ps("\nExplore: ");pi(st.n_ex);ps(" Deep=");pi(st.t_f2);ps("\nComm: ");pi(st.n_co);ps(" DSN=");pi(st.t_f3);ps("\nMfg: ");pi(st.n_ma);ps(" QA=");pi(st.t_f4);ps("\nTour: ");pi(st.n_to);ps(" Pax=");pi(st.t_f5);ps("\n");}
void spc_state(void){ps("[SPC] Sa=");pi(st.n_sa);ps(" Ex=");pi(st.n_ex);ps(" Co=");pi(st.n_co);ps(" Ma=");pi(st.n_ma);ps(" To=");pi(st.n_to);ps("\n");}
int main(void){
ps("=== Space Tech Admin Demo ===\n\n");spc_init();
ps("Satellite operations...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spc_satellite(t,c,147+(i*17),132+(i*14),112+(i*10),95+(i*6),2020+(i%5));}
ps("\nSpace exploration...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spc_explore(t,c,136+(i*15),122+(i*12),104+(i*8),91+(i*5),2021+(i%4));}
ps("\nSpace communication...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spc_comm(t,c,128+(i*13),114+(i*10),98+(i*7),87+(i*4),2022+(i%3));}
ps("\nSpace manufacturing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spc_manufact(t,c,120+(i*11),108+(i*9),94+(i*6),84+(i*3),2023+(i%2));}
ps("\nSpace tourism...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spc_tourism(t,c,114+(i*9),103+(i*7),90+(i*5),82+(i*3),2024);}
ps("\n");spc_report();spc_state();ps("\n=== Demo Complete ===\n");return 0;}
