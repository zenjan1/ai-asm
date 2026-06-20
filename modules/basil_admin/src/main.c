/* basil_admin: Basil management technology administration (v1.0)
 * Basil planning, basil execution, basil evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bas_t;
typedef struct{int n_bp,n_be,n_bv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bas_state_t;
static bas_t bps[N],bes[N-2],bvs[N-4],acs[N-6],mks[N-6]; static bas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bas_init(void){if(init)return -1;st.n_bp=0;st.n_be=0;st.n_bv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bps[i].active=0;for(int i=0;i<N-2;i++)bes[i].active=0;for(int i=0;i<N-4;i++)bvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BAS] Basil initialized\n");return 0;}
int bas_planning(int t,int c,int a,int b,int d,int e,int y){return add(bps,&st.n_bp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bas_execution(int t,int c,int a,int b,int d,int e,int y){return add(bes,&st.n_be,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bvs,&st.n_bv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bas_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bas_report(void){ps("[BAS] Bp: ");pi(st.n_bp);ps(" PCS=");pi(st.t_f1);ps("\nBe: ");pi(st.n_be);ps(" PCS=");pi(st.t_f2);ps("\nBv: ");pi(st.n_bv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bas_state(void){ps("[BAS] Bp=");pi(st.n_bp);ps(" Be=");pi(st.n_be);ps(" Bv=");pi(st.n_bv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Basil Admin Demo ===\n\n");bas_init();
ps("Basil planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bas_planning(t,c,538+(i*17),527+(i*14),507+(i*10),489+(i*6),2020+(i%5));}
ps("\nBasil execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bas_execution(t,c,527+(i*15),516+(i*12),498+(i*8),485+(i*5),2021+(i%4));}
ps("\nBasil evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bas_evaluation(t,c,519+(i*13),508+(i*10),492+(i*7),481+(i*4),2022+(i%3));}
ps("\nBasil accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bas_accessory(t,c,511+(i*11),502+(i*9),488+(i*6),478+(i*3),2023+(i%2));}
ps("\nBasil marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bas_market(t,c,505+(i*9),496+(i*7),483+(i*5),475+(i*3),2024);}
ps("\n");bas_report();bas_state();ps("\n=== Demo Complete ===\n");return 0;}
