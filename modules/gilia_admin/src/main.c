/* gilia_admin: Gilia management technology administration (v1.0)
 * Gilia planning, gilia execution, gilia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gil_t;
typedef struct{int n_gilp,n_gile,n_gil2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gil_state_t;
static gil_t gilps[N],gilss[N-2],gilvss[N-4],gils[N-6],gilmks[N-6]; static gil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gil_init(void){if(init)return -1;st.n_gilp=0;st.n_gile=0;st.n_gil2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gilps[i].active=0;for(int i=0;i<N-2;i++)gilss[i].active=0;for(int i=0;i<N-4;i++)gilvss[i].active=0;for(int i=0;i<N-6;i++)gils[i].active=0;for(int i=0;i<N-6;i++)gilmks[i].active=0;init=1;ps("[GIL] Gilia initialized\n");return 0;}
int gil_planning(int t,int c,int a,int b,int d,int e,int y){return add(gilps,&st.n_gilp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gil_execution(int t,int c,int a,int b,int d,int e,int y){return add(gilss,&st.n_gile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gilvss,&st.n_gil2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gils,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gil_market(int t,int c,int a,int b,int d,int e,int y){return add(gilmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gil_report(void){ps("[GIL] Gilp: ");pi(st.n_gilp);ps(" PCS=");pi(st.t_f1);ps("\nGile: ");pi(st.n_gile);ps(" PCS=");pi(st.t_f2);ps("\nGilv: ");pi(st.n_gil2);ps(" PCS=");pi(st.t_f3);ps("\nGilc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gil_state(void){ps("[GIL] Gilp=");pi(st.n_gilp);ps(" Gile=");pi(st.n_gile);ps(" Gilv=");pi(st.n_gil2);ps(" Gilc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gilia Admin Demo ===\n\n");gil_init();
ps("Gilia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gil_planning(t,c,1010+(i*17),999+(i*14),979+(i*10),961+(i*6),2020+(i%5));}
ps("\nGilia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gil_execution(t,c,999+(i*15),988+(i*12),970+(i*8),957+(i*5),2021+(i%4));}
ps("\nGilia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gil_evaluation(t,c,991+(i*13),980+(i*10),964+(i*7),953+(i*4),2022+(i%3));}
ps("\nGilia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gil_accessory(t,c,983+(i*11),974+(i*9),960+(i*6),950+(i*3),2023+(i%2));}
ps("\nGilia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gil_market(t,c,977+(i*9),968+(i*7),955+(i*5),947+(i*3),2024);}
ps("\n");gil_report();gil_state();ps("\n=== Demo Complete ===\n");return 0;}
