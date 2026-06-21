/* rhamnus_admin: Rhamnus management technology administration (v1.0)
 * Rhamnus planning, rhamnus execution, rhamnus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rhm_t;
typedef struct{int n_rhp,n_rhe,n_rhv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rhm_state_t;
static rhm_t rhmss[N],rhmess[N-2],rhmvss[N-4],rhmas[N-6],rhmmks[N-6]; static rhm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rhm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rhm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rhm_init(void){if(init)return -1;st.n_rhp=0;st.n_rhe=0;st.n_rhv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhmss[i].active=0;for(int i=0;i<N-2;i++)rhmess[i].active=0;for(int i=0;i<N-4;i++)rhmvss[i].active=0;for(int i=0;i<N-6;i++)rhmas[i].active=0;for(int i=0;i<N-6;i++)rhmmks[i].active=0;init=1;ps("[RHM] Rhamnus initialized\n");return 0;}
int rhm_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhmss,&st.n_rhp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rhm_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhmess,&st.n_rhe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rhm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhmvss,&st.n_rhv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rhm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rhm_market(int t,int c,int a,int b,int d,int e,int y){return add(rhmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rhm_report(void){ps("[RHM] Rhp: ");pi(st.n_rhp);ps(" PCS=");pi(st.t_f1);ps("\nRhe: ");pi(st.n_rhe);ps(" PCS=");pi(st.t_f2);ps("\nRhv: ");pi(st.n_rhv);ps(" PCS=");pi(st.t_f3);ps("\nRhc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rhm_state(void){ps("[RHM] Rhp=");pi(st.n_rhp);ps(" Rhe=");pi(st.n_rhe);ps(" Rhv=");pi(st.n_rhv);ps(" Rhc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhamnus Admin Demo ===\n\n");rhm_init();
ps("Rhamnus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rhm_planning(t,c,856+(i*17),845+(i*14),825+(i*10),807+(i*6),2020+(i%5));}
ps("\nRhamnus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rhm_execution(t,c,845+(i*15),834+(i*12),816+(i*8),803+(i*5),2021+(i%4));}
ps("\nRhamnus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rhm_evaluation(t,c,837+(i*13),826+(i*10),810+(i*7),799+(i*4),2022+(i%3));}
ps("\nRhamnus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhm_accessory(t,c,829+(i*11),820+(i*9),806+(i*6),796+(i*3),2023+(i%2));}
ps("\nRhamnus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhm_market(t,c,823+(i*9),814+(i*7),801+(i*5),793+(i*3),2024);}
ps("\n");rhm_report();rhm_state();ps("\n=== Demo Complete ===\n");return 0;}
