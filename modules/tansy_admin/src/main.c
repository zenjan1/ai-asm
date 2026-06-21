/* tansy_admin: Tansy management technology administration (v1.0)
 * Tansy planning, tansy execution, tansy evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tn_t;
typedef struct{int n_tnp,n_tne,n_tnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tn_state_t;
static tn_t tnps[N],tnss[N-2],tnvss[N-4],tnas[N-6],tnmks[N-6]; static tn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TNS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tn_init(void){if(init)return -1;st.n_tnp=0;st.n_tne=0;st.n_tnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tnps[i].active=0;for(int i=0;i<N-2;i++)tnss[i].active=0;for(int i=0;i<N-4;i++)tnvss[i].active=0;for(int i=0;i<N-6;i++)tnas[i].active=0;for(int i=0;i<N-6;i++)tnmks[i].active=0;init=1;ps("[TNS] Tansy initialized\n");return 0;}
int tn_planning(int t,int c,int a,int b,int d,int e,int y){return add(tnps,&st.n_tnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tn_execution(int t,int c,int a,int b,int d,int e,int y){return add(tnss,&st.n_tne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tnvss,&st.n_tnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tn_market(int t,int c,int a,int b,int d,int e,int y){return add(tnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tn_report(void){ps("[TNS] Tnp: ");pi(st.n_tnp);ps(" PCS=");pi(st.t_f1);ps("\nTne: ");pi(st.n_tne);ps(" PCS=");pi(st.t_f2);ps("\nTnv: ");pi(st.n_tnv);ps(" PCS=");pi(st.t_f3);ps("\nTnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tn_state(void){ps("[TNS] Tnp=");pi(st.n_tnp);ps(" Tne=");pi(st.n_tne);ps(" Tnv=");pi(st.n_tnv);ps(" Tnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tansy Admin Demo ===\n\n");tn_init();
ps("Tansy planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tn_planning(t,c,683+(i*17),672+(i*14),652+(i*10),634+(i*6),2020+(i%5));}
ps("\nTansy execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tn_execution(t,c,672+(i*15),661+(i*12),643+(i*8),630+(i*5),2021+(i%4));}
ps("\nTansy evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tn_evaluation(t,c,664+(i*13),653+(i*10),637+(i*7),626+(i*4),2022+(i%3));}
ps("\nTansy accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tn_accessory(t,c,656+(i*11),647+(i*9),633+(i*6),623+(i*3),2023+(i%2));}
ps("\nTansy marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tn_market(t,c,650+(i*9),641+(i*7),628+(i*5),620+(i*3),2024);}
ps("\n");tn_report();tn_state();ps("\n=== Demo Complete ===\n");return 0;}
