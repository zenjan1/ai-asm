/* cobaea_admin: Cobaea management technology administration (v1.0)
 * Cobaea planning, cobaea execution, cobaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cob_t;
typedef struct{int n_cobp,n_cobe,n_cob2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cob_state_t;
static cob_t cobps[N],cobss[N-2],cobvss[N-4],cobas[N-6],cobmks[N-6]; static cob_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cob_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cob_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cob_init(void){if(init)return -1;st.n_cobp=0;st.n_cobe=0;st.n_cob2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cobps[i].active=0;for(int i=0;i<N-2;i++)cobss[i].active=0;for(int i=0;i<N-4;i++)cobvss[i].active=0;for(int i=0;i<N-6;i++)cobas[i].active=0;for(int i=0;i<N-6;i++)cobmks[i].active=0;init=1;ps("[COB] Cobaea initialized\n");return 0;}
int cob_planning(int t,int c,int a,int b,int d,int e,int y){return add(cobps,&st.n_cobp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cob_execution(int t,int c,int a,int b,int d,int e,int y){return add(cobss,&st.n_cobe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cob_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cobvss,&st.n_cob2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cob_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cobas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cob_market(int t,int c,int a,int b,int d,int e,int y){return add(cobmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cob_report(void){ps("[COB] Cobbp: ");pi(st.n_cobp);ps(" PCS=");pi(st.t_f1);ps("\nCobe: ");pi(st.n_cobe);ps(" PCS=");pi(st.t_f2);ps("\nCobv: ");pi(st.n_cob2);ps(" PCS=");pi(st.t_f3);ps("\nCobc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cob_state(void){ps("[COB] Cobbp=");pi(st.n_cobp);ps(" Cobe=");pi(st.n_cobe);ps(" Cobv=");pi(st.n_cob2);ps(" Cobc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cobaea Admin Demo ===\n\n");cob_init();
ps("Cobaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cob_planning(t,c,986+(i*17),975+(i*14),955+(i*10),937+(i*6),2020+(i%5));}
ps("\nCobaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cob_execution(t,c,975+(i*15),964+(i*12),946+(i*8),933+(i*5),2021+(i%4));}
ps("\nCobaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cob_evaluation(t,c,967+(i*13),956+(i*10),940+(i*7),929+(i*4),2022+(i%3));}
ps("\nCobaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cob_accessory(t,c,959+(i*11),950+(i*9),936+(i*6),926+(i*3),2023+(i%2));}
ps("\nCobaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cob_market(t,c,953+(i*9),944+(i*7),931+(i*5),923+(i*3),2024);}
ps("\n");cob_report();cob_state();ps("\n=== Demo Complete ===\n");return 0;}
