/* winterberry_admin: Winterberry management technology administration (v1.0)
 * Winterberry planning, winterberry execution, winterberry evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} win_t;
typedef struct{int n_winp,n_wine,n_win2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} win_state_t;
static win_t winps[N],winss[N-2],winvss[N-4],winas[N-6],winmks[N-6]; static win_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(win_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;win_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int win_init(void){if(init)return -1;st.n_winp=0;st.n_wine=0;st.n_win2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)winps[i].active=0;for(int i=0;i<N-2;i++)winss[i].active=0;for(int i=0;i<N-4;i++)winvss[i].active=0;for(int i=0;i<N-6;i++)winas[i].active=0;for(int i=0;i<N-6;i++)winmks[i].active=0;init=1;ps("[WIN] Winterberry initialized\n");return 0;}
int win_planning(int t,int c,int a,int b,int d,int e,int y){return add(winps,&st.n_winp,&st.t_f1,N,t,c,a,b,d,e,y);}
int win_execution(int t,int c,int a,int b,int d,int e,int y){return add(winss,&st.n_wine,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int win_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(winvss,&st.n_win2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int win_accessory(int t,int c,int a,int b,int d,int e,int y){return add(winas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int win_market(int t,int c,int a,int b,int d,int e,int y){return add(winmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void win_report(void){ps("[WIN] Winp: ");pi(st.n_winp);ps(" PCS=");pi(st.t_f1);ps("\nWine: ");pi(st.n_wine);ps(" PCS=");pi(st.t_f2);ps("\nWinv: ");pi(st.n_win2);ps(" PCS=");pi(st.t_f3);ps("\nWinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void win_state(void){ps("[WIN] Winp=");pi(st.n_winp);ps(" Wine=");pi(st.n_wine);ps(" Winv=");pi(st.n_win2);ps(" Winc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Winterberry Admin Demo ===\n\n");win_init();
ps("Winterberry planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;win_planning(t,c,963+(i*17),952+(i*14),932+(i*10),914+(i*6),2020+(i%5));}
ps("\nWinterberry execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;win_execution(t,c,952+(i*15),941+(i*12),923+(i*8),910+(i*5),2021+(i%4));}
ps("\nWinterberry evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;win_evaluation(t,c,944+(i*13),933+(i*10),917+(i*7),906+(i*4),2022+(i%3));}
ps("\nWinterberry accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;win_accessory(t,c,936+(i*11),927+(i*9),913+(i*6),903+(i*3),2023+(i%2));}
ps("\nWinterberry marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;win_market(t,c,930+(i*9),921+(i*7),908+(i*5),900+(i*3),2024);}
ps("\n");win_report();win_state();ps("\n=== Demo Complete ===\n");return 0;}
