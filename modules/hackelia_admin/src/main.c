/* hackelia_admin: Hackelia management technology administration (v1.0)
 * Hackelia planning, hackelia execution, hackelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hack_t;
typedef struct{int n_hackp,n_hack,n_hack2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hack_state_t;
static hack_t hacks[N],hacke[N-2],hack2[N-4],hackac[N-6],hackam[N-6]; static hack_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hack_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hack_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HACK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hack_init(void){if(init)return -1;st.n_hackp=0;st.n_hack=0;st.n_hack2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hacks[i].active=0;for(int i=0;i<N-2;i++)hacke[i].active=0;for(int i=0;i<N-4;i++)hack2[i].active=0;for(int i=0;i<N-6;i++)hackac[i].active=0;for(int i=0;i<N-6;i++)hackam[i].active=0;init=1;ps("[HACK] Hackelia initialized\n");return 0;}
int hack_planning(int t,int c,int a,int b,int d,int e,int y){return add(hacks,&st.n_hackp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hack_execution(int t,int c,int a,int b,int d,int e,int y){return add(hacke,&st.n_hack,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hack_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hack2,&st.n_hack2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hack_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hackac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hack_market(int t,int c,int a,int b,int d,int e,int y){return add(hackam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hack_report(void){ps("[HACK] Hackp: ");pi(st.n_hackp);ps(" PCS=");pi(st.t_f1);ps("\nHacke: ");pi(st.n_hack);ps(" PCS=");pi(st.t_f2);ps("\nHack2: ");pi(st.n_hack2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hack_state(void){ps("[HACK] Hackp=");pi(st.n_hackp);ps(" Hack=");pi(st.n_hack);ps(" Hack2=");pi(st.n_hack2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hackelia Admin Demo ===\n\n");hack_init();
ps("Hackelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hack_planning(t,c,1525+(i*17),1514+(i*14),1494+(i*10),1476+(i*6),2020+(i%5));}
ps("\nHackelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hack_execution(t,c,1514+(i*15),1503+(i*12),1485+(i*8),1472+(i*5),2021+(i%4));}
ps("\nHackelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hack_evaluation(t,c,1506+(i*13),1495+(i*10),1479+(i*7),1468+(i*4),2022+(i%3));}
ps("\nHackelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hack_accessory(t,c,1498+(i*11),1489+(i*9),1475+(i*6),1465+(i*3),2023+(i%2));}
ps("\nHackelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hack_market(t,c,1492+(i*9),1483+(i*7),1470+(i*5),1462+(i*3),2024);}
ps("\n");hack_report();hack_state();ps("\n=== Demo Complete ===\n");return 0;}
