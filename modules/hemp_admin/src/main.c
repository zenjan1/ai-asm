/* hemp_admin: Hemp management technology administration (v1.0)
 * Hemp planning, hemp execution, hemp evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hem_t;
typedef struct{int n_hp,n_he,n_hv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hem_state_t;
static hem_t hps[N],hes[N-2],hvs[N-4],acs[N-6],mks[N-6]; static hem_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hem_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hem_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HEM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hem_init(void){if(init)return -1;st.n_hp=0;st.n_he=0;st.n_hv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hps[i].active=0;for(int i=0;i<N-2;i++)hes[i].active=0;for(int i=0;i<N-4;i++)hvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HEM] Hemp initialized\n");return 0;}
int hem_planning(int t,int c,int a,int b,int d,int e,int y){return add(hps,&st.n_hp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hem_execution(int t,int c,int a,int b,int d,int e,int y){return add(hes,&st.n_he,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hem_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hvs,&st.n_hv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hem_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hem_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hem_report(void){ps("[HEM] Hp: ");pi(st.n_hp);ps(" PCS=");pi(st.t_f1);ps("\nHe: ");pi(st.n_he);ps(" PCS=");pi(st.t_f2);ps("\nHv: ");pi(st.n_hv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hem_state(void){ps("[HEM] Hp=");pi(st.n_hp);ps(" He=");pi(st.n_he);ps(" Hv=");pi(st.n_hv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hemp Admin Demo ===\n\n");hem_init();
ps("Hemp planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hem_planning(t,c,527+(i*17),516+(i*14),496+(i*10),478+(i*6),2020+(i%5));}
ps("\nHemp execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hem_execution(t,c,516+(i*15),505+(i*12),487+(i*8),474+(i*5),2021+(i%4));}
ps("\nHemp evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hem_evaluation(t,c,508+(i*13),497+(i*10),481+(i*7),470+(i*4),2022+(i%3));}
ps("\nHemp accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hem_accessory(t,c,500+(i*11),491+(i*9),477+(i*6),467+(i*3),2023+(i%2));}
ps("\nHemp marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hem_market(t,c,494+(i*9),485+(i*7),472+(i*5),464+(i*3),2024);}
ps("\n");hem_report();hem_state();ps("\n=== Demo Complete ===\n");return 0;}
