/* uvularia_admin: Uvularia management technology administration (v1.0)
 * Uvularia planning, uvularia execution, uvularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} uvu_t;
typedef struct{int n_uvup,n_uvue,n_uvu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} uvu_state_t;
static uvu_t uvups[N],uvuss[N-2],uvuvss[N-4],uvuas[N-6],uvumks[N-6]; static uvu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(uvu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;uvu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UVU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int uvu_init(void){if(init)return -1;st.n_uvup=0;st.n_uvue=0;st.n_uvu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uvups[i].active=0;for(int i=0;i<N-2;i++)uvuss[i].active=0;for(int i=0;i<N-4;i++)uvuvss[i].active=0;for(int i=0;i<N-6;i++)uvuas[i].active=0;for(int i=0;i<N-6;i++)uvumks[i].active=0;init=1;ps("[UVU] Uvularia initialized\n");return 0;}
int uvu_planning(int t,int c,int a,int b,int d,int e,int y){return add(uvups,&st.n_uvup,&st.t_f1,N,t,c,a,b,d,e,y);}
int uvu_execution(int t,int c,int a,int b,int d,int e,int y){return add(uvuss,&st.n_uvue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int uvu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(uvuvss,&st.n_uvu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int uvu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(uvuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int uvu_market(int t,int c,int a,int b,int d,int e,int y){return add(uvumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void uvu_report(void){ps("[UVU] Uvup: ");pi(st.n_uvup);ps(" PCS=");pi(st.t_f1);ps("\nUvue: ");pi(st.n_uvue);ps(" PCS=");pi(st.t_f2);ps("\nUvuv: ");pi(st.n_uvu2);ps(" PCS=");pi(st.t_f3);ps("\nUvuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void uvu_state(void){ps("[UVU] Uvup=");pi(st.n_uvup);ps(" Uvue=");pi(st.n_uvue);ps(" Uvuv=");pi(st.n_uvu2);ps(" Uvuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Uvularia Admin Demo ===\n\n");uvu_init();
ps("Uvularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;uvu_planning(t,c,956+(i*17),945+(i*14),925+(i*10),907+(i*6),2020+(i%5));}
ps("\nUvularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;uvu_execution(t,c,945+(i*15),934+(i*12),916+(i*8),903+(i*5),2021+(i%4));}
ps("\nUvularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;uvu_evaluation(t,c,937+(i*13),926+(i*10),910+(i*7),899+(i*4),2022+(i%3));}
ps("\nUvularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uvu_accessory(t,c,929+(i*11),920+(i*9),906+(i*6),896+(i*3),2023+(i%2));}
ps("\nUvularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uvu_market(t,c,923+(i*9),914+(i*7),901+(i*5),893+(i*3),2024);}
ps("\n");uvu_report();uvu_state();ps("\n=== Demo Complete ===\n");return 0;}
