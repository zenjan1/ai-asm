/* controltech_admin: Automatic control technology administration (v1.0)
 * Process control, motion control, logic control, control elements, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctl_t;
typedef struct{int n_pc,n_mc,n_lc,n_ce,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctl_state_t;
static ctl_t pcs[N],mcs[N-2],lcs[N-4],ces[N-6],mks[N-6]; static ctl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctl_init(void){if(init)return -1;st.n_pc=0;st.n_mc=0;st.n_lc=0;st.n_ce=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pcs[i].active=0;for(int i=0;i<N-2;i++)mcs[i].active=0;for(int i=0;i<N-4;i++)lcs[i].active=0;for(int i=0;i<N-6;i++)ces[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTL] Controltech initialized\n");return 0;}
int ctl_process(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctl_motion(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctl_logic(int t,int c,int a,int b,int d,int e,int y){return add(lcs,&st.n_lc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctl_element(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctl_report(void){ps("[CTL] Pc: ");pi(st.n_pc);ps(" PCS=");pi(st.t_f1);ps("\nMc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f2);ps("\nLc: ");pi(st.n_lc);ps(" PCS=");pi(st.t_f3);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctl_state(void){ps("[CTL] Pc=");pi(st.n_pc);ps(" Mc=");pi(st.n_mc);ps(" Lc=");pi(st.n_lc);ps(" Ce=");pi(st.n_ce);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Control Tech Admin Demo ===\n\n");ctl_init();
ps("Process control...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctl_process(t,c,297+(i*17),282+(i*14),262+(i*10),244+(i*6),2020+(i%5));}
ps("\nMotion control...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctl_motion(t,c,286+(i*15),272+(i*12),254+(i*8),241+(i*5),2021+(i%4));}
ps("\nLogic control...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctl_logic(t,c,278+(i*13),264+(i*10),248+(i*7),237+(i*4),2022+(i%3));}
ps("\nControl elements...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctl_element(t,c,270+(i*11),258+(i*9),244+(i*6),234+(i*3),2023+(i%2));}
ps("\nControl marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctl_market(t,c,264+(i*9),253+(i*7),240+(i*5),232+(i*3),2024);}
ps("\n");ctl_report();ctl_state();ps("\n=== Demo Complete ===\n");return 0;}
