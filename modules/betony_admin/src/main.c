/* betony_admin: Betony management technology administration (v1.0)
 * Betony planning, betony execution, betony evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bt_t;
typedef struct{int n_btn,n_bte,n_btv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bt_state_t;
static bt_t btps[N],btss[N-2],btvss[N-4],btas[N-6],btmks[N-6]; static bt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BTN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bt_init(void){if(init)return -1;st.n_btn=0;st.n_bte=0;st.n_btv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)btps[i].active=0;for(int i=0;i<N-2;i++)btss[i].active=0;for(int i=0;i<N-4;i++)btvss[i].active=0;for(int i=0;i<N-6;i++)btas[i].active=0;for(int i=0;i<N-6;i++)btmks[i].active=0;init=1;ps("[BTN] Betony initialized\n");return 0;}
int bt_planning(int t,int c,int a,int b,int d,int e,int y){return add(btps,&st.n_btn,&st.t_f1,N,t,c,a,b,d,e,y);}
int bt_execution(int t,int c,int a,int b,int d,int e,int y){return add(btss,&st.n_bte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(btvss,&st.n_btv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(btas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bt_market(int t,int c,int a,int b,int d,int e,int y){return add(btmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bt_report(void){ps("[BTN] Btn: ");pi(st.n_btn);ps(" PCS=");pi(st.t_f1);ps("\nBte: ");pi(st.n_bte);ps(" PCS=");pi(st.t_f2);ps("\nBtv: ");pi(st.n_btv);ps(" PCS=");pi(st.t_f3);ps("\nBtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bt_state(void){ps("[BTN] Btn=");pi(st.n_btn);ps(" Bte=");pi(st.n_bte);ps(" Btv=");pi(st.n_btv);ps(" Btc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Betony Admin Demo ===\n\n");bt_init();
ps("Betony planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bt_planning(t,c,692+(i*17),681+(i*14),661+(i*10),643+(i*6),2020+(i%5));}
ps("\nBetony execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bt_execution(t,c,681+(i*15),670+(i*12),652+(i*8),639+(i*5),2021+(i%4));}
ps("\nBetony evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bt_evaluation(t,c,673+(i*13),662+(i*10),646+(i*7),635+(i*4),2022+(i%3));}
ps("\nBetony accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bt_accessory(t,c,665+(i*11),656+(i*9),642+(i*6),632+(i*3),2023+(i%2));}
ps("\nBetony marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bt_market(t,c,659+(i*9),650+(i*7),637+(i*5),629+(i*3),2024);}
ps("\n");bt_report();bt_state();ps("\n=== Demo Complete ===\n");return 0;}
