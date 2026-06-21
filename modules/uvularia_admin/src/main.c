/* uvularia_admin: Uvularia management technology administration (v1.0)
 * Uvularia planning, uvularia execution, uvularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} uvl_t;
typedef struct{int n_uvp,n_uve,n_uvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} uvl_state_t;
static uvl_t uvlps[N],uvlss[N-2],uvlvss[N-4],uvlas[N-6],uvlmks[N-6]; static uvl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(uvl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;uvl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UVL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int uvl_init(void){if(init)return -1;st.n_uvp=0;st.n_uve=0;st.n_uvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uvlps[i].active=0;for(int i=0;i<N-2;i++)uvlss[i].active=0;for(int i=0;i<N-4;i++)uvlvss[i].active=0;for(int i=0;i<N-6;i++)uvlas[i].active=0;for(int i=0;i<N-6;i++)uvlmks[i].active=0;init=1;ps("[UVL] Uvularia initialized\n");return 0;}
int uvl_planning(int t,int c,int a,int b,int d,int e,int y){return add(uvlps,&st.n_uvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int uvl_execution(int t,int c,int a,int b,int d,int e,int y){return add(uvlss,&st.n_uve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int uvl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(uvlvss,&st.n_uvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int uvl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(uvlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int uvl_market(int t,int c,int a,int b,int d,int e,int y){return add(uvlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void uvl_report(void){ps("[UVL] Uvp: ");pi(st.n_uvp);ps(" PCS=");pi(st.t_f1);ps("\nUve: ");pi(st.n_uve);ps(" PCS=");pi(st.t_f2);ps("\nUvv: ");pi(st.n_uvv);ps(" PCS=");pi(st.t_f3);ps("\nUvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void uvl_state(void){ps("[UVL] Uvp=");pi(st.n_uvp);ps(" Uve=");pi(st.n_uve);ps(" Uvv=");pi(st.n_uvv);ps(" Uvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Uvularia Admin Demo ===\n\n");uvl_init();
ps("Uvularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;uvl_planning(t,c,873+(i*17),862+(i*14),842+(i*10),824+(i*6),2020+(i%5));}
ps("\nUvularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;uvl_execution(t,c,862+(i*15),851+(i*12),833+(i*8),820+(i*5),2021+(i%4));}
ps("\nUvularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;uvl_evaluation(t,c,854+(i*13),843+(i*10),827+(i*7),816+(i*4),2022+(i%3));}
ps("\nUvularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uvl_accessory(t,c,846+(i*11),837+(i*9),823+(i*6),813+(i*3),2023+(i%2));}
ps("\nUvularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uvl_market(t,c,840+(i*9),831+(i*7),818+(i*5),810+(i*3),2024);}
ps("\n");uvl_report();uvl_state();ps("\n=== Demo Complete ===\n");return 0;}
