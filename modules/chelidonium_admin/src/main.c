/* chelidonium_admin: Chelidonium management technology administration (v1.0)
 * Chelidonium planning, chelidonium execution, chelidonium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chel_t;
typedef struct{int n_chelp,n_chel,n_chel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chel_state_t;
static chel_t chels[N],chele[N-2],chel2[N-4],chelac[N-6],chelim[N-6]; static chel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(chel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chel_init(void){if(init)return -1;st.n_chelp=0;st.n_chel=0;st.n_chel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chels[i].active=0;for(int i=0;i<N-2;i++)chele[i].active=0;for(int i=0;i<N-4;i++)chel2[i].active=0;for(int i=0;i<N-6;i++)chelac[i].active=0;for(int i=0;i<N-6;i++)chelim[i].active=0;init=1;ps("[CHEL] Chelidonium initialized\n");return 0;}
int chel_planning(int t,int c,int a,int b,int d,int e,int y){return add(chels,&st.n_chelp,&st.t_f1,N,t,c,a,b,d,e,y);}
int chel_execution(int t,int c,int a,int b,int d,int e,int y){return add(chele,&st.n_chel,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chel2,&st.n_chel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chelac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chel_market(int t,int c,int a,int b,int d,int e,int y){return add(chelim,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chel_report(void){ps("[CHEL] Hlp: ");pi(st.n_chelp);ps(" PCS=");pi(st.t_f1);ps("\nHle: ");pi(st.n_chel);ps(" PCS=");pi(st.t_f2);ps("\nHl2: ");pi(st.n_chel2);ps(" PCS=");pi(st.t_f3);ps("\nHlac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chel_state(void){ps("[CHEL] Hlp=");pi(st.n_chelp);ps(" Hl=");pi(st.n_chel);ps(" Hl2=");pi(st.n_chel2);ps(" Hlac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chelidonium Admin Demo ===\n\n");chel_init();
ps("Chelidonium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chel_planning(t,c,1473+(i*17),1462+(i*14),1442+(i*10),1424+(i*6),2020+(i%5));}
ps("\nChelidonium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chel_execution(t,c,1462+(i*15),1451+(i*12),1433+(i*8),1420+(i*5),2021+(i%4));}
ps("\nChelidonium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chel_evaluation(t,c,1454+(i*13),1443+(i*10),1427+(i*7),1416+(i*4),2022+(i%3));}
ps("\nChelidonium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chel_accessory(t,c,1446+(i*11),1437+(i*9),1423+(i*6),1413+(i*3),2023+(i%2));}
ps("\nChelidonium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chel_market(t,c,1440+(i*9),1431+(i*7),1418+(i*5),1410+(i*3),2024);}
ps("\n");chel_report();chel_state();ps("\n=== Demo Complete ===\n");return 0;}
