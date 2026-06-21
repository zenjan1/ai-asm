/* bougainvillea_admin: Bougainvillea management technology administration (v1.0)
 * Bougainvillea planning, bougainvillea execution, bougainvillea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bv_t;
typedef struct{int n_bvp,n_bve,n_bvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bv_state_t;
static bv_t bvps[N],bvss[N-2],bvvs[N-4],bvas[N-6],bvmks[N-6]; static bv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BVG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bv_init(void){if(init)return -1;st.n_bvp=0;st.n_bve=0;st.n_bvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bvps[i].active=0;for(int i=0;i<N-2;i++)bvss[i].active=0;for(int i=0;i<N-4;i++)bvvs[i].active=0;for(int i=0;i<N-6;i++)bvas[i].active=0;for(int i=0;i<N-6;i++)bvmks[i].active=0;init=1;ps("[BVG] Bougainvillea initialized\n");return 0;}
int bv_planning(int t,int c,int a,int b,int d,int e,int y){return add(bvps,&st.n_bvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bv_execution(int t,int c,int a,int b,int d,int e,int y){return add(bvss,&st.n_bve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bvvs,&st.n_bvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bvas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bv_market(int t,int c,int a,int b,int d,int e,int y){return add(bvmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bv_report(void){ps("[BVG] Bvp: ");pi(st.n_bvp);ps(" PCS=");pi(st.t_f1);ps("\nBve: ");pi(st.n_bve);ps(" PCS=");pi(st.t_f2);ps("\nBvv: ");pi(st.n_bvv);ps(" PCS=");pi(st.t_f3);ps("\nBvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bv_state(void){ps("[BVG] Bvp=");pi(st.n_bvp);ps(" Bve=");pi(st.n_bve);ps(" Bvv=");pi(st.n_bvv);ps(" Bvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bougainvillea Admin Demo ===\n\n");bv_init();
ps("Bougainvillea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bv_planning(t,c,643+(i*17),632+(i*14),612+(i*10),594+(i*6),2020+(i%5));}
ps("\nBougainvillea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bv_execution(t,c,632+(i*15),621+(i*12),603+(i*8),590+(i*5),2021+(i%4));}
ps("\nBougainvillea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bv_evaluation(t,c,624+(i*13),613+(i*10),597+(i*7),586+(i*4),2022+(i%3));}
ps("\nBougainvillea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bv_accessory(t,c,616+(i*11),607+(i*9),593+(i*6),583+(i*3),2023+(i%2));}
ps("\nBougainvillea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bv_market(t,c,610+(i*9),601+(i*7),588+(i*5),580+(i*3),2024);}
ps("\n");bv_report();bv_state();ps("\n=== Demo Complete ===\n");return 0;}
