/* zygophyllas_admin: Zygophyllas management technology administration (v1.0)
 * Zygophyllas planning, zygophyllas execution, zygophyllas evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zygo_t;
typedef struct{int n_zygop,n_zygo,n_zygo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zygo_state_t;
static zygo_t zygps[N],zygoe[N-2],zygov[N-4],zygoac[N-6],zygom[N-6]; static zygo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zygo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zygo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZYGO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zygo_init(void){if(init)return -1;st.n_zygop=0;st.n_zygo=0;st.n_zygo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zygps[i].active=0;for(int i=0;i<N-2;i++)zygoe[i].active=0;for(int i=0;i<N-4;i++)zygov[i].active=0;for(int i=0;i<N-6;i++)zygoac[i].active=0;for(int i=0;i<N-6;i++)zygom[i].active=0;init=1;ps("[ZYGO] Zygophyllas initialized\n");return 0;}
int zygo_planning(int t,int c,int a,int b,int d,int e,int y){return add(zygps,&st.n_zygop,&st.t_f1,N,t,c,a,b,d,e,y);}
int zygo_execution(int t,int c,int a,int b,int d,int e,int y){return add(zygoe,&st.n_zygo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zygo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zygov,&st.n_zygo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zygo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zygoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zygo_market(int t,int c,int a,int b,int d,int e,int y){return add(zygom,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zygo_report(void){ps("[ZYGO] Zygop: ");pi(st.n_zygop);ps(" PCS=");pi(st.t_f1);ps("\nZygoe: ");pi(st.n_zygo);ps(" PCS=");pi(st.t_f2);ps("\nZygov: ");pi(st.n_zygo2);ps(" PCS=");pi(st.t_f3);ps("\nZygoac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zygo_state(void){ps("[ZYGO] Zygop=");pi(st.n_zygop);ps(" Zygoe=");pi(st.n_zygo);ps(" Zygov=");pi(st.n_zygo2);ps(" Zygoac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zygophyllas Admin Demo ===\n\n");zygo_init();
ps("Zygophyllas planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zygo_planning(t,c,1384+(i*17),1373+(i*14),1353+(i*10),1335+(i*6),2020+(i%5));}
ps("\nZygophyllas execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zygo_execution(t,c,1373+(i*15),1362+(i*12),1344+(i*8),1331+(i*5),2021+(i%4));}
ps("\nZygophyllas evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zygo_evaluation(t,c,1365+(i*13),1354+(i*10),1338+(i*7),1327+(i*4),2022+(i%3));}
ps("\nZygophyllas accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zygo_accessory(t,c,1357+(i*11),1348+(i*9),1334+(i*6),1324+(i*3),2023+(i%2));}
ps("\nZygophyllas marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zygo_market(t,c,1351+(i*9),1342+(i*7),1329+(i*5),1321+(i*3),2024);}
ps("\n");zygo_report();zygo_state();ps("\n=== Demo Complete ===\n");return 0;}
