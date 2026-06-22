/* melananthera_admin: Melananthera management technology administration (v1.0)
 * Melananthera planning, melananthera execution, melananthera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mel_t;
typedef struct{int n_melp,n_mele,n_mel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mel_state_t;
static mel_t melms[N],meless[N-2],melvss[N-4],melas[N-6],melmks[N-6]; static mel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mel_init(void){if(init)return -1;st.n_melp=0;st.n_mele=0;st.n_mel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)melms[i].active=0;for(int i=0;i<N-2;i++)meless[i].active=0;for(int i=0;i<N-4;i++)melvss[i].active=0;for(int i=0;i<N-6;i++)melas[i].active=0;for(int i=0;i<N-6;i++)melmks[i].active=0;init=1;ps("[MEL] Melananthera initialized\n");return 0;}
int mel_planning(int t,int c,int a,int b,int d,int e,int y){return add(melms,&st.n_melp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mel_execution(int t,int c,int a,int b,int d,int e,int y){return add(meless,&st.n_mele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(melvss,&st.n_mel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(melas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mel_market(int t,int c,int a,int b,int d,int e,int y){return add(melmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mel_report(void){ps("[MEL] Melp: ");pi(st.n_melp);ps(" PCS=");pi(st.t_f1);ps("\nMele: ");pi(st.n_mele);ps(" PCS=");pi(st.t_f2);ps("\nMelv: ");pi(st.n_mel2);ps(" PCS=");pi(st.t_f3);ps("\nMelc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mel_state(void){ps("[MEL] Melp=");pi(st.n_melp);ps(" Mele=");pi(st.n_mele);ps(" Melv=");pi(st.n_mel2);ps(" Melc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Melananthera Admin Demo ===\n\n");mel_init();
ps("Melananthera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mel_planning(t,c,1314+(i*17),1303+(i*14),1283+(i*10),1265+(i*6),2020+(i%5));}
ps("\nMelananthera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mel_execution(t,c,1303+(i*15),1292+(i*12),1274+(i*8),1261+(i*5),2021+(i%4));}
ps("\nMelananthera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mel_evaluation(t,c,1295+(i*13),1284+(i*10),1268+(i*7),1257+(i*4),2022+(i%3));}
ps("\nMelananthera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mel_accessory(t,c,1287+(i*11),1278+(i*9),1264+(i*6),1254+(i*3),2023+(i%2));}
ps("\nMelananthera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mel_market(t,c,1281+(i*9),1272+(i*7),1259+(i*5),1251+(i*3),2024);}
ps("\n");mel_report();mel_state();ps("\n=== Demo Complete ===\n");return 0;}
