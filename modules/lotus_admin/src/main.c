/* lotus_admin: Lotus management technology administration (v1.0)
 * Lotus planning, lotus execution, lotus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lot_t;
typedef struct{int n_lp,n_le,n_lv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lot_state_t;
static lot_t lps[N],les[N-2],lvs[N-4],acs[N-6],mks[N-6]; static lot_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lot_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lot_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lot_init(void){if(init)return -1;st.n_lp=0;st.n_le=0;st.n_lv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lps[i].active=0;for(int i=0;i<N-2;i++)les[i].active=0;for(int i=0;i<N-4;i++)lvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LOT] Lotus initialized\n");return 0;}
int lot_planning(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lot_execution(int t,int c,int a,int b,int d,int e,int y){return add(les,&st.n_le,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lot_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lvs,&st.n_lv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lot_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lot_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lot_report(void){ps("[LOT] Lp: ");pi(st.n_lp);ps(" PCS=");pi(st.t_f1);ps("\nLe: ");pi(st.n_le);ps(" PCS=");pi(st.t_f2);ps("\nLv: ");pi(st.n_lv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lot_state(void){ps("[LOT] Lp=");pi(st.n_lp);ps(" Le=");pi(st.n_le);ps(" Lv=");pi(st.n_lv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lotus Admin Demo ===\n\n");lot_init();
ps("Lotus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lot_planning(t,c,547+(i*17),536+(i*14),516+(i*10),498+(i*6),2020+(i%5));}
ps("\nLotus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lot_execution(t,c,536+(i*15),525+(i*12),507+(i*8),494+(i*5),2021+(i%4));}
ps("\nLotus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lot_evaluation(t,c,528+(i*13),517+(i*10),501+(i*7),490+(i*4),2022+(i%3));}
ps("\nLotus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lot_accessory(t,c,520+(i*11),511+(i*9),497+(i*6),487+(i*3),2023+(i%2));}
ps("\nLotus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lot_market(t,c,514+(i*9),505+(i*7),492+(i*5),484+(i*3),2024);}
ps("\n");lot_report();lot_state();ps("\n=== Demo Complete ===\n");return 0;}
