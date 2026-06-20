/* musitech_admin: Musical instrument technology administration (v1.0)
 * Keyboard instruments, string instruments, wind instruments, percussion instruments, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} msc_t;
typedef struct{int n_kb,n_st,n_wd,n_pc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} msc_state_t;
static msc_t kbs[N],sts[N-2],wds[N-4],pccs[N-6],mks[N-6]; static msc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(msc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;msc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int msc_init(void){if(init)return -1;st.n_kb=0;st.n_st=0;st.n_wd=0;st.n_pc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kbs[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)wds[i].active=0;for(int i=0;i<N-6;i++)pccs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MSC] Musitech initialized\n");return 0;}
int msc_keyboard(int t,int c,int a,int b,int d,int e,int y){return add(kbs,&st.n_kb,&st.t_f1,N,t,c,a,b,d,e,y);}
int msc_string(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int msc_wind(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int msc_percussion(int t,int c,int a,int b,int d,int e,int y){return add(pccs,&st.n_pc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int msc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void msc_report(void){ps("[MSC] Kb: ");pi(st.n_kb);ps(" PCS=");pi(st.t_f1);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f2);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f3);ps("\nPc: ");pi(st.n_pc);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void msc_state(void){ps("[MSC] Kb=");pi(st.n_kb);ps(" St=");pi(st.n_st);ps(" Wd=");pi(st.n_wd);ps(" Pc=");pi(st.n_pc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Music Instrument Tech Admin Demo ===\n\n");msc_init();
ps("Keyboard instruments...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;msc_keyboard(t,c,234+(i*17),219+(i*14),199+(i*10),181+(i*6),2020+(i%5));}
ps("\nString instruments...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;msc_string(t,c,223+(i*15),209+(i*12),191+(i*8),178+(i*5),2021+(i%4));}
ps("\nWind instruments...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;msc_wind(t,c,215+(i*13),201+(i*10),185+(i*7),174+(i*4),2022+(i%3));}
ps("\nPercussion instruments...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msc_percussion(t,c,207+(i*11),195+(i*9),181+(i*6),171+(i*3),2023+(i%2));}
ps("\nMusic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msc_market(t,c,201+(i*9),190+(i*7),177+(i*5),169+(i*3),2024);}
ps("\n");msc_report();msc_state();ps("\n=== Demo Complete ===\n");return 0;}
