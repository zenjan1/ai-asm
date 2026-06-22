/* diplotaxis_admin: Diplotaxis management technology administration (v1.0)
 * Diplotaxis planning, diplotaxis execution, diplotaxis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dpl_t;
typedef struct{int n_dplp,n_dple,n_dpl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dpl_state_t;
static dpl_t dplps[N],dplss[N-2],dplvss[N-4],dplas[N-6],dplmks[N-6]; static dpl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dpl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dpl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DPL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dpl_init(void){if(init)return -1;st.n_dplp=0;st.n_dple=0;st.n_dpl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dplps[i].active=0;for(int i=0;i<N-2;i++)dplss[i].active=0;for(int i=0;i<N-4;i++)dplvss[i].active=0;for(int i=0;i<N-6;i++)dplas[i].active=0;for(int i=0;i<N-6;i++)dplmks[i].active=0;init=1;ps("[DPL] Diplotaxis initialized\n");return 0;}
int dpl_planning(int t,int c,int a,int b,int d,int e,int y){return add(dplps,&st.n_dplp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dpl_execution(int t,int c,int a,int b,int d,int e,int y){return add(dplss,&st.n_dple,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dpl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dplvss,&st.n_dpl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dpl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dplas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dpl_market(int t,int c,int a,int b,int d,int e,int y){return add(dplmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dpl_report(void){ps("[DPL] Dipp: ");pi(st.n_dplp);ps(" PCS=");pi(st.t_f1);ps("\nDipe: ");pi(st.n_dple);ps(" PCS=");pi(st.t_f2);ps("\nDipv: ");pi(st.n_dpl2);ps(" PCS=");pi(st.t_f3);ps("\nDipc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dpl_state(void){ps("[DPL] Dipp=");pi(st.n_dplp);ps(" Dipe=");pi(st.n_dple);ps(" Dipv=");pi(st.n_dpl2);ps(" Dipc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Diplotaxis Admin Demo ===\n\n");dpl_init();
ps("Diplotaxis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dpl_planning(t,c,1267+(i*17),1256+(i*14),1236+(i*10),1218+(i*6),2020+(i%5));}
ps("\nDiplotaxis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dpl_execution(t,c,1256+(i*15),1245+(i*12),1227+(i*8),1214+(i*5),2021+(i%4));}
ps("\nDiplotaxis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dpl_evaluation(t,c,1248+(i*13),1237+(i*10),1221+(i*7),1210+(i*4),2022+(i%3));}
ps("\nDiplotaxis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dpl_accessory(t,c,1240+(i*11),1231+(i*9),1217+(i*6),1207+(i*3),2023+(i%2));}
ps("\nDiplotaxis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dpl_market(t,c,1234+(i*9),1225+(i*7),1212+(i*5),1204+(i*3),2024);}
ps("\n");dpl_report();dpl_state();ps("\n=== Demo Complete ===\n");return 0;}
