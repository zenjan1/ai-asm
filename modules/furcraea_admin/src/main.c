/* furcraea_admin: Furcraea management technology administration (v1.0)
 * Furcraea planning, furcraea execution, furcraea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fur_t;
typedef struct{int n_furp,n_fure,n_fur2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fur_state_t;
static fur_t furps[N],furss[N-2],furvss[N-4],furas[N-6],furmks[N-6]; static fur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fur_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fur_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fur_init(void){if(init)return -1;st.n_furp=0;st.n_fure=0;st.n_fur2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)furps[i].active=0;for(int i=0;i<N-2;i++)furss[i].active=0;for(int i=0;i<N-4;i++)furvss[i].active=0;for(int i=0;i<N-6;i++)furas[i].active=0;for(int i=0;i<N-6;i++)furmks[i].active=0;init=1;ps("[FUR] Furcraea initialized\n");return 0;}
int fur_planning(int t,int c,int a,int b,int d,int e,int y){return add(furps,&st.n_furp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fur_execution(int t,int c,int a,int b,int d,int e,int y){return add(furss,&st.n_fure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(furvss,&st.n_fur2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(furas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fur_market(int t,int c,int a,int b,int d,int e,int y){return add(furmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fur_report(void){ps("[FUR] Furrp: ");pi(st.n_furp);ps(" PCS=");pi(st.t_f1);ps("\nFure: ");pi(st.n_fure);ps(" PCS=");pi(st.t_f2);ps("\nFurv: ");pi(st.n_fur2);ps(" PCS=");pi(st.t_f3);ps("\nFurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fur_state(void){ps("[FUR] Furrp=");pi(st.n_furp);ps(" Fure=");pi(st.n_fure);ps(" Furv=");pi(st.n_fur2);ps(" Furc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Furcraea Admin Demo ===\n\n");fur_init();
ps("Furcraea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fur_planning(t,c,1001+(i*17),990+(i*14),970+(i*10),952+(i*6),2020+(i%5));}
ps("\nFurcraea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fur_execution(t,c,990+(i*15),979+(i*12),961+(i*8),948+(i*5),2021+(i%4));}
ps("\nFurcraea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fur_evaluation(t,c,982+(i*13),971+(i*10),955+(i*7),944+(i*4),2022+(i%3));}
ps("\nFurcraea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fur_accessory(t,c,974+(i*11),965+(i*9),951+(i*6),941+(i*3),2023+(i%2));}
ps("\nFurcraea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fur_market(t,c,968+(i*9),959+(i*7),946+(i*5),938+(i*3),2024);}
ps("\n");fur_report();fur_state();ps("\n=== Demo Complete ===\n");return 0;}
