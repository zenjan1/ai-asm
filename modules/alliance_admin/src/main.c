/* alliance_admin: Alliance management technology administration (v1.0)
 * Alliance formation, alliance operations, alliance evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alx_t;
typedef struct{int n_af,n_ao,n_ae,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alx_state_t;
static alx_t afs[N],aos[N-2],aes[N-4],acs[N-6],mks[N-6]; static alx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alx_init(void){if(init)return -1;st.n_af=0;st.n_ao=0;st.n_ae=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)afs[i].active=0;for(int i=0;i<N-2;i++)aos[i].active=0;for(int i=0;i<N-4;i++)aes[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ALX] Alliance initialized\n");return 0;}
int alx_formation(int t,int c,int a,int b,int d,int e,int y){return add(afs,&st.n_af,&st.t_f1,N,t,c,a,b,d,e,y);}
int alx_operations(int t,int c,int a,int b,int d,int e,int y){return add(aos,&st.n_ao,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aes,&st.n_ae,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alx_report(void){ps("[ALX] Af: ");pi(st.n_af);ps(" PCS=");pi(st.t_f1);ps("\nAo: ");pi(st.n_ao);ps(" PCS=");pi(st.t_f2);ps("\nAe: ");pi(st.n_ae);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alx_state(void){ps("[ALX] Af=");pi(st.n_af);ps(" Ao=");pi(st.n_ao);ps(" Ae=");pi(st.n_ae);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alliance Admin Demo ===\n\n");alx_init();
ps("Alliance formation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alx_formation(t,c,470+(i*17),459+(i*14),439+(i*10),421+(i*6),2020+(i%5));}
ps("\nAlliance operations...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alx_operations(t,c,459+(i*15),448+(i*12),430+(i*8),417+(i*5),2021+(i%4));}
ps("\nAlliance evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alx_evaluation(t,c,451+(i*13),440+(i*10),424+(i*7),413+(i*4),2022+(i%3));}
ps("\nAlliance accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alx_accessory(t,c,443+(i*11),434+(i*9),420+(i*6),410+(i*3),2023+(i%2));}
ps("\nAlliance marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alx_market(t,c,437+(i*9),428+(i*7),415+(i*5),407+(i*3),2024);}
ps("\n");alx_report();alx_state();ps("\n=== Demo Complete ===\n");return 0;}
