/* jeffersonia_admin: Jeffersonia management technology administration (v1.0)
 * Jeffersonia planning, jeffersonia execution, jeffersonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jef_t;
typedef struct{int n_jefp,n_jefe,n_jef2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jef_state_t;
static jef_t jefps[N],jefss[N-2],jefvss[N-4],jefas[N-6],jefmks[N-6]; static jef_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jef_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jef_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JEF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jef_init(void){if(init)return -1;st.n_jefp=0;st.n_jefe=0;st.n_jef2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jefps[i].active=0;for(int i=0;i<N-2;i++)jefss[i].active=0;for(int i=0;i<N-4;i++)jefvss[i].active=0;for(int i=0;i<N-6;i++)jefas[i].active=0;for(int i=0;i<N-6;i++)jefmks[i].active=0;init=1;ps("[JEF] Jeffersonia initialized\n");return 0;}
int jef_planning(int t,int c,int a,int b,int d,int e,int y){return add(jefps,&st.n_jefp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jef_execution(int t,int c,int a,int b,int d,int e,int y){return add(jefss,&st.n_jefe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jef_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jefvss,&st.n_jef2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jef_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jefas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jef_market(int t,int c,int a,int b,int d,int e,int y){return add(jefmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jef_report(void){ps("[JEF] Jefp: ");pi(st.n_jefp);ps(" PCS=");pi(st.t_f1);ps("\nJefe: ");pi(st.n_jefe);ps(" PCS=");pi(st.t_f2);ps("\nJefv: ");pi(st.n_jef2);ps(" PCS=");pi(st.t_f3);ps("\nJefc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jef_state(void){ps("[JEF] Jefp=");pi(st.n_jefp);ps(" Jefe=");pi(st.n_jefe);ps(" Jefv=");pi(st.n_jef2);ps(" Jefc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jeffersonia Admin Demo ===\n\n");jef_init();
ps("Jeffersonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jef_planning(t,c,1029+(i*17),1018+(i*14),998+(i*10),980+(i*6),2020+(i%5));}
ps("\nJeffersonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jef_execution(t,c,1018+(i*15),1007+(i*12),989+(i*8),976+(i*5),2021+(i%4));}
ps("\nJeffersonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jef_evaluation(t,c,1010+(i*13),999+(i*10),983+(i*7),972+(i*4),2022+(i%3));}
ps("\nJeffersonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jef_accessory(t,c,1002+(i*11),993+(i*9),979+(i*6),969+(i*3),2023+(i%2));}
ps("\nJeffersonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jef_market(t,c,996+(i*9),987+(i*7),974+(i*5),966+(i*3),2024);}
ps("\n");jef_report();jef_state();ps("\n=== Demo Complete ===\n");return 0;}
