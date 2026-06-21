/* turmeric_admin: Turmeric management technology administration (v1.0)
 * Turmeric planning, turmeric execution, turmeric evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tm_t;
typedef struct{int n_tmp,n_tme,n_tmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tm_state_t;
static tm_t tmps[N],tmss[N-2],tmvss[N-4],tmas[N-6],tmmks[N-6]; static tm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tm_init(void){if(init)return -1;st.n_tmp=0;st.n_tme=0;st.n_tmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tmps[i].active=0;for(int i=0;i<N-2;i++)tmss[i].active=0;for(int i=0;i<N-4;i++)tmvss[i].active=0;for(int i=0;i<N-6;i++)tmas[i].active=0;for(int i=0;i<N-6;i++)tmmks[i].active=0;init=1;ps("[TRM] Turmeric initialized\n");return 0;}
int tm_planning(int t,int c,int a,int b,int d,int e,int y){return add(tmps,&st.n_tmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tm_execution(int t,int c,int a,int b,int d,int e,int y){return add(tmss,&st.n_tme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tmvss,&st.n_tmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tm_market(int t,int c,int a,int b,int d,int e,int y){return add(tmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tm_report(void){ps("[TRM] Tmp: ");pi(st.n_tmp);ps(" PCS=");pi(st.t_f1);ps("\nTme: ");pi(st.n_tme);ps(" PCS=");pi(st.t_f2);ps("\nTmv: ");pi(st.n_tmv);ps(" PCS=");pi(st.t_f3);ps("\nTmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tm_state(void){ps("[TRM] Tmp=");pi(st.n_tmp);ps(" Tme=");pi(st.n_tme);ps(" Tmv=");pi(st.n_tmv);ps(" Tmc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Turmeric Admin Demo ===\n\n");tm_init();
ps("Turmeric planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tm_planning(t,c,676+(i*17),665+(i*14),645+(i*10),627+(i*6),2020+(i%5));}
ps("\nTurmeric execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tm_execution(t,c,665+(i*15),654+(i*12),636+(i*8),623+(i*5),2021+(i%4));}
ps("\nTurmeric evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tm_evaluation(t,c,657+(i*13),646+(i*10),630+(i*7),619+(i*4),2022+(i%3));}
ps("\nTurmeric accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tm_accessory(t,c,649+(i*11),640+(i*9),626+(i*6),616+(i*3),2023+(i%2));}
ps("\nTurmeric marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tm_market(t,c,643+(i*9),634+(i*7),621+(i*5),613+(i*3),2024);}
ps("\n");tm_report();tm_state();ps("\n=== Demo Complete ===\n");return 0;}
