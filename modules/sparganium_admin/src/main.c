/* sparganium_admin: Sparganium management technology administration (v1.0)
 * Sparganium planning, sparganium execution, sparganium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spar_t;
typedef struct{int n_sparp,n_spare,n_spar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spar_state_t;
static spar_t spars[N],sparess[N-2],sparvss[N-4],sparas[N-6],sparmks[N-6]; static spar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(spar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spar_init(void){if(init)return -1;st.n_sparp=0;st.n_spare=0;st.n_spar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)spars[i].active=0;for(int i=0;i<N-2;i++)sparess[i].active=0;for(int i=0;i<N-4;i++)sparvss[i].active=0;for(int i=0;i<N-6;i++)sparas[i].active=0;for(int i=0;i<N-6;i++)sparmks[i].active=0;init=1;ps("[SPAR] Sparganium initialized\n");return 0;}
int spar_planning(int t,int c,int a,int b,int d,int e,int y){return add(spars,&st.n_sparp,&st.t_f1,N,t,c,a,b,d,e,y);}
int spar_execution(int t,int c,int a,int b,int d,int e,int y){return add(sparess,&st.n_spare,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sparvss,&st.n_spar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sparas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spar_market(int t,int c,int a,int b,int d,int e,int y){return add(sparmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spar_report(void){ps("[SPAR] Sparp: ");pi(st.n_sparp);ps(" PCS=");pi(st.t_f1);ps("\nSpae: ");pi(st.n_spare);ps(" PCS=");pi(st.t_f2);ps("\nSparv: ");pi(st.n_spar2);ps(" PCS=");pi(st.t_f3);ps("\nSparc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spar_state(void){ps("[SPAR] Sparp=");pi(st.n_sparp);ps(" Spae=");pi(st.n_spare);ps(" Sparv=");pi(st.n_spar2);ps(" Sparc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sparganium Admin Demo ===\n\n");spar_init();
ps("Sparganium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spar_planning(t,c,1340+(i*17),1329+(i*14),1309+(i*10),1291+(i*6),2020+(i%5));}
ps("\nSparganium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spar_execution(t,c,1329+(i*15),1318+(i*12),1300+(i*8),1287+(i*5),2021+(i%4));}
ps("\nSparganium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spar_evaluation(t,c,1321+(i*13),1310+(i*10),1294+(i*7),1283+(i*4),2022+(i%3));}
ps("\nSparganium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spar_accessory(t,c,1313+(i*11),1304+(i*9),1290+(i*6),1280+(i*3),2023+(i%2));}
ps("\nSparganium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spar_market(t,c,1307+(i*9),1298+(i*7),1285+(i*5),1277+(i*3),2024);}
ps("\n");spar_report();spar_state();ps("\n=== Demo Complete ===\n");return 0;}
