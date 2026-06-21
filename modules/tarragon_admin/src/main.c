/* tarragon_admin: Tarragon management technology administration (v1.0)
 * Tarragon planning, tarragon execution, tarragon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tr_t;
typedef struct{int n_trp,n_tre,n_trv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tr_state_t;
static tr_t trps[N],trss[N-2],trvss[N-4],tras[N-6],trmks[N-6]; static tr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tr_init(void){if(init)return -1;st.n_trp=0;st.n_tre=0;st.n_trv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trps[i].active=0;for(int i=0;i<N-2;i++)trss[i].active=0;for(int i=0;i<N-4;i++)trvss[i].active=0;for(int i=0;i<N-6;i++)tras[i].active=0;for(int i=0;i<N-6;i++)trmks[i].active=0;init=1;ps("[TAR] Tarragon initialized\n");return 0;}
int tr_planning(int t,int c,int a,int b,int d,int e,int y){return add(trps,&st.n_trp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tr_execution(int t,int c,int a,int b,int d,int e,int y){return add(trss,&st.n_tre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(trvss,&st.n_trv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tr_market(int t,int c,int a,int b,int d,int e,int y){return add(trmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tr_report(void){ps("[TAR] Trp: ");pi(st.n_trp);ps(" PCS=");pi(st.t_f1);ps("\nTre: ");pi(st.n_tre);ps(" PCS=");pi(st.t_f2);ps("\nTrv: ");pi(st.n_trv);ps(" PCS=");pi(st.t_f3);ps("\nTrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tr_state(void){ps("[TAR] Trp=");pi(st.n_trp);ps(" Tre=");pi(st.n_tre);ps(" Trv=");pi(st.n_trv);ps(" Trc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tarragon Admin Demo ===\n\n");tr_init();
ps("Tarragon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tr_planning(t,c,660+(i*17),649+(i*14),629+(i*10),611+(i*6),2020+(i%5));}
ps("\nTarragon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tr_execution(t,c,649+(i*15),638+(i*12),620+(i*8),607+(i*5),2021+(i%4));}
ps("\nTarragon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tr_evaluation(t,c,641+(i*13),630+(i*10),614+(i*7),603+(i*4),2022+(i%3));}
ps("\nTarragon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tr_accessory(t,c,633+(i*11),624+(i*9),610+(i*6),600+(i*3),2023+(i%2));}
ps("\nTarragon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tr_market(t,c,627+(i*9),618+(i*7),605+(i*5),597+(i*3),2024);}
ps("\n");tr_report();tr_state();ps("\n=== Demo Complete ===\n");return 0;}
