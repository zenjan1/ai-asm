/* armeria_admin: Armeria management technology administration (v1.0)
 * Armeria planning, armeria execution, armeria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arm_t;
typedef struct{int n_amp,n_ame,n_amv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arm_state_t;
static arm_t armps[N],armss[N-2],armvss[N-4],armas[N-6],armmks[N-6]; static arm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arm_init(void){if(init)return -1;st.n_amp=0;st.n_ame=0;st.n_amv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)armps[i].active=0;for(int i=0;i<N-2;i++)armss[i].active=0;for(int i=0;i<N-4;i++)armvss[i].active=0;for(int i=0;i<N-6;i++)armas[i].active=0;for(int i=0;i<N-6;i++)armmks[i].active=0;init=1;ps("[ARM] Armeria initialized\n");return 0;}
int arm_planning(int t,int c,int a,int b,int d,int e,int y){return add(armps,&st.n_amp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arm_execution(int t,int c,int a,int b,int d,int e,int y){return add(armss,&st.n_ame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(armvss,&st.n_amv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(armas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arm_market(int t,int c,int a,int b,int d,int e,int y){return add(armmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arm_report(void){ps("[ARM] Amp: ");pi(st.n_amp);ps(" PCS=");pi(st.t_f1);ps("\nAme: ");pi(st.n_ame);ps(" PCS=");pi(st.t_f2);ps("\nAmv: ");pi(st.n_amv);ps(" PCS=");pi(st.t_f3);ps("\nAmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arm_state(void){ps("[ARM] Amp=");pi(st.n_amp);ps(" Ame=");pi(st.n_ame);ps(" Amv=");pi(st.n_amv);ps(" Amc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Armeria Admin Demo ===\n\n");arm_init();
ps("Armeria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arm_planning(t,c,885+(i*17),874+(i*14),854+(i*10),836+(i*6),2020+(i%5));}
ps("\nArmeria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arm_execution(t,c,874+(i*15),863+(i*12),845+(i*8),832+(i*5),2021+(i%4));}
ps("\nArmeria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arm_evaluation(t,c,866+(i*13),855+(i*10),839+(i*7),828+(i*4),2022+(i%3));}
ps("\nArmeria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arm_accessory(t,c,858+(i*11),849+(i*9),835+(i*6),825+(i*3),2023+(i%2));}
ps("\nArmeria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arm_market(t,c,852+(i*9),843+(i*7),830+(i*5),822+(i*3),2024);}
ps("\n");arm_report();arm_state();ps("\n=== Demo Complete ===\n");return 0;}
