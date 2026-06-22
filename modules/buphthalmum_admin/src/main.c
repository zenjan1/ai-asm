/* buphthalmum_admin: Buphthalmum management technology administration (v1.0)
 * Buphthalmum planning, buphthalmum execution, buphthalmum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} buph_t;
typedef struct{int n_buphp,n_buph,n_buph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} buph_state_t;
static buph_t buphs[N],buphe[N-2],buph2[N-4],buphac[N-6],bupham[N-6]; static buph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(buph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;buph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int buph_init(void){if(init)return -1;st.n_buphp=0;st.n_buph=0;st.n_buph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)buphs[i].active=0;for(int i=0;i<N-2;i++)buphe[i].active=0;for(int i=0;i<N-4;i++)buph2[i].active=0;for(int i=0;i<N-6;i++)buphac[i].active=0;for(int i=0;i<N-6;i++)bupham[i].active=0;init=1;ps("[BUPH] Buphthalmum initialized\n");return 0;}
int buph_planning(int t,int c,int a,int b,int d,int e,int y){return add(buphs,&st.n_buphp,&st.t_f1,N,t,c,a,b,d,e,y);}
int buph_execution(int t,int c,int a,int b,int d,int e,int y){return add(buphe,&st.n_buph,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int buph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(buph2,&st.n_buph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int buph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(buphac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int buph_market(int t,int c,int a,int b,int d,int e,int y){return add(bupham,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void buph_report(void){ps("[BUPH] Hp: ");pi(st.n_buphp);ps(" PCS=");pi(st.t_f1);ps("\nHe: ");pi(st.n_buph);ps(" PCS=");pi(st.t_f2);ps("\nHh2: ");pi(st.n_buph2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void buph_state(void){ps("[BUPH] Hp=");pi(st.n_buphp);ps(" Hh=");pi(st.n_buph);ps(" Hh2=");pi(st.n_buph2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buphthalmum Admin Demo ===\n\n");buph_init();
ps("Buphthalmum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;buph_planning(t,c,1449+(i*17),1438+(i*14),1418+(i*10),1400+(i*6),2020+(i%5));}
ps("\nBuphthalmum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;buph_execution(t,c,1438+(i*15),1427+(i*12),1409+(i*8),1396+(i*5),2021+(i%4));}
ps("\nBuphthalmum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;buph_evaluation(t,c,1430+(i*13),1419+(i*10),1403+(i*7),1392+(i*4),2022+(i%3));}
ps("\nBuphthalmum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;buph_accessory(t,c,1422+(i*11),1413+(i*9),1399+(i*6),1389+(i*3),2023+(i%2));}
ps("\nBuphthalmum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;buph_market(t,c,1416+(i*9),1407+(i*7),1394+(i*5),1386+(i*3),2024);}
ps("\n");buph_report();buph_state();ps("\n=== Demo Complete ===\n");return 0;}
