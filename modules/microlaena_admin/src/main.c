/* microlaena_admin: Microlaena management technology administration (v1.0)
 * Microlaena planning, microlaena execution, microlaena evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mic_t;
typedef struct{int n_micp,n_mice,n_mic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mic_state_t;
static mic_t micps[N],micess[N-2],micvss[N-4],micas[N-6],micmks[N-6]; static mic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mic_init(void){if(init)return -1;st.n_micp=0;st.n_mice=0;st.n_mic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)micps[i].active=0;for(int i=0;i<N-2;i++)micess[i].active=0;for(int i=0;i<N-4;i++)micvss[i].active=0;for(int i=0;i<N-6;i++)micas[i].active=0;for(int i=0;i<N-6;i++)micmks[i].active=0;init=1;ps("[MIC] Microlaena initialized\n");return 0;}
int mic_planning(int t,int c,int a,int b,int d,int e,int y){return add(micps,&st.n_micp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mic_execution(int t,int c,int a,int b,int d,int e,int y){return add(micess,&st.n_mice,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(micvss,&st.n_mic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(micas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mic_market(int t,int c,int a,int b,int d,int e,int y){return add(micmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mic_report(void){ps("[MIC] Micp: ");pi(st.n_micp);ps(" PCS=");pi(st.t_f1);ps("\nMice: ");pi(st.n_mice);ps(" PCS=");pi(st.t_f2);ps("\nMicv: ");pi(st.n_mic2);ps(" PCS=");pi(st.t_f3);ps("\nMicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mic_state(void){ps("[MIC] Micp=");pi(st.n_micp);ps(" Mice=");pi(st.n_mice);ps(" Micv=");pi(st.n_mic2);ps(" Micc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Microlaena Admin Demo ===\n\n");mic_init();
ps("Microlaena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mic_planning(t,c,1315+(i*17),1304+(i*14),1284+(i*10),1266+(i*6),2020+(i%5));}
ps("\nMicrolaena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mic_execution(t,c,1304+(i*15),1293+(i*12),1275+(i*8),1262+(i*5),2021+(i%4));}
ps("\nMicrolaena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mic_evaluation(t,c,1296+(i*13),1285+(i*10),1269+(i*7),1258+(i*4),2022+(i%3));}
ps("\nMicrolaena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mic_accessory(t,c,1288+(i*11),1279+(i*9),1265+(i*6),1255+(i*3),2023+(i%2));}
ps("\nMicrolaena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mic_market(t,c,1282+(i*9),1273+(i*7),1260+(i*5),1252+(i*3),2024);}
ps("\n");mic_report();mic_state();ps("\n=== Demo Complete ===\n");return 0;}
