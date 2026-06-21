/* juglans_admin: Juglans management technology administration (v1.0)
 * Juglans planning, juglans execution, juglans evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jug_t;
typedef struct{int n_jugp,n_juge,n_jug2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jug_state_t;
static jug_t jugps[N],jugss[N-2],jugvss[N-4],jugas[N-6],jugmks[N-6]; static jug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jug_init(void){if(init)return -1;st.n_jugp=0;st.n_juge=0;st.n_jug2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jugps[i].active=0;for(int i=0;i<N-2;i++)jugss[i].active=0;for(int i=0;i<N-4;i++)jugvss[i].active=0;for(int i=0;i<N-6;i++)jugas[i].active=0;for(int i=0;i<N-6;i++)jugmks[i].active=0;init=1;ps("[JUG] Juglans initialized\n");return 0;}
int jug_planning(int t,int c,int a,int b,int d,int e,int y){return add(jugps,&st.n_jugp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jug_execution(int t,int c,int a,int b,int d,int e,int y){return add(jugss,&st.n_juge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jugvss,&st.n_jug2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jugas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jug_market(int t,int c,int a,int b,int d,int e,int y){return add(jugmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jug_report(void){ps("[JUG] Jugp: ");pi(st.n_jugp);ps(" PCS=");pi(st.t_f1);ps("\nJuge: ");pi(st.n_juge);ps(" PCS=");pi(st.t_f2);ps("\nJugv: ");pi(st.n_jug2);ps(" PCS=");pi(st.t_f3);ps("\nJugc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jug_state(void){ps("[JUG] Jugp=");pi(st.n_jugp);ps(" Juge=");pi(st.n_juge);ps(" Jugv=");pi(st.n_jug2);ps(" Jugc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Juglans Admin Demo ===\n\n");jug_init();
ps("Juglans planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jug_planning(t,c,1032+(i*17),1021+(i*14),1001+(i*10),983+(i*6),2020+(i%5));}
ps("\nJuglans execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jug_execution(t,c,1021+(i*15),1010+(i*12),992+(i*8),979+(i*5),2021+(i%4));}
ps("\nJuglans evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jug_evaluation(t,c,1013+(i*13),1002+(i*10),986+(i*7),975+(i*4),2022+(i%3));}
ps("\nJuglans accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jug_accessory(t,c,1005+(i*11),996+(i*9),982+(i*6),972+(i*3),2023+(i%2));}
ps("\nJuglans marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jug_market(t,c,999+(i*9),990+(i*7),977+(i*5),969+(i*3),2024);}
ps("\n");jug_report();jug_state();ps("\n=== Demo Complete ===\n");return 0;}
