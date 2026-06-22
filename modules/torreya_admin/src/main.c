/* torreya_admin: Torreya management technology administration (v1.0)
 * Torreya planning, torreya execution, torreya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} torr_t;
typedef struct{int n_torrp,n_torre,n_torr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} torr_state_t;
static torr_t torrs[N],torress[N-2],torrvss[N-4],torras[N-6],torrmks[N-6]; static torr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(torr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;torr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TORR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int torr_init(void){if(init)return -1;st.n_torrp=0;st.n_torre=0;st.n_torr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)torrs[i].active=0;for(int i=0;i<N-2;i++)torress[i].active=0;for(int i=0;i<N-4;i++)torrvss[i].active=0;for(int i=0;i<N-6;i++)torras[i].active=0;for(int i=0;i<N-6;i++)torrmks[i].active=0;init=1;ps("[TORR] Torreya initialized\n");return 0;}
int torr_planning(int t,int c,int a,int b,int d,int e,int y){return add(torrs,&st.n_torrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int torr_execution(int t,int c,int a,int b,int d,int e,int y){return add(torress,&st.n_torre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int torr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(torrvss,&st.n_torr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int torr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(torras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int torr_market(int t,int c,int a,int b,int d,int e,int y){return add(torrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void torr_report(void){ps("[TORR] Torrp: ");pi(st.n_torrp);ps(" PCS=");pi(st.t_f1);ps("\nTorre: ");pi(st.n_torre);ps(" PCS=");pi(st.t_f2);ps("\nTorr2: ");pi(st.n_torr2);ps(" PCS=");pi(st.t_f3);ps("\nTorr: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void torr_state(void){ps("[TORR] Torrp=");pi(st.n_torrp);ps(" Torre=");pi(st.n_torre);ps(" Torr2=");pi(st.n_torr2);ps(" Torrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Torreya Admin Demo ===\n\n");torr_init();
ps("Torreya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;torr_planning(t,c,1349+(i*17),1338+(i*14),1318+(i*10),1300+(i*6),2020+(i%5));}
ps("\nTorreya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;torr_execution(t,c,1338+(i*15),1327+(i*12),1309+(i*8),1296+(i*5),2021+(i%4));}
ps("\nTorreya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;torr_evaluation(t,c,1330+(i*13),1319+(i*10),1303+(i*7),1292+(i*4),2022+(i%3));}
ps("\nTorreya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;torr_accessory(t,c,1322+(i*11),1313+(i*9),1299+(i*6),1289+(i*3),2023+(i%2));}
ps("\nTorreya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;torr_market(t,c,1316+(i*9),1307+(i*7),1294+(i*5),1286+(i*3),2024);}
ps("\n");torr_report();torr_state();ps("\n=== Demo Complete ===\n");return 0;}
