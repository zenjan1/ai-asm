/* bluebell_admin: Bluebell management technology administration (v1.0)
 * Bluebell planning, bluebell execution, bluebell evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blu_t;
typedef struct{int n_blp,n_ble,n_blv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blu_state_t;
static blu_t blps[N],bles[N-2],blvs[N-4],acs[N-6],mks[N-6]; static blu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blu_init(void){if(init)return -1;st.n_blp=0;st.n_ble=0;st.n_blv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)blps[i].active=0;for(int i=0;i<N-2;i++)bles[i].active=0;for(int i=0;i<N-4;i++)blvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLU] Bluebell initialized\n");return 0;}
int blu_planning(int t,int c,int a,int b,int d,int e,int y){return add(blps,&st.n_blp,&st.t_f1,N,t,c,a,b,d,e,y);}
int blu_execution(int t,int c,int a,int b,int d,int e,int y){return add(bles,&st.n_ble,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(blvs,&st.n_blv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blu_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blu_report(void){ps("[BLU] Blp: ");pi(st.n_blp);ps(" PCS=");pi(st.t_f1);ps("\nBle: ");pi(st.n_ble);ps(" PCS=");pi(st.t_f2);ps("\nBlv: ");pi(st.n_blv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blu_state(void){ps("[BLU] Blp=");pi(st.n_blp);ps(" Ble=");pi(st.n_ble);ps(" Blv=");pi(st.n_blv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bluebell Admin Demo ===\n\n");blu_init();
ps("Bluebell planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blu_planning(t,c,586+(i*17),575+(i*14),555+(i*10),537+(i*6),2020+(i%5));}
ps("\nBluebell execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blu_execution(t,c,575+(i*15),564+(i*12),546+(i*8),533+(i*5),2021+(i%4));}
ps("\nBluebell evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blu_evaluation(t,c,567+(i*13),556+(i*10),540+(i*7),529+(i*4),2022+(i%3));}
ps("\nBluebell accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blu_accessory(t,c,559+(i*11),550+(i*9),536+(i*6),526+(i*3),2023+(i%2));}
ps("\nBluebell marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blu_market(t,c,553+(i*9),544+(i*7),531+(i*5),523+(i*3),2024);}
ps("\n");blu_report();blu_state();ps("\n=== Demo Complete ===\n");return 0;}
