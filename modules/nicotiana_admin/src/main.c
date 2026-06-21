/* nicotiana_admin: Nicotiana management technology administration (v1.0)
 * Nicotiana planning, nicotiana execution, nicotiana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ntb_t;
typedef struct{int n_ntp,n_nte,n_ntv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ntb_state_t;
static ntb_t ntbps[N],ntbss[N-2],ntbvss[N-4],ntbas[N-6],ntbmks[N-6]; static ntb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ntb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ntb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ntb_init(void){if(init)return -1;st.n_ntp=0;st.n_nte=0;st.n_ntv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ntbps[i].active=0;for(int i=0;i<N-2;i++)ntbss[i].active=0;for(int i=0;i<N-4;i++)ntbvss[i].active=0;for(int i=0;i<N-6;i++)ntbas[i].active=0;for(int i=0;i<N-6;i++)ntbmks[i].active=0;init=1;ps("[NTB] Nicotiana initialized\n");return 0;}
int ntb_planning(int t,int c,int a,int b,int d,int e,int y){return add(ntbps,&st.n_ntp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ntb_execution(int t,int c,int a,int b,int d,int e,int y){return add(ntbss,&st.n_nte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ntb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ntbvss,&st.n_ntv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ntb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ntbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ntb_market(int t,int c,int a,int b,int d,int e,int y){return add(ntbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ntb_report(void){ps("[NTB] Ntp: ");pi(st.n_ntp);ps(" PCS=");pi(st.t_f1);ps("\nNte: ");pi(st.n_nte);ps(" PCS=");pi(st.t_f2);ps("\nNtv: ");pi(st.n_ntv);ps(" PCS=");pi(st.t_f3);ps("\nNtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ntb_state(void){ps("[NTB] Ntp=");pi(st.n_ntp);ps(" Nte=");pi(st.n_nte);ps(" Ntv=");pi(st.n_ntv);ps(" Ntc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nicotiana Admin Demo ===\n\n");ntb_init();
ps("Nicotiana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ntb_planning(t,c,844+(i*17),833+(i*14),813+(i*10),795+(i*6),2020+(i%5));}
ps("\nNicotiana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ntb_execution(t,c,833+(i*15),822+(i*12),804+(i*8),791+(i*5),2021+(i%4));}
ps("\nNicotiana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ntb_evaluation(t,c,825+(i*13),814+(i*10),798+(i*7),787+(i*4),2022+(i%3));}
ps("\nNicotiana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntb_accessory(t,c,817+(i*11),808+(i*9),794+(i*6),784+(i*3),2023+(i%2));}
ps("\nNicotiana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntb_market(t,c,811+(i*9),802+(i*7),789+(i*5),781+(i*3),2024);}
ps("\n");ntb_report();ntb_state();ps("\n=== Demo Complete ===\n");return 0;}
