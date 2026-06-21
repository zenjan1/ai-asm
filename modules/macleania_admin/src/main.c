/* macleania_admin: Macleania management technology administration (v1.0)
 * Macleania planning, macleania execution, macleania evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mcl_t;
typedef struct{int n_mlp,n_mle,n_mlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mcl_state_t;
static mcl_t mclps[N],mclss[N-2],mclvss[N-4],mclas[N-6],mclmks[N-6]; static mcl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mcl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mcl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mcl_init(void){if(init)return -1;st.n_mlp=0;st.n_mle=0;st.n_mlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mclps[i].active=0;for(int i=0;i<N-2;i++)mclss[i].active=0;for(int i=0;i<N-4;i++)mclvss[i].active=0;for(int i=0;i<N-6;i++)mclas[i].active=0;for(int i=0;i<N-6;i++)mclmks[i].active=0;init=1;ps("[MCL] Macleania initialized\n");return 0;}
int mcl_planning(int t,int c,int a,int b,int d,int e,int y){return add(mclps,&st.n_mlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mcl_execution(int t,int c,int a,int b,int d,int e,int y){return add(mclss,&st.n_mle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mcl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mclvss,&st.n_mlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mcl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mclas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mcl_market(int t,int c,int a,int b,int d,int e,int y){return add(mclmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mcl_report(void){ps("[MCL] Mlp: ");pi(st.n_mlp);ps(" PCS=");pi(st.t_f1);ps("\nMle: ");pi(st.n_mle);ps(" PCS=");pi(st.t_f2);ps("\nMlv: ");pi(st.n_mlv);ps(" PCS=");pi(st.t_f3);ps("\nMlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mcl_state(void){ps("[MCL] Mlp=");pi(st.n_mlp);ps(" Mle=");pi(st.n_mle);ps(" Mlv=");pi(st.n_mlv);ps(" Mlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Macleania Admin Demo ===\n\n");mcl_init();
ps("Macleania planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mcl_planning(t,c,837+(i*17),826+(i*14),806+(i*10),788+(i*6),2020+(i%5));}
ps("\nMacleania execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mcl_execution(t,c,826+(i*15),815+(i*12),797+(i*8),784+(i*5),2021+(i%4));}
ps("\nMacleania evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mcl_evaluation(t,c,818+(i*13),807+(i*10),791+(i*7),780+(i*4),2022+(i%3));}
ps("\nMacleania accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcl_accessory(t,c,810+(i*11),801+(i*9),787+(i*6),777+(i*3),2023+(i%2));}
ps("\nMacleania marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcl_market(t,c,804+(i*9),795+(i*7),782+(i*5),774+(i*3),2024);}
ps("\n");mcl_report();mcl_state();ps("\n=== Demo Complete ===\n");return 0;}
