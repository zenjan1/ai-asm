/* sweetpea_admin: Sweetpea management technology administration (v1.0)
 * Sweetpea planning, sweetpea execution, sweetpea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} swe_t;
typedef struct{int n_swp,n_swe,n_swv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} swe_state_t;
static swe_t swps[N],swes[N-2],swvs[N-4],acs[N-6],mks[N-6]; static swe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(swe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;swe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SWE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int swe_init(void){if(init)return -1;st.n_swp=0;st.n_swe=0;st.n_swv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)swps[i].active=0;for(int i=0;i<N-2;i++)swes[i].active=0;for(int i=0;i<N-4;i++)swvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SWE] Sweetpea initialized\n");return 0;}
int swe_planning(int t,int c,int a,int b,int d,int e,int y){return add(swps,&st.n_swp,&st.t_f1,N,t,c,a,b,d,e,y);}
int swe_execution(int t,int c,int a,int b,int d,int e,int y){return add(swes,&st.n_swe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int swe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(swvs,&st.n_swv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int swe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int swe_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void swe_report(void){ps("[SWE] Swp: ");pi(st.n_swp);ps(" PCS=");pi(st.t_f1);ps("\nSwe: ");pi(st.n_swe);ps(" PCS=");pi(st.t_f2);ps("\nSwv: ");pi(st.n_swv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void swe_state(void){ps("[SWE] Swp=");pi(st.n_swp);ps(" Swe=");pi(st.n_swe);ps(" Swv=");pi(st.n_swv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sweetpea Admin Demo ===\n\n");swe_init();
ps("Sweetpea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;swe_planning(t,c,579+(i*17),568+(i*14),548+(i*10),530+(i*6),2020+(i%5));}
ps("\nSweetpea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;swe_execution(t,c,568+(i*15),557+(i*12),539+(i*8),526+(i*5),2021+(i%4));}
ps("\nSweetpea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;swe_evaluation(t,c,560+(i*13),549+(i*10),533+(i*7),522+(i*4),2022+(i%3));}
ps("\nSweetpea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swe_accessory(t,c,552+(i*11),543+(i*9),529+(i*6),519+(i*3),2023+(i%2));}
ps("\nSweetpea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swe_market(t,c,546+(i*9),537+(i*7),524+(i*5),516+(i*3),2024);}
ps("\n");swe_report();swe_state();ps("\n=== Demo Complete ===\n");return 0;}
