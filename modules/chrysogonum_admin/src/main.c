/* chrysogonum_admin: Chrysogonum management technology administration (v1.0)
 * Chrysogonum planning, chrysogonum execution, chrysogonum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chg_t;
typedef struct{int n_chgp,n_chge,n_chg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chg_state_t;
static chg_t chgps[N],chgss[N-2],chgvss[N-4],chgas[N-6],chgmks[N-6]; static chg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chg_init(void){if(init)return -1;st.n_chgp=0;st.n_chge=0;st.n_chg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chgps[i].active=0;for(int i=0;i<N-2;i++)chgss[i].active=0;for(int i=0;i<N-4;i++)chgvss[i].active=0;for(int i=0;i<N-6;i++)chgas[i].active=0;for(int i=0;i<N-6;i++)chgmks[i].active=0;init=1;ps("[CHG] Chrysogonum initialized\n");return 0;}
int chg_planning(int t,int c,int a,int b,int d,int e,int y){return add(chgps,&st.n_chgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int chg_execution(int t,int c,int a,int b,int d,int e,int y){return add(chgss,&st.n_chge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chgvss,&st.n_chg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chg_market(int t,int c,int a,int b,int d,int e,int y){return add(chgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chg_report(void){ps("[CHG] Chrpp: ");pi(st.n_chgp);ps(" PCS=");pi(st.t_f1);ps("\nChre: ");pi(st.n_chge);ps(" PCS=");pi(st.t_f2);ps("\nChrv: ");pi(st.n_chg2);ps(" PCS=");pi(st.t_f3);ps("\nChrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chg_state(void){ps("[CHG] Chrpp=");pi(st.n_chgp);ps(" Chre=");pi(st.n_chge);ps(" Chrv=");pi(st.n_chg2);ps(" Chrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chrysogonum Admin Demo ===\n\n");chg_init();
ps("Chrysogonum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chg_planning(t,c,1128+(i*17),1117+(i*14),1097+(i*10),1079+(i*6),2020+(i%5));}
ps("\nChrysogonum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chg_execution(t,c,1117+(i*15),1106+(i*12),1088+(i*8),1075+(i*5),2021+(i%4));}
ps("\nChrysogonum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chg_evaluation(t,c,1109+(i*13),1098+(i*10),1082+(i*7),1071+(i*4),2022+(i%3));}
ps("\nChrysogonum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chg_accessory(t,c,1101+(i*11),1092+(i*9),1078+(i*6),1068+(i*3),2023+(i%2));}
ps("\nChrysogonum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chg_market(t,c,1095+(i*9),1086+(i*7),1073+(i*5),1065+(i*3),2024);}
ps("\n");chg_report();chg_state();ps("\n=== Demo Complete ===\n");return 0;}
