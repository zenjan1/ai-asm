/* larix_admin: Larix management technology administration (v1.0)
 * Larix planning, larix execution, larix evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lar_t;
typedef struct{int n_larp,n_lare,n_lar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lar_state_t;
static lar_t larps[N],larss[N-2],larvss[N-4],laras[N-6],larmks[N-6]; static lar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lar_init(void){if(init)return -1;st.n_larp=0;st.n_lare=0;st.n_lar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)larps[i].active=0;for(int i=0;i<N-2;i++)larss[i].active=0;for(int i=0;i<N-4;i++)larvss[i].active=0;for(int i=0;i<N-6;i++)laras[i].active=0;for(int i=0;i<N-6;i++)larmks[i].active=0;init=1;ps("[LAR] Larix initialized\n");return 0;}
int lar_planning(int t,int c,int a,int b,int d,int e,int y){return add(larps,&st.n_larp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lar_execution(int t,int c,int a,int b,int d,int e,int y){return add(larss,&st.n_lare,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(larvss,&st.n_lar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(laras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lar_market(int t,int c,int a,int b,int d,int e,int y){return add(larmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lar_report(void){ps("[LAR] Larp: ");pi(st.n_larp);ps(" PCS=");pi(st.t_f1);ps("\nLare: ");pi(st.n_lare);ps(" PCS=");pi(st.t_f2);ps("\nLarv: ");pi(st.n_lar2);ps(" PCS=");pi(st.t_f3);ps("\nLarc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lar_state(void){ps("[LAR] Larp=");pi(st.n_larp);ps(" Lare=");pi(st.n_lare);ps(" Larv=");pi(st.n_lar2);ps(" Larc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Larix Admin Demo ===\n\n");lar_init();
ps("Larix planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lar_planning(t,c,1044+(i*17),1033+(i*14),1013+(i*10),995+(i*6),2020+(i%5));}
ps("\nLarix execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lar_execution(t,c,1033+(i*15),1022+(i*12),1004+(i*8),991+(i*5),2021+(i%4));}
ps("\nLarix evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lar_evaluation(t,c,1025+(i*13),1014+(i*10),998+(i*7),987+(i*4),2022+(i%3));}
ps("\nLarix accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lar_accessory(t,c,1017+(i*11),1008+(i*9),994+(i*6),984+(i*3),2023+(i%2));}
ps("\nLarix marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lar_market(t,c,1011+(i*9),1002+(i*7),989+(i*5),981+(i*3),2024);}
ps("\n");lar_report();lar_state();ps("\n=== Demo Complete ===\n");return 0;}
