/* disocactus_admin: Disocactus management technology administration (v1.0)
 * Disocactus planning, disocactus execution, disocactus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dsc_t;
typedef struct{int n_dscp,n_dsce,n_dsc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dsc_state_t;
static dsc_t dscps[N],dscss[N-2],dscvss[N-4],dscas[N-6],dscmks[N-6]; static dsc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dsc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dsc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dsc_init(void){if(init)return -1;st.n_dscp=0;st.n_dsce=0;st.n_dsc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dscps[i].active=0;for(int i=0;i<N-2;i++)dscss[i].active=0;for(int i=0;i<N-4;i++)dscvss[i].active=0;for(int i=0;i<N-6;i++)dscas[i].active=0;for(int i=0;i<N-6;i++)dscmks[i].active=0;init=1;ps("[DSC] Disocactus initialized\n");return 0;}
int dsc_planning(int t,int c,int a,int b,int d,int e,int y){return add(dscps,&st.n_dscp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dsc_execution(int t,int c,int a,int b,int d,int e,int y){return add(dscss,&st.n_dsce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dsc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dscvss,&st.n_dsc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dsc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dscas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dsc_market(int t,int c,int a,int b,int d,int e,int y){return add(dscmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dsc_report(void){ps("[DSC] Disp: ");pi(st.n_dscp);ps(" PCS=");pi(st.t_f1);ps("\nDise: ");pi(st.n_dsce);ps(" PCS=");pi(st.t_f2);ps("\nDisv: ");pi(st.n_dsc2);ps(" PCS=");pi(st.t_f3);ps("\nDisc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dsc_state(void){ps("[DSC] Disp=");pi(st.n_dscp);ps(" Dise=");pi(st.n_dsce);ps(" Disv=");pi(st.n_dsc2);ps(" Disc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Disocactus Admin Demo ===\n\n");dsc_init();
ps("Disocactus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dsc_planning(t,c,1270+(i*17),1259+(i*14),1239+(i*10),1221+(i*6),2020+(i%5));}
ps("\nDisocactus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dsc_execution(t,c,1259+(i*15),1248+(i*12),1230+(i*8),1217+(i*5),2021+(i%4));}
ps("\nDisocactus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dsc_evaluation(t,c,1251+(i*13),1240+(i*10),1224+(i*7),1213+(i*4),2022+(i%3));}
ps("\nDisocactus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsc_accessory(t,c,1243+(i*11),1234+(i*9),1220+(i*6),1210+(i*3),2023+(i%2));}
ps("\nDisocactus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsc_market(t,c,1237+(i*9),1228+(i*7),1215+(i*5),1207+(i*3),2024);}
ps("\n");dsc_report();dsc_state();ps("\n=== Demo Complete ===\n");return 0;}
