/* malva_admin: Malva management technology administration (v1.0)
 * Malva planning, malva execution, malva evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ml_t;
typedef struct{int n_mlp,n_mle,n_mlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ml_state_t;
static ml_t mlps[N],mlss[N-2],mlvss[N-4],mlas[N-6],mlmks[N-6]; static ml_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ml_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ml_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mlt_init(void){if(init)return -1;st.n_mlp=0;st.n_mle=0;st.n_mlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mlps[i].active=0;for(int i=0;i<N-2;i++)mlss[i].active=0;for(int i=0;i<N-4;i++)mlvss[i].active=0;for(int i=0;i<N-6;i++)mlas[i].active=0;for(int i=0;i<N-6;i++)mlmks[i].active=0;init=1;ps("[MLT] Malva initialized\n");return 0;}
int mlt_planning(int t,int c,int a,int b,int d,int e,int y){return add(mlps,&st.n_mlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mlt_execution(int t,int c,int a,int b,int d,int e,int y){return add(mlss,&st.n_mle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mlt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mlvss,&st.n_mlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mlt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mlt_market(int t,int c,int a,int b,int d,int e,int y){return add(mlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mlt_report(void){ps("[MLT] Mlp: ");pi(st.n_mlp);ps(" PCS=");pi(st.t_f1);ps("\nMle: ");pi(st.n_mle);ps(" PCS=");pi(st.t_f2);ps("\nMlv: ");pi(st.n_mlv);ps(" PCS=");pi(st.t_f3);ps("\nMlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mlt_state(void){ps("[MLT] Mlp=");pi(st.n_mlp);ps(" Mle=");pi(st.n_mle);ps(" Mlv=");pi(st.n_mlv);ps(" Mlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Malva Admin Demo ===\n\n");mlt_init();
ps("Malva planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mlt_planning(t,c,731+(i*17),720+(i*14),700+(i*10),682+(i*6),2020+(i%5));}
ps("\nMalva execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mlt_execution(t,c,720+(i*15),709+(i*12),691+(i*8),678+(i*5),2021+(i%4));}
ps("\nMalva evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mlt_evaluation(t,c,712+(i*13),701+(i*10),685+(i*7),674+(i*4),2022+(i%3));}
ps("\nMalva accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlt_accessory(t,c,704+(i*11),695+(i*9),681+(i*6),671+(i*3),2023+(i%2));}
ps("\nMalva marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlt_market(t,c,698+(i*9),689+(i*7),676+(i*5),668+(i*3),2024);}
ps("\n");mlt_report();mlt_state();ps("\n=== Demo Complete ===\n");return 0;}
