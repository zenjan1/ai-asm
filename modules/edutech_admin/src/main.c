/* edutech_admin: Education technology administration (v1.0)
 * Online education, smart classrooms, educational content, training platforms, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} edt_t;
typedef struct{int n_oe,n_sc,n_ec,n_tp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} edt_state_t;
static edt_t oes[N],scs[N-2],ecs[N-4],tps[N-6],mks[N-6]; static edt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(edt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;edt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int edt_init(void){if(init)return -1;st.n_oe=0;st.n_sc=0;st.n_ec=0;st.n_tp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oes[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)ecs[i].active=0;for(int i=0;i<N-6;i++)tps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EDT] Edutech initialized\n");return 0;}
int edt_online(int t,int c,int a,int b,int d,int e,int y){return add(oes,&st.n_oe,&st.t_f1,N,t,c,a,b,d,e,y);}
int edt_classroom(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int edt_content(int t,int c,int a,int b,int d,int e,int y){return add(ecs,&st.n_ec,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int edt_training(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int edt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void edt_report(void){ps("[EDT] Oe: ");pi(st.n_oe);ps(" PCS=");pi(st.t_f1);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f2);ps("\nEc: ");pi(st.n_ec);ps(" PCS=");pi(st.t_f3);ps("\nTp: ");pi(st.n_tp);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void edt_state(void){ps("[EDT] Oe=");pi(st.n_oe);ps(" Sc=");pi(st.n_sc);ps(" Ec=");pi(st.n_ec);ps(" Tp=");pi(st.n_tp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Education Tech Admin Demo ===\n\n");edt_init();
ps("Online education...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;edt_online(t,c,237+(i*17),222+(i*14),202+(i*10),184+(i*6),2020+(i%5));}
ps("\nSmart classrooms...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;edt_classroom(t,c,226+(i*15),212+(i*12),194+(i*8),181+(i*5),2021+(i%4));}
ps("\nEducational content...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;edt_content(t,c,218+(i*13),204+(i*10),188+(i*7),177+(i*4),2022+(i%3));}
ps("\nTraining platforms...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edt_training(t,c,210+(i*11),198+(i*9),184+(i*6),174+(i*3),2023+(i%2));}
ps("\nEducation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edt_market(t,c,204+(i*9),193+(i*7),180+(i*5),172+(i*3),2024);}
ps("\n");edt_report();edt_state();ps("\n=== Demo Complete ===\n");return 0;}
