/* gynerium_admin: Gynerium management technology administration (v1.0)
 * Gynerium planning, gynerium execution, gynerium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gyn_t;
typedef struct{int n_gynp,n_gyne,n_gyn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gyn_state_t;
static gyn_t gynps[N],gynss[N-2],gynvss[N-4],gynas[N-6],gynmks[N-6]; static gyn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gyn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gyn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GYN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gyn_init(void){if(init)return -1;st.n_gynp=0;st.n_gyne=0;st.n_gyn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gynps[i].active=0;for(int i=0;i<N-2;i++)gynss[i].active=0;for(int i=0;i<N-4;i++)gynvss[i].active=0;for(int i=0;i<N-6;i++)gynas[i].active=0;for(int i=0;i<N-6;i++)gynmks[i].active=0;init=1;ps("[GYN] Gynerium initialized\n");return 0;}
int gyn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gynps,&st.n_gynp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gyn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gynss,&st.n_gyne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gyn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gynvss,&st.n_gyn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gyn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gynas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gyn_market(int t,int c,int a,int b,int d,int e,int y){return add(gynmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gyn_report(void){ps("[GYN] Gynp: ");pi(st.n_gynp);ps(" PCS=");pi(st.t_f1);ps("\nGyne: ");pi(st.n_gyne);ps(" PCS=");pi(st.t_f2);ps("\nGynv: ");pi(st.n_gyn2);ps(" PCS=");pi(st.t_f3);ps("\nGync: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gyn_state(void){ps("[GYN] Gynp=");pi(st.n_gynp);ps(" Gyne=");pi(st.n_gyne);ps(" Gynv=");pi(st.n_gyn2);ps(" Gync=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gynerium Admin Demo ===\n\n");gyn_init();
ps("Gynerium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gyn_planning(t,c,1017+(i*17),1006+(i*14),986+(i*10),968+(i*6),2020+(i%5));}
ps("\nGynerium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gyn_execution(t,c,1006+(i*15),995+(i*12),977+(i*8),964+(i*5),2021+(i%4));}
ps("\nGynerium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gyn_evaluation(t,c,998+(i*13),987+(i*10),971+(i*7),960+(i*4),2022+(i%3));}
ps("\nGynerium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gyn_accessory(t,c,990+(i*11),981+(i*9),967+(i*6),957+(i*3),2023+(i%2));}
ps("\nGynerium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gyn_market(t,c,984+(i*9),975+(i*7),962+(i*5),954+(i*3),2024);}
ps("\n");gyn_report();gyn_state();ps("\n=== Demo Complete ===\n");return 0;}
