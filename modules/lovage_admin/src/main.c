/* lovage_admin: Lovage management technology administration (v1.0)
 * Lovage planning, lovage execution, lovage evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lv_t;
typedef struct{int n_lvp,n_lve,n_lvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lv_state_t;
static lv_t lvps[N],lvss[N-2],lvvss[N-4],lvas[N-6],lvmks[N-6]; static lv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LVG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lv_init(void){if(init)return -1;st.n_lvp=0;st.n_lve=0;st.n_lvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lvps[i].active=0;for(int i=0;i<N-2;i++)lvss[i].active=0;for(int i=0;i<N-4;i++)lvvss[i].active=0;for(int i=0;i<N-6;i++)lvas[i].active=0;for(int i=0;i<N-6;i++)lvmks[i].active=0;init=1;ps("[LVG] Lovage initialized\n");return 0;}
int lv_planning(int t,int c,int a,int b,int d,int e,int y){return add(lvps,&st.n_lvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lv_execution(int t,int c,int a,int b,int d,int e,int y){return add(lvss,&st.n_lve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lvvss,&st.n_lvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lvas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lv_market(int t,int c,int a,int b,int d,int e,int y){return add(lvmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lv_report(void){ps("[LVG] Lvp: ");pi(st.n_lvp);ps(" PCS=");pi(st.t_f1);ps("\nLve: ");pi(st.n_lve);ps(" PCS=");pi(st.t_f2);ps("\nLvv: ");pi(st.n_lvv);ps(" PCS=");pi(st.t_f3);ps("\nLvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lv_state(void){ps("[LVG] Lvp=");pi(st.n_lvp);ps(" Lve=");pi(st.n_lve);ps(" Lvv=");pi(st.n_lvv);ps(" Lvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lovage Admin Demo ===\n\n");lv_init();
ps("Lovage planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lv_planning(t,c,685+(i*17),674+(i*14),654+(i*10),636+(i*6),2020+(i%5));}
ps("\nLovage execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lv_execution(t,c,674+(i*15),663+(i*12),645+(i*8),632+(i*5),2021+(i%4));}
ps("\nLovage evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lv_evaluation(t,c,666+(i*13),655+(i*10),639+(i*7),628+(i*4),2022+(i%3));}
ps("\nLovage accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lv_accessory(t,c,658+(i*11),649+(i*9),635+(i*6),625+(i*3),2023+(i%2));}
ps("\nLovage marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lv_market(t,c,652+(i*9),643+(i*7),630+(i*5),622+(i*3),2024);}
ps("\n");lv_report();lv_state();ps("\n=== Demo Complete ===\n");return 0;}
