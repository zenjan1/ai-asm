/* maurandya_admin: Maurandya management technology administration (v1.0)
 * Maurandya planning, maurandya execution, maurandya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mau_t;
typedef struct{int n_maup,n_maue,n_mau2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mau_state_t;
static mau_t maups[N],mauess[N-2],mauvss[N-4],mauas[N-6],maumks[N-6]; static mau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mau_init(void){if(init)return -1;st.n_maup=0;st.n_maue=0;st.n_mau2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)maups[i].active=0;for(int i=0;i<N-2;i++)mauess[i].active=0;for(int i=0;i<N-4;i++)mauvss[i].active=0;for(int i=0;i<N-6;i++)mauas[i].active=0;for(int i=0;i<N-6;i++)maumks[i].active=0;init=1;ps("[MAU] Maurandya initialized\n");return 0;}
int mau_planning(int t,int c,int a,int b,int d,int e,int y){return add(maups,&st.n_maup,&st.t_f1,N,t,c,a,b,d,e,y);}
int mau_execution(int t,int c,int a,int b,int d,int e,int y){return add(mauess,&st.n_maue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mauvss,&st.n_mau2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mauas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mau_market(int t,int c,int a,int b,int d,int e,int y){return add(maumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mau_report(void){ps("[MAU] Maup: ");pi(st.n_maup);ps(" PCS=");pi(st.t_f1);ps("\nMaue: ");pi(st.n_maue);ps(" PCS=");pi(st.t_f2);ps("\nMauv: ");pi(st.n_mau2);ps(" PCS=");pi(st.t_f3);ps("\nMauc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mau_state(void){ps("[MAU] Maup=");pi(st.n_maup);ps(" Maue=");pi(st.n_maue);ps(" Mauv=");pi(st.n_mau2);ps(" Mauc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Maurandya Admin Demo ===\n\n");mau_init();
ps("Maurandya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mau_planning(t,c,1313+(i*17),1302+(i*14),1282+(i*10),1264+(i*6),2020+(i%5));}
ps("\nMaurandya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mau_execution(t,c,1302+(i*15),1291+(i*12),1273+(i*8),1260+(i*5),2021+(i%4));}
ps("\nMaurandya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mau_evaluation(t,c,1294+(i*13),1283+(i*10),1267+(i*7),1256+(i*4),2022+(i%3));}
ps("\nMaurandya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mau_accessory(t,c,1286+(i*11),1277+(i*9),1263+(i*6),1253+(i*3),2023+(i%2));}
ps("\nMaurandya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mau_market(t,c,1280+(i*9),1271+(i*7),1258+(i*5),1250+(i*3),2024);}
ps("\n");mau_report();mau_state();ps("\n=== Demo Complete ===\n");return 0;}
