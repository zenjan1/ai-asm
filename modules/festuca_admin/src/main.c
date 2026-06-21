/* festuca_admin: Festuca management technology administration (v1.0)
 * Festuca planning, festuca execution, festuca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fes_t;
typedef struct{int n_fesp,n_fese,n_fes2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fes_state_t;
static fes_t fesps[N],fess[N-2],fesvss[N-4],fesas[N-6],fesmks[N-6]; static fes_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fes_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fes_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fes_init(void){if(init)return -1;st.n_fesp=0;st.n_fese=0;st.n_fes2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fesps[i].active=0;for(int i=0;i<N-2;i++)fess[i].active=0;for(int i=0;i<N-4;i++)fesvss[i].active=0;for(int i=0;i<N-6;i++)fesas[i].active=0;for(int i=0;i<N-6;i++)fesmks[i].active=0;init=1;ps("[FES] Festuca initialized\n");return 0;}
int fes_planning(int t,int c,int a,int b,int d,int e,int y){return add(fesps,&st.n_fesp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fes_execution(int t,int c,int a,int b,int d,int e,int y){return add(fess,&st.n_fese,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fes_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fesvss,&st.n_fes2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fes_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fesas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fes_market(int t,int c,int a,int b,int d,int e,int y){return add(fesmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fes_report(void){ps("[FES] Fessp: ");pi(st.n_fesp);ps(" PCS=");pi(st.t_f1);ps("\nFese: ");pi(st.n_fese);ps(" PCS=");pi(st.t_f2);ps("\nFesv: ");pi(st.n_fes2);ps(" PCS=");pi(st.t_f3);ps("\nFesc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fes_state(void){ps("[FES] Fessp=");pi(st.n_fesp);ps(" Fese=");pi(st.n_fese);ps(" Fesv=");pi(st.n_fes2);ps(" Fesc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Festuca Admin Demo ===\n\n");fes_init();
ps("Festuca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fes_planning(t,c,1005+(i*17),994+(i*14),974+(i*10),956+(i*6),2020+(i%5));}
ps("\nFestuca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fes_execution(t,c,994+(i*15),983+(i*12),965+(i*8),952+(i*5),2021+(i%4));}
ps("\nFestuca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fes_evaluation(t,c,986+(i*13),975+(i*10),959+(i*7),948+(i*4),2022+(i%3));}
ps("\nFestuca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fes_accessory(t,c,978+(i*11),969+(i*9),955+(i*6),945+(i*3),2023+(i%2));}
ps("\nFestuca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fes_market(t,c,972+(i*9),963+(i*7),950+(i*5),942+(i*3),2024);}
ps("\n");fes_report();fes_state();ps("\n=== Demo Complete ===\n");return 0;}
