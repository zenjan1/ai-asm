/* dill_admin: Dill management technology administration (v1.0)
 * Dill planning, dill execution, dill evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dl_t;
typedef struct{int n_dlp,n_dle,n_dlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dl_state_t;
static dl_t dlps[N],dlss[N-2],dlvss[N-4],dlas[N-6],dlmks[N-6]; static dl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DLL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dl_init(void){if(init)return -1;st.n_dlp=0;st.n_dle=0;st.n_dlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dlps[i].active=0;for(int i=0;i<N-2;i++)dlss[i].active=0;for(int i=0;i<N-4;i++)dlvss[i].active=0;for(int i=0;i<N-6;i++)dlas[i].active=0;for(int i=0;i<N-6;i++)dlmks[i].active=0;init=1;ps("[DLL] Dill initialized\n");return 0;}
int dl_planning(int t,int c,int a,int b,int d,int e,int y){return add(dlps,&st.n_dlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dl_execution(int t,int c,int a,int b,int d,int e,int y){return add(dlss,&st.n_dle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dlvss,&st.n_dlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dl_market(int t,int c,int a,int b,int d,int e,int y){return add(dlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dl_report(void){ps("[DLL] Dlp: ");pi(st.n_dlp);ps(" PCS=");pi(st.t_f1);ps("\nDle: ");pi(st.n_dle);ps(" PCS=");pi(st.t_f2);ps("\nDlv: ");pi(st.n_dlv);ps(" PCS=");pi(st.t_f3);ps("\nDlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dl_state(void){ps("[DLL] Dlp=");pi(st.n_dlp);ps(" Dle=");pi(st.n_dle);ps(" Dlv=");pi(st.n_dlv);ps(" Dlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dill Admin Demo ===\n\n");dl_init();
ps("Dill planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dl_planning(t,c,657+(i*17),646+(i*14),626+(i*10),608+(i*6),2020+(i%5));}
ps("\nDill execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dl_execution(t,c,646+(i*15),635+(i*12),617+(i*8),604+(i*5),2021+(i%4));}
ps("\nDill evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dl_evaluation(t,c,638+(i*13),627+(i*10),611+(i*7),600+(i*4),2022+(i%3));}
ps("\nDill accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dl_accessory(t,c,630+(i*11),621+(i*9),607+(i*6),597+(i*3),2023+(i%2));}
ps("\nDill marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dl_market(t,c,624+(i*9),615+(i*7),602+(i*5),594+(i*3),2024);}
ps("\n");dl_report();dl_state();ps("\n=== Demo Complete ===\n");return 0;}
