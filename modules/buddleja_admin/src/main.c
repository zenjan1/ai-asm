/* buddleja_admin: Buddleja management technology administration (v1.0)
 * Buddleja planning, buddleja execution, buddleja evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} budd_t;
typedef struct{int n_buddp,n_budd,n_budd2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} budd_state_t;
static budd_t budds[N],budde[N-2],budd2[N-4],buddac[N-6],buddam[N-6]; static budd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(budd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;budd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUDD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int budd_init(void){if(init)return -1;st.n_buddp=0;st.n_budd=0;st.n_budd2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)budds[i].active=0;for(int i=0;i<N-2;i++)budde[i].active=0;for(int i=0;i<N-4;i++)budd2[i].active=0;for(int i=0;i<N-6;i++)buddac[i].active=0;for(int i=0;i<N-6;i++)buddam[i].active=0;init=1;ps("[BUDD] Buddleja initialized\n");return 0;}
int budd_planning(int t,int c,int a,int b,int d,int e,int y){return add(budds,&st.n_buddp,&st.t_f1,N,t,c,a,b,d,e,y);}
int budd_execution(int t,int c,int a,int b,int d,int e,int y){return add(budde,&st.n_budd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int budd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(budd2,&st.n_budd2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int budd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(buddac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int budd_market(int t,int c,int a,int b,int d,int e,int y){return add(buddam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void budd_report(void){ps("[BUDD] Dup: ");pi(st.n_buddp);ps(" PCS=");pi(st.t_f1);ps("\nDue: ");pi(st.n_budd);ps(" PCS=");pi(st.t_f2);ps("\nDu2: ");pi(st.n_budd2);ps(" PCS=");pi(st.t_f3);ps("\nDac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void budd_state(void){ps("[BUDD] Dup=");pi(st.n_buddp);ps(" Du=");pi(st.n_budd);ps(" Du2=");pi(st.n_budd2);ps(" Dac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buddleja Admin Demo ===\n\n");budd_init();
ps("Buddleja planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;budd_planning(t,c,1446+(i*17),1435+(i*14),1415+(i*10),1397+(i*6),2020+(i%5));}
ps("\nBuddleja execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;budd_execution(t,c,1435+(i*15),1424+(i*12),1406+(i*8),1393+(i*5),2021+(i%4));}
ps("\nBuddleja evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;budd_evaluation(t,c,1427+(i*13),1416+(i*10),1400+(i*7),1389+(i*4),2022+(i%3));}
ps("\nBuddleja accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;budd_accessory(t,c,1419+(i*11),1410+(i*9),1396+(i*6),1386+(i*3),2023+(i%2));}
ps("\nBuddleja marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;budd_market(t,c,1413+(i*9),1404+(i*7),1391+(i*5),1383+(i*3),2024);}
ps("\n");budd_report();budd_state();ps("\n=== Demo Complete ===\n");return 0;}
