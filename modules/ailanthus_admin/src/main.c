/* ailanthus_admin: Ailanthus management technology administration (v1.0)
 * Ailanthus planning, ailanthus execution, ailanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ail_t;
typedef struct{int n_ailp,n_aile,n_ail2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ail_state_t;
static ail_t ailps[N],ailss[N-2],ailvss[N-4],ailas[N-6],ailmks[N-6]; static ail_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ail_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ail_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ail_init(void){if(init)return -1;st.n_ailp=0;st.n_aile=0;st.n_ail2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ailps[i].active=0;for(int i=0;i<N-2;i++)ailss[i].active=0;for(int i=0;i<N-4;i++)ailvss[i].active=0;for(int i=0;i<N-6;i++)ailas[i].active=0;for(int i=0;i<N-6;i++)ailmks[i].active=0;init=1;ps("[AIL] Ailanthus initialized\n");return 0;}
int ail_planning(int t,int c,int a,int b,int d,int e,int y){return add(ailps,&st.n_ailp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ail_execution(int t,int c,int a,int b,int d,int e,int y){return add(ailss,&st.n_aile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ail_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ailvss,&st.n_ail2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ail_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ailas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ail_market(int t,int c,int a,int b,int d,int e,int y){return add(ailmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ail_report(void){ps("[AIL] Ailp: ");pi(st.n_ailp);ps(" PCS=");pi(st.t_f1);ps("\nAile: ");pi(st.n_aile);ps(" PCS=");pi(st.t_f2);ps("\nAilv: ");pi(st.n_ail2);ps(" PCS=");pi(st.t_f3);ps("\nAic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ail_state(void){ps("[AIL] Ailp=");pi(st.n_ailp);ps(" Aile=");pi(st.n_aile);ps(" Ailv=");pi(st.n_ail2);ps(" Aic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ailanthus Admin Demo ===\n\n");ail_init();
ps("Ailanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ail_planning(t,c,891+(i*17),880+(i*14),860+(i*10),842+(i*6),2020+(i%5));}
ps("\nAilanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ail_execution(t,c,880+(i*15),869+(i*12),851+(i*8),838+(i*5),2021+(i%4));}
ps("\nAilanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ail_evaluation(t,c,872+(i*13),861+(i*10),845+(i*7),834+(i*4),2022+(i%3));}
ps("\nAilanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ail_accessory(t,c,864+(i*11),855+(i*9),841+(i*6),831+(i*3),2023+(i%2));}
ps("\nAilanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ail_market(t,c,858+(i*9),849+(i*7),836+(i*5),828+(i*3),2024);}
ps("\n");ail_report();ail_state();ps("\n=== Demo Complete ===\n");return 0;}
