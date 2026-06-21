/* rosemary_admin: Rosemary management technology administration (v1.0)
 * Rosemary planning, rosemary execution, rosemary evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rs_t;
typedef struct{int n_rsp,n_rse,n_rsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rs_state_t;
static rs_t rsps[N],rsss[N-2],rsvss[N-4],rsas[N-6],rsmks[N-6]; static rs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RSM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rs_init(void){if(init)return -1;st.n_rsp=0;st.n_rse=0;st.n_rsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rsps[i].active=0;for(int i=0;i<N-2;i++)rsss[i].active=0;for(int i=0;i<N-4;i++)rsvss[i].active=0;for(int i=0;i<N-6;i++)rsas[i].active=0;for(int i=0;i<N-6;i++)rsmks[i].active=0;init=1;ps("[RSM] Rosemary initialized\n");return 0;}
int rs_planning(int t,int c,int a,int b,int d,int e,int y){return add(rsps,&st.n_rsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rs_execution(int t,int c,int a,int b,int d,int e,int y){return add(rsss,&st.n_rse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rs_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rsvss,&st.n_rsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rs_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rs_market(int t,int c,int a,int b,int d,int e,int y){return add(rsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rs_report(void){ps("[RSM] Rsp: ");pi(st.n_rsp);ps(" PCS=");pi(st.t_f1);ps("\nRse: ");pi(st.n_rse);ps(" PCS=");pi(st.t_f2);ps("\nRsv: ");pi(st.n_rsv);ps(" PCS=");pi(st.t_f3);ps("\nRsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rs_state(void){ps("[RSM] Rsp=");pi(st.n_rsp);ps(" Rse=");pi(st.n_rse);ps(" Rsv=");pi(st.n_rsv);ps(" Rsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rosemary Admin Demo ===\n\n");rs_init();
ps("Rosemary planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rs_planning(t,c,652+(i*17),641+(i*14),621+(i*10),603+(i*6),2020+(i%5));}
ps("\nRosemary execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rs_execution(t,c,641+(i*15),630+(i*12),612+(i*8),599+(i*5),2021+(i%4));}
ps("\nRosemary evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rs_evaluation(t,c,633+(i*13),622+(i*10),606+(i*7),595+(i*4),2022+(i%3));}
ps("\nRosemary accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rs_accessory(t,c,625+(i*11),616+(i*9),602+(i*6),592+(i*3),2023+(i%2));}
ps("\nRosemary marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rs_market(t,c,619+(i*9),610+(i*7),597+(i*5),589+(i*3),2024);}
ps("\n");rs_report();rs_state();ps("\n=== Demo Complete ===\n");return 0;}
