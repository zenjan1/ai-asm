/* cocculus_admin: Cocculus management technology administration (v1.0)
 * Cocculus planning, cocculus execution, cocculus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} coc_t;
typedef struct{int n_cocp,n_coce,n_coc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} coc_state_t;
static coc_t cocps[N],cocss[N-2],cocvss[N-4],cocas[N-6],cocmks[N-6]; static coc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(coc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;coc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int coc_init(void){if(init)return -1;st.n_cocp=0;st.n_coce=0;st.n_coc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cocps[i].active=0;for(int i=0;i<N-2;i++)cocss[i].active=0;for(int i=0;i<N-4;i++)cocvss[i].active=0;for(int i=0;i<N-6;i++)cocas[i].active=0;for(int i=0;i<N-6;i++)cocmks[i].active=0;init=1;ps("[COC] Cocculus initialized\n");return 0;}
int coc_planning(int t,int c,int a,int b,int d,int e,int y){return add(cocps,&st.n_cocp,&st.t_f1,N,t,c,a,b,d,e,y);}
int coc_execution(int t,int c,int a,int b,int d,int e,int y){return add(cocss,&st.n_coce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int coc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cocvss,&st.n_coc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int coc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cocas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int coc_market(int t,int c,int a,int b,int d,int e,int y){return add(cocmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void coc_report(void){ps("[COC] Coccpp: ");pi(st.n_cocp);ps(" PCS=");pi(st.t_f1);ps("\nCocce: ");pi(st.n_coce);ps(" PCS=");pi(st.t_f2);ps("\nCoccv: ");pi(st.n_coc2);ps(" PCS=");pi(st.t_f3);ps("\nCoccc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void coc_state(void){ps("[COC] Coccpp=");pi(st.n_cocp);ps(" Cocce=");pi(st.n_coce);ps(" Coccv=");pi(st.n_coc2);ps(" Coccc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cocculus Admin Demo ===\n\n");coc_init();
ps("Cocculus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;coc_planning(t,c,987+(i*17),976+(i*14),956+(i*10),938+(i*6),2020+(i%5));}
ps("\nCocculus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;coc_execution(t,c,976+(i*15),965+(i*12),947+(i*8),934+(i*5),2021+(i%4));}
ps("\nCocculus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;coc_evaluation(t,c,968+(i*13),957+(i*10),941+(i*7),930+(i*4),2022+(i%3));}
ps("\nCocculus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;coc_accessory(t,c,960+(i*11),951+(i*9),937+(i*6),927+(i*3),2023+(i%2));}
ps("\nCocculus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;coc_market(t,c,954+(i*9),945+(i*7),932+(i*5),924+(i*3),2024);}
ps("\n");coc_report();coc_state();ps("\n=== Demo Complete ===\n");return 0;}
