/* gymnocladus_admin: Gymnocladus management technology administration (v1.0)
 * Gymnocladus planning, gymnocladus execution, gymnocladus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gym_t;
typedef struct{int n_gymp,n_gyme,n_gym2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gym_state_t;
static gym_t gymps[N],gymss[N-2],gymvss[N-4],gymas[N-6],gymmks[N-6]; static gym_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gym_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gym_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GYM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gym_init(void){if(init)return -1;st.n_gymp=0;st.n_gyme=0;st.n_gym2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gymps[i].active=0;for(int i=0;i<N-2;i++)gymss[i].active=0;for(int i=0;i<N-4;i++)gymvss[i].active=0;for(int i=0;i<N-6;i++)gymas[i].active=0;for(int i=0;i<N-6;i++)gymmks[i].active=0;init=1;ps("[GYM] Gymnocladus initialized\n");return 0;}
int gym_planning(int t,int c,int a,int b,int d,int e,int y){return add(gymps,&st.n_gymp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gym_execution(int t,int c,int a,int b,int d,int e,int y){return add(gymss,&st.n_gyme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gym_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gymvss,&st.n_gym2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gym_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gymas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gym_market(int t,int c,int a,int b,int d,int e,int y){return add(gymmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gym_report(void){ps("[GYM] Gympp: ");pi(st.n_gymp);ps(" PCS=");pi(st.t_f1);ps("\nGyme: ");pi(st.n_gyme);ps(" PCS=");pi(st.t_f2);ps("\nGymv: ");pi(st.n_gym2);ps(" PCS=");pi(st.t_f3);ps("\nGymc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gym_state(void){ps("[GYM] Gympp=");pi(st.n_gymp);ps(" Gyme=");pi(st.n_gyme);ps(" Gymv=");pi(st.n_gym2);ps(" Gymc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gymnocladus Admin Demo ===\n\n");gym_init();
ps("Gymnocladus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gym_planning(t,c,1147+(i*17),1136+(i*14),1116+(i*10),1098+(i*6),2020+(i%5));}
ps("\nGymnocladus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gym_execution(t,c,1136+(i*15),1125+(i*12),1107+(i*8),1094+(i*5),2021+(i%4));}
ps("\nGymnocladus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gym_evaluation(t,c,1128+(i*13),1117+(i*10),1101+(i*7),1090+(i*4),2022+(i%3));}
ps("\nGymnocladus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gym_accessory(t,c,1120+(i*11),1111+(i*9),1097+(i*6),1087+(i*3),2023+(i%2));}
ps("\nGymnocladus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gym_market(t,c,1114+(i*9),1105+(i*7),1092+(i*5),1084+(i*3),2024);}
ps("\n");gym_report();gym_state();ps("\n=== Demo Complete ===\n");return 0;}
