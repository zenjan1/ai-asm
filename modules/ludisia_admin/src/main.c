/* ludisia_admin: Ludisia management technology administration (v1.0)
 * Ludisia planning, ludisia execution, ludisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lud_t;
typedef struct{int n_ludp,n_lude,n_lud2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lud_state_t;
static lud_t ludps[N],ludss[N-2],ludvss[N-4],ludas[N-6],ludmks[N-6]; static lud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lud_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lud_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lud_init(void){if(init)return -1;st.n_ludp=0;st.n_lude=0;st.n_lud2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ludps[i].active=0;for(int i=0;i<N-2;i++)ludss[i].active=0;for(int i=0;i<N-4;i++)ludvss[i].active=0;for(int i=0;i<N-6;i++)ludas[i].active=0;for(int i=0;i<N-6;i++)ludmks[i].active=0;init=1;ps("[LUD] Ludisia initialized\n");return 0;}
int lud_planning(int t,int c,int a,int b,int d,int e,int y){return add(ludps,&st.n_ludp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lud_execution(int t,int c,int a,int b,int d,int e,int y){return add(ludss,&st.n_lude,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lud_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ludvss,&st.n_lud2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lud_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ludas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lud_market(int t,int c,int a,int b,int d,int e,int y){return add(ludmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lud_report(void){ps("[LUD] Ludp: ");pi(st.n_ludp);ps(" PCS=");pi(st.t_f1);ps("\nLude: ");pi(st.n_lude);ps(" PCS=");pi(st.t_f2);ps("\nLudv: ");pi(st.n_lud2);ps(" PCS=");pi(st.t_f3);ps("\nLudc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lud_state(void){ps("[LUD] Ludp=");pi(st.n_ludp);ps(" Lude=");pi(st.n_lude);ps(" Ludv=");pi(st.n_lud2);ps(" Ludc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ludisia Admin Demo ===\n\n");lud_init();
ps("Ludisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lud_planning(t,c,1060+(i*17),1049+(i*14),1029+(i*10),1011+(i*6),2020+(i%5));}
ps("\nLudisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lud_execution(t,c,1049+(i*15),1038+(i*12),1020+(i*8),1007+(i*5),2021+(i%4));}
ps("\nLudisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lud_evaluation(t,c,1041+(i*13),1030+(i*10),1014+(i*7),1003+(i*4),2022+(i%3));}
ps("\nLudisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lud_accessory(t,c,1033+(i*11),1024+(i*9),1010+(i*6),1000+(i*3),2023+(i%2));}
ps("\nLudisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lud_market(t,c,1027+(i*9),1018+(i*7),1005+(i*5),997+(i*3),2024);}
ps("\n");lud_report();lud_state();ps("\n=== Demo Complete ===\n");return 0;}
