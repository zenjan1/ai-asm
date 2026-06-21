/* halesia_admin: Halesia management technology administration (v1.0)
 * Halesia planning, halesia execution, halesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hal_t;
typedef struct{int n_halp,n_hale,n_hal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hal_state_t;
static hal_t halps[N],halss[N-2],halvss[N-4],halas[N-6],halmks[N-6]; static hal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hal_init(void){if(init)return -1;st.n_halp=0;st.n_hale=0;st.n_hal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)halps[i].active=0;for(int i=0;i<N-2;i++)halss[i].active=0;for(int i=0;i<N-4;i++)halvss[i].active=0;for(int i=0;i<N-6;i++)halas[i].active=0;for(int i=0;i<N-6;i++)halmks[i].active=0;init=1;ps("[HAL] Halesia initialized\n");return 0;}
int hal_planning(int t,int c,int a,int b,int d,int e,int y){return add(halps,&st.n_halp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hal_execution(int t,int c,int a,int b,int d,int e,int y){return add(halss,&st.n_hale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(halvss,&st.n_hal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(halas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hal_market(int t,int c,int a,int b,int d,int e,int y){return add(halmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hal_report(void){ps("[HAL] Halp: ");pi(st.n_halp);ps(" PCS=");pi(st.t_f1);ps("\nHale: ");pi(st.n_hale);ps(" PCS=");pi(st.t_f2);ps("\nHalv: ");pi(st.n_hal2);ps(" PCS=");pi(st.t_f3);ps("\nHalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hal_state(void){ps("[HAL] Halp=");pi(st.n_halp);ps(" Hale=");pi(st.n_hale);ps(" Halv=");pi(st.n_hal2);ps(" Halac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Halesia Admin Demo ===\n\n");hal_init();
ps("Halesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hal_planning(t,c,1018+(i*17),1007+(i*14),987+(i*10),969+(i*6),2020+(i%5));}
ps("\nHalesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hal_execution(t,c,1007+(i*15),996+(i*12),978+(i*8),965+(i*5),2021+(i%4));}
ps("\nHalesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hal_evaluation(t,c,999+(i*13),988+(i*10),972+(i*7),961+(i*4),2022+(i%3));}
ps("\nHalesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hal_accessory(t,c,991+(i*11),982+(i*9),968+(i*6),958+(i*3),2023+(i%2));}
ps("\nHalesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hal_market(t,c,985+(i*9),976+(i*7),963+(i*5),955+(i*3),2024);}
ps("\n");hal_report();hal_state();ps("\n=== Demo Complete ===\n");return 0;}
