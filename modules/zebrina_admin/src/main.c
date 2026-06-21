/* zebrina_admin: Zebrina management technology administration (v1.0)
 * Zebrina planning, zebrina execution, zebrina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zbr_t;
typedef struct{int n_zbp,n_zbe,n_zbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zbr_state_t;
static zbr_t zbrps[N],zbrss[N-2],zbrvss[N-4],zbras[N-6],zbrmks[N-6]; static zbr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zbr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zbr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZBR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zbr_init(void){if(init)return -1;st.n_zbp=0;st.n_zbe=0;st.n_zbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zbrps[i].active=0;for(int i=0;i<N-2;i++)zbrss[i].active=0;for(int i=0;i<N-4;i++)zbrvss[i].active=0;for(int i=0;i<N-6;i++)zbras[i].active=0;for(int i=0;i<N-6;i++)zbrmks[i].active=0;init=1;ps("[ZBR] Zebrina initialized\n");return 0;}
int zbr_planning(int t,int c,int a,int b,int d,int e,int y){return add(zbrps,&st.n_zbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zbr_execution(int t,int c,int a,int b,int d,int e,int y){return add(zbrss,&st.n_zbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zbr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zbrvss,&st.n_zbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zbr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zbras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zbr_market(int t,int c,int a,int b,int d,int e,int y){return add(zbrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zbr_report(void){ps("[ZBR] Zbp: ");pi(st.n_zbp);ps(" PCS=");pi(st.t_f1);ps("\nZbe: ");pi(st.n_zbe);ps(" PCS=");pi(st.t_f2);ps("\nZbv: ");pi(st.n_zbv);ps(" PCS=");pi(st.t_f3);ps("\nZbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zbr_state(void){ps("[ZBR] Zbp=");pi(st.n_zbp);ps(" Zbe=");pi(st.n_zbe);ps(" Zbv=");pi(st.n_zbv);ps(" Zbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zebrina Admin Demo ===\n\n");zbr_init();
ps("Zebrina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zbr_planning(t,c,882+(i*17),871+(i*14),851+(i*10),833+(i*6),2020+(i%5));}
ps("\nZebrina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zbr_execution(t,c,871+(i*15),860+(i*12),842+(i*8),829+(i*5),2021+(i%4));}
ps("\nZebrina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zbr_evaluation(t,c,863+(i*13),852+(i*10),836+(i*7),825+(i*4),2022+(i%3));}
ps("\nZebrina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zbr_accessory(t,c,855+(i*11),846+(i*9),832+(i*6),822+(i*3),2023+(i%2));}
ps("\nZebrina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zbr_market(t,c,849+(i*9),840+(i*7),827+(i*5),819+(i*3),2024);}
ps("\n");zbr_report();zbr_state();ps("\n=== Demo Complete ===\n");return 0;}
