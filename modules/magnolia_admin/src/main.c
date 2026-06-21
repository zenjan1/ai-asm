/* magnolia_admin: Magnolia management technology administration (v1.0)
 * Magnolia planning, magnolia execution, magnolia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mgn_t;
typedef struct{int n_mgp,n_mge,n_mgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mgn_state_t;
static mgn_t mgps[N],mgss[N-2],mgvss[N-4],mgas[N-6],mgmks[N-6]; static mgn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mgn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mgn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MGN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mgn_init(void){if(init)return -1;st.n_mgp=0;st.n_mge=0;st.n_mgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mgps[i].active=0;for(int i=0;i<N-2;i++)mgss[i].active=0;for(int i=0;i<N-4;i++)mgvss[i].active=0;for(int i=0;i<N-6;i++)mgas[i].active=0;for(int i=0;i<N-6;i++)mgmks[i].active=0;init=1;ps("[MGN] Magnolia initialized\n");return 0;}
int mgn_planning(int t,int c,int a,int b,int d,int e,int y){return add(mgps,&st.n_mgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mgn_execution(int t,int c,int a,int b,int d,int e,int y){return add(mgss,&st.n_mge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mgn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mgvss,&st.n_mgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mgn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mgn_market(int t,int c,int a,int b,int d,int e,int y){return add(mgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mgn_report(void){ps("[MGN] Mgp: ");pi(st.n_mgp);ps(" PCS=");pi(st.t_f1);ps("\nMge: ");pi(st.n_mge);ps(" PCS=");pi(st.t_f2);ps("\nMgv: ");pi(st.n_mgv);ps(" PCS=");pi(st.t_f3);ps("\nMgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mgn_state(void){ps("[MGN] Mgp=");pi(st.n_mgp);ps(" Mge=");pi(st.n_mge);ps(" Mgv=");pi(st.n_mgv);ps(" Mgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Magnolia Admin Demo ===\n\n");mgn_init();
ps("Magnolia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mgn_planning(t,c,792+(i*17),781+(i*14),761+(i*10),743+(i*6),2020+(i%5));}
ps("\nMagnolia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mgn_execution(t,c,781+(i*15),770+(i*12),752+(i*8),739+(i*5),2021+(i%4));}
ps("\nMagnolia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mgn_evaluation(t,c,773+(i*13),762+(i*10),748+(i*7),737+(i*4),2022+(i%3));}
ps("\nMagnolia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mgn_accessory(t,c,765+(i*11),756+(i*9),742+(i*6),732+(i*3),2023+(i%2));}
ps("\nMagnolia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mgn_market(t,c,759+(i*9),750+(i*7),737+(i*5),729+(i*3),2024);}
ps("\n");mgn_report();mgn_state();ps("\n=== Demo Complete ===\n");return 0;}
