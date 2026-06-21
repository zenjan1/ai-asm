/* rhododendron_admin: Rhododendron management technology administration (v1.0)
 * Rhododendron planning, rhododendron execution, rhododendron evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rh_t;
typedef struct{int n_rhp,n_rhe,n_rhv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rh_state_t;
static rh_t rhps[N],rhss[N-2],rhvs[N-4],rhas[N-6],rhmks[N-6]; static rh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rh_init(void){if(init)return -1;st.n_rhp=0;st.n_rhe=0;st.n_rhv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhps[i].active=0;for(int i=0;i<N-2;i++)rhss[i].active=0;for(int i=0;i<N-4;i++)rhvs[i].active=0;for(int i=0;i<N-6;i++)rhas[i].active=0;for(int i=0;i<N-6;i++)rhmks[i].active=0;init=1;ps("[RHD] Rhododendron initialized\n");return 0;}
int rh_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhps,&st.n_rhp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rh_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhss,&st.n_rhe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rh_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhvs,&st.n_rhv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rh_market(int t,int c,int a,int b,int d,int e,int y){return add(rhmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rh_report(void){ps("[RHD] Rhp: ");pi(st.n_rhp);ps(" PCS=");pi(st.t_f1);ps("\nRhe: ");pi(st.n_rhe);ps(" PCS=");pi(st.t_f2);ps("\nRhv: ");pi(st.n_rhv);ps(" PCS=");pi(st.t_f3);ps("\nRhac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rh_state(void){ps("[RHD] Rhp=");pi(st.n_rhp);ps(" Rhe=");pi(st.n_rhe);ps(" Rhv=");pi(st.n_rhv);ps(" Rhac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhododendron Admin Demo ===\n\n");rh_init();
ps("Rhododendron planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rh_planning(t,c,644+(i*17),633+(i*14),613+(i*10),595+(i*6),2020+(i%5));}
ps("\nRhododendron execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rh_execution(t,c,633+(i*15),622+(i*12),604+(i*8),591+(i*5),2021+(i%4));}
ps("\nRhododendron evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rh_evaluation(t,c,625+(i*13),614+(i*10),598+(i*7),587+(i*4),2022+(i%3));}
ps("\nRhododendron accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rh_accessory(t,c,617+(i*11),608+(i*9),594+(i*6),584+(i*3),2023+(i%2));}
ps("\nRhododendron marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rh_market(t,c,611+(i*9),602+(i*7),589+(i*5),581+(i*3),2024);}
ps("\n");rh_report();rh_state();ps("\n=== Demo Complete ===\n");return 0;}
