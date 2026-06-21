/* guzmania_admin: Guzmania management technology administration (v1.0)
 * Guzmania planning, guzmania execution, guzmania evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} guz_t;
typedef struct{int n_guzp,n_guze,n_guz2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} guz_state_t;
static guz_t guzps[N],guzss[N-2],guzvss[N-4],guzas[N-6],guzmks[N-6]; static guz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(guz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;guz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int guz_init(void){if(init)return -1;st.n_guzp=0;st.n_guze=0;st.n_guz2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)guzps[i].active=0;for(int i=0;i<N-2;i++)guzss[i].active=0;for(int i=0;i<N-4;i++)guzvss[i].active=0;for(int i=0;i<N-6;i++)guzas[i].active=0;for(int i=0;i<N-6;i++)guzmks[i].active=0;init=1;ps("[GUZ] Guzmania initialized\n");return 0;}
int guz_planning(int t,int c,int a,int b,int d,int e,int y){return add(guzps,&st.n_guzp,&st.t_f1,N,t,c,a,b,d,e,y);}
int guz_execution(int t,int c,int a,int b,int d,int e,int y){return add(guzss,&st.n_guze,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int guz_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(guzvss,&st.n_guz2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int guz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(guzas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int guz_market(int t,int c,int a,int b,int d,int e,int y){return add(guzmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void guz_report(void){ps("[GUZ] Guzp: ");pi(st.n_guzp);ps(" PCS=");pi(st.t_f1);ps("\nGuze: ");pi(st.n_guze);ps(" PCS=");pi(st.t_f2);ps("\nGuzv: ");pi(st.n_guz2);ps(" PCS=");pi(st.t_f3);ps("\nGuzc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void guz_state(void){ps("[GUZ] Guzp=");pi(st.n_guzp);ps(" Guze=");pi(st.n_guze);ps(" Guzv=");pi(st.n_guz2);ps(" Guzc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Guzmania Admin Demo ===\n\n");guz_init();
ps("Guzmania planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;guz_planning(t,c,1016+(i*17),1005+(i*14),985+(i*10),967+(i*6),2020+(i%5));}
ps("\nGuzmania execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;guz_execution(t,c,1005+(i*15),994+(i*12),976+(i*8),963+(i*5),2021+(i%4));}
ps("\nGuzmania evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;guz_evaluation(t,c,997+(i*13),986+(i*10),970+(i*7),959+(i*4),2022+(i%3));}
ps("\nGuzmania accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;guz_accessory(t,c,989+(i*11),980+(i*9),966+(i*6),956+(i*3),2023+(i%2));}
ps("\nGuzmania marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;guz_market(t,c,983+(i*9),974+(i*7),961+(i*5),953+(i*3),2024);}
ps("\n");guz_report();guz_state();ps("\n=== Demo Complete ===\n");return 0;}
