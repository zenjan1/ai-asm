/* luehea_admin: Luehea management technology administration (v1.0)
 * Luehea planning, luehea execution, luehea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lue_t;
typedef struct{int n_luep,n_luee,n_lue2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lue_state_t;
static lue_t lueps[N],luess[N-2],luevss[N-4],lueas[N-6],luemks[N-6]; static lue_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lue_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lue_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lue_init(void){if(init)return -1;st.n_luep=0;st.n_luee=0;st.n_lue2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lueps[i].active=0;for(int i=0;i<N-2;i++)luess[i].active=0;for(int i=0;i<N-4;i++)luevss[i].active=0;for(int i=0;i<N-6;i++)lueas[i].active=0;for(int i=0;i<N-6;i++)luemks[i].active=0;init=1;ps("[LUE] Luehea initialized\n");return 0;}
int lue_planning(int t,int c,int a,int b,int d,int e,int y){return add(lueps,&st.n_luep,&st.t_f1,N,t,c,a,b,d,e,y);}
int lue_execution(int t,int c,int a,int b,int d,int e,int y){return add(luess,&st.n_luee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lue_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(luevss,&st.n_lue2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lue_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lueas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lue_market(int t,int c,int a,int b,int d,int e,int y){return add(luemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lue_report(void){ps("[LUE] Luep: ");pi(st.n_luep);ps(" PCS=");pi(st.t_f1);ps("\nLuee: ");pi(st.n_luee);ps(" PCS=");pi(st.t_f2);ps("\nLuev: ");pi(st.n_lue2);ps(" PCS=");pi(st.t_f3);ps("\nLuec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lue_state(void){ps("[LUE] Luep=");pi(st.n_luep);ps(" Luee=");pi(st.n_luee);ps(" Luev=");pi(st.n_lue2);ps(" Luec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Luehea Admin Demo ===\n\n");lue_init();
ps("Luehea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lue_planning(t,c,1061+(i*17),1050+(i*14),1030+(i*10),1012+(i*6),2020+(i%5));}
ps("\nLuehea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lue_execution(t,c,1050+(i*15),1039+(i*12),1021+(i*8),1008+(i*5),2021+(i%4));}
ps("\nLuehea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lue_evaluation(t,c,1042+(i*13),1031+(i*10),1015+(i*7),1004+(i*4),2022+(i%3));}
ps("\nLuehea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lue_accessory(t,c,1034+(i*11),1025+(i*9),1011+(i*6),1001+(i*3),2023+(i%2));}
ps("\nLuehea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lue_market(t,c,1028+(i*9),1019+(i*7),1006+(i*5),998+(i*3),2024);}
ps("\n");lue_report();lue_state();ps("\n=== Demo Complete ===\n");return 0;}
