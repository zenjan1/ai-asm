/* leveria_admin: Leveria management technology administration (v1.0)
 * Leveria planning, leveria execution, leveria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lev_t;
typedef struct{int n_levp,n_leve,n_lev2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lev_state_t;
static lev_t levps[N],levss[N-2],levvss[N-4],levas[N-6],levmks[N-6]; static lev_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lev_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lev_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lev_init(void){if(init)return -1;st.n_levp=0;st.n_leve=0;st.n_lev2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)levps[i].active=0;for(int i=0;i<N-2;i++)levss[i].active=0;for(int i=0;i<N-4;i++)levvss[i].active=0;for(int i=0;i<N-6;i++)levas[i].active=0;for(int i=0;i<N-6;i++)levmks[i].active=0;init=1;ps("[LEV] Leveria initialized\n");return 0;}
int lev_planning(int t,int c,int a,int b,int d,int e,int y){return add(levps,&st.n_levp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lev_execution(int t,int c,int a,int b,int d,int e,int y){return add(levss,&st.n_leve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lev_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(levvss,&st.n_lev2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lev_accessory(int t,int c,int a,int b,int d,int e,int y){return add(levas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lev_market(int t,int c,int a,int b,int d,int e,int y){return add(levmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lev_report(void){ps("[LEV] Levp: ");pi(st.n_levp);ps(" PCS=");pi(st.t_f1);ps("\nLeve: ");pi(st.n_leve);ps(" PCS=");pi(st.t_f2);ps("\nLevv: ");pi(st.n_lev2);ps(" PCS=");pi(st.t_f3);ps("\nLevc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lev_state(void){ps("[LEV] Levp=");pi(st.n_levp);ps(" Leve=");pi(st.n_leve);ps(" Levv=");pi(st.n_lev2);ps(" Levc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leveria Admin Demo ===\n\n");lev_init();
ps("Leveria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lev_planning(t,c,1053+(i*17),1042+(i*14),1022+(i*10),1004+(i*6),2020+(i%5));}
ps("\nLeveria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lev_execution(t,c,1042+(i*15),1031+(i*12),1013+(i*8),1000+(i*5),2021+(i%4));}
ps("\nLeveria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lev_evaluation(t,c,1034+(i*13),1023+(i*10),1007+(i*7),996+(i*4),2022+(i%3));}
ps("\nLeveria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lev_accessory(t,c,1026+(i*11),1017+(i*9),1003+(i*6),993+(i*3),2023+(i%2));}
ps("\nLeveria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lev_market(t,c,1020+(i*9),1011+(i*7),998+(i*5),990+(i*3),2024);}
ps("\n");lev_report();lev_state();ps("\n=== Demo Complete ===\n");return 0;}
