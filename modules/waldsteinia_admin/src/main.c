/* waldsteinia_admin: Waldsteinia management technology administration (v1.0)
 * Waldsteinia planning, waldsteinia execution, waldsteinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wal_t;
typedef struct{int n_walp,n_wale,n_wal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wal_state_t;
static wal_t walps[N],walss[N-2],walvss[N-4],walas[N-6],walmks[N-6]; static wal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wal_init(void){if(init)return -1;st.n_walp=0;st.n_wale=0;st.n_wal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)walps[i].active=0;for(int i=0;i<N-2;i++)walss[i].active=0;for(int i=0;i<N-4;i++)walvss[i].active=0;for(int i=0;i<N-6;i++)walas[i].active=0;for(int i=0;i<N-6;i++)walmks[i].active=0;init=1;ps("[WAL] Waldsteinia initialized\n");return 0;}
int wal_planning(int t,int c,int a,int b,int d,int e,int y){return add(walps,&st.n_walp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wal_execution(int t,int c,int a,int b,int d,int e,int y){return add(walss,&st.n_wale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(walvss,&st.n_wal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(walas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wal_market(int t,int c,int a,int b,int d,int e,int y){return add(walmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wal_report(void){ps("[WAL] Walpp: ");pi(st.n_walp);ps(" PCS=");pi(st.t_f1);ps("\nWale: ");pi(st.n_wale);ps(" PCS=");pi(st.t_f2);ps("\nWalv: ");pi(st.n_wal2);ps(" PCS=");pi(st.t_f3);ps("\nWalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wal_state(void){ps("[WAL] Walpp=");pi(st.n_walp);ps(" Wale=");pi(st.n_wale);ps(" Walv=");pi(st.n_wal2);ps(" Walc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Waldsteinia Admin Demo ===\n\n");wal_init();
ps("Waldsteinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wal_planning(t,c,1202+(i*17),1191+(i*14),1171+(i*10),1153+(i*6),2020+(i%5));}
ps("\nWaldsteinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wal_execution(t,c,1191+(i*15),1180+(i*12),1162+(i*8),1149+(i*5),2021+(i%4));}
ps("\nWaldsteinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wal_evaluation(t,c,1183+(i*13),1172+(i*10),1156+(i*7),1145+(i*4),2022+(i%3));}
ps("\nWaldsteinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wal_accessory(t,c,1175+(i*11),1166+(i*9),1152+(i*6),1142+(i*3),2023+(i%2));}
ps("\nWaldsteinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wal_market(t,c,1169+(i*9),1160+(i*7),1147+(i*5),1139+(i*3),2024);}
ps("\n");wal_report();wal_state();ps("\n=== Demo Complete ===\n");return 0;}
