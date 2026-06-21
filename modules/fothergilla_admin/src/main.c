/* fothergilla_admin: Fothergilla management technology administration (v1.0)
 * Fothergilla planning, fothergilla execution, fothergilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ft_t;
typedef struct{int n_ftgp,n_ftge,n_ftgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ft_state_t;
static ft_t ftgps[N],ftgss[N-2],ftgvss[N-4],ftgas[N-6],ftgmks[N-6]; static ft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FTG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ftg_init(void){if(init)return -1;st.n_ftgp=0;st.n_ftge=0;st.n_ftgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ftgps[i].active=0;for(int i=0;i<N-2;i++)ftgss[i].active=0;for(int i=0;i<N-4;i++)ftgvss[i].active=0;for(int i=0;i<N-6;i++)ftgas[i].active=0;for(int i=0;i<N-6;i++)ftgmks[i].active=0;init=1;ps("[FTG] Fothergilla initialized\n");return 0;}
int ftg_planning(int t,int c,int a,int b,int d,int e,int y){return add(ftgps,&st.n_ftgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ftg_execution(int t,int c,int a,int b,int d,int e,int y){return add(ftgss,&st.n_ftge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ftg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ftgvss,&st.n_ftgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ftg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ftgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ftg_market(int t,int c,int a,int b,int d,int e,int y){return add(ftgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ftg_report(void){ps("[FTG] Ftgp: ");pi(st.n_ftgp);ps(" PCS=");pi(st.t_f1);ps("\nFtge: ");pi(st.n_ftge);ps(" PCS=");pi(st.t_f2);ps("\nFtgv: ");pi(st.n_ftgv);ps(" PCS=");pi(st.t_f3);ps("\nFtgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ftg_state(void){ps("[FTG] Ftgp=");pi(st.n_ftgp);ps(" Ftge=");pi(st.n_ftge);ps(" Ftgv=");pi(st.n_ftgv);ps(" Ftgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fothergilla Admin Demo ===\n\n");ftg_init();
ps("Fothergilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ftg_planning(t,c,774+(i*17),763+(i*14),743+(i*10),725+(i*6),2020+(i%5));}
ps("\nFothergilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ftg_execution(t,c,763+(i*15),752+(i*12),734+(i*8),721+(i*5),2021+(i%4));}
ps("\nFothergilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ftg_evaluation(t,c,755+(i*13),744+(i*10),730+(i*7),719+(i*4),2022+(i%3));}
ps("\nFothergilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ftg_accessory(t,c,747+(i*11),738+(i*9),724+(i*6),714+(i*3),2023+(i%2));}
ps("\nFothergilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ftg_market(t,c,741+(i*9),732+(i*7),719+(i*5),711+(i*3),2024);}
ps("\n");ftg_report();ftg_state();ps("\n=== Demo Complete ===\n");return 0;}
