/* vineyard_admin: Vineyard management technology administration (v1.0)
 * Vineyard planning, vineyard execution, vineyard evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vny_t;
typedef struct{int n_vp,n_ve,n_vv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vny_state_t;
static vny_t vps[N],ves[N-2],vvs[N-4],acs[N-6],mks[N-6]; static vny_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vny_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vny_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VNY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vny_init(void){if(init)return -1;st.n_vp=0;st.n_ve=0;st.n_vv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vps[i].active=0;for(int i=0;i<N-2;i++)ves[i].active=0;for(int i=0;i<N-4;i++)vvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[VNY] Vineyard initialized\n");return 0;}
int vny_planning(int t,int c,int a,int b,int d,int e,int y){return add(vps,&st.n_vp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vny_execution(int t,int c,int a,int b,int d,int e,int y){return add(ves,&st.n_ve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vny_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vvs,&st.n_vv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vny_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vny_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vny_report(void){ps("[VNY] Vp: ");pi(st.n_vp);ps(" PCS=");pi(st.t_f1);ps("\nVe: ");pi(st.n_ve);ps(" PCS=");pi(st.t_f2);ps("\nVv: ");pi(st.n_vv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vny_state(void){ps("[VNY] Vp=");pi(st.n_vp);ps(" Ve=");pi(st.n_ve);ps(" Vv=");pi(st.n_vv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vineyard Admin Demo ===\n\n");vny_init();
ps("Vineyard planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vny_planning(t,c,513+(i*17),502+(i*14),482+(i*10),464+(i*6),2020+(i%5));}
ps("\nVineyard execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vny_execution(t,c,502+(i*15),491+(i*12),473+(i*8),460+(i*5),2021+(i%4));}
ps("\nVineyard evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vny_evaluation(t,c,494+(i*13),483+(i*10),467+(i*7),456+(i*4),2022+(i%3));}
ps("\nVineyard accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vny_accessory(t,c,486+(i*11),477+(i*9),463+(i*6),453+(i*3),2023+(i%2));}
ps("\nVineyard marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vny_market(t,c,480+(i*9),471+(i*7),458+(i*5),450+(i*3),2024);}
ps("\n");vny_report();vny_state();ps("\n=== Demo Complete ===\n");return 0;}
