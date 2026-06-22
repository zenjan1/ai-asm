/* cyrilla_admin: Cyrilla management technology administration (v1.0)
 * Cyrilla planning, cyrilla execution, cyrilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ril_t;
typedef struct{int n_rilp,n_rile,n_ril2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ril_state_t;
static ril_t rilps[N],rilss[N-2],rilvss[N-4],rilas[N-6],rilmks[N-6]; static ril_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ril_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ril_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ril_init(void){if(init)return -1;st.n_rilp=0;st.n_rile=0;st.n_ril2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rilps[i].active=0;for(int i=0;i<N-2;i++)rilss[i].active=0;for(int i=0;i<N-4;i++)rilvss[i].active=0;for(int i=0;i<N-6;i++)rilas[i].active=0;for(int i=0;i<N-6;i++)rilmks[i].active=0;init=1;ps("[RIL] Cyrilla initialized\n");return 0;}
int ril_planning(int t,int c,int a,int b,int d,int e,int y){return add(rilps,&st.n_rilp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ril_execution(int t,int c,int a,int b,int d,int e,int y){return add(rilss,&st.n_rile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ril_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rilvss,&st.n_ril2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ril_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rilas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ril_market(int t,int c,int a,int b,int d,int e,int y){return add(rilmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ril_report(void){ps("[RIL] Cyrpp: ");pi(st.n_rilp);ps(" PCS=");pi(st.t_f1);ps("\nCyrpe: ");pi(st.n_rile);ps(" PCS=");pi(st.t_f2);ps("\nCyrv: ");pi(st.n_ril2);ps(" PCS=");pi(st.t_f3);ps("\nCyrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ril_state(void){ps("[RIL] Cyrpp=");pi(st.n_rilp);ps(" Cyrpe=");pi(st.n_rile);ps(" Cyrv=");pi(st.n_ril2);ps(" Cyrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyrilla Admin Demo ===\n\n");ril_init();
ps("Cyrilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ril_planning(t,c,1253+(i*17),1242+(i*14),1222+(i*10),1204+(i*6),2020+(i%5));}
ps("\nCyrilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ril_execution(t,c,1242+(i*15),1231+(i*12),1213+(i*8),1200+(i*5),2021+(i%4));}
ps("\nCyrilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ril_evaluation(t,c,1234+(i*13),1223+(i*10),1207+(i*7),1196+(i*4),2022+(i%3));}
ps("\nCyrilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ril_accessory(t,c,1226+(i*11),1217+(i*9),1203+(i*6),1193+(i*3),2023+(i%2));}
ps("\nCyrilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ril_market(t,c,1220+(i*9),1211+(i*7),1198+(i*5),1190+(i*3),2024);}
ps("\n");ril_report();ril_state();ps("\n=== Demo Complete ===\n");return 0;}
