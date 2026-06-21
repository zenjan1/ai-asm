/* wahlenbergia_admin: Wahlenbergia management technology administration (v1.0)
 * Wahlenbergia planning, wahlenbergia execution, wahlenbergia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wbg_t;
typedef struct{int n_wbp,n_wbe,n_wbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wbg_state_t;
static wbg_t wbgps[N],wbgss[N-2],wbgvss[N-4],wbgas[N-6],wbgmks[N-6]; static wbg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wbg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wbg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WBG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wbg_init(void){if(init)return -1;st.n_wbp=0;st.n_wbe=0;st.n_wbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wbgps[i].active=0;for(int i=0;i<N-2;i++)wbgss[i].active=0;for(int i=0;i<N-4;i++)wbgvss[i].active=0;for(int i=0;i<N-6;i++)wbgas[i].active=0;for(int i=0;i<N-6;i++)wbgmks[i].active=0;init=1;ps("[WBG] Wahlenbergia initialized\n");return 0;}
int wbg_planning(int t,int c,int a,int b,int d,int e,int y){return add(wbgps,&st.n_wbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wbg_execution(int t,int c,int a,int b,int d,int e,int y){return add(wbgss,&st.n_wbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wbg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wbgvss,&st.n_wbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wbg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wbgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wbg_market(int t,int c,int a,int b,int d,int e,int y){return add(wbgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wbg_report(void){ps("[WBG] Wbp: ");pi(st.n_wbp);ps(" PCS=");pi(st.t_f1);ps("\nWbe: ");pi(st.n_wbe);ps(" PCS=");pi(st.t_f2);ps("\nWbv: ");pi(st.n_wbv);ps(" PCS=");pi(st.t_f3);ps("\nWbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wbg_state(void){ps("[WBG] Wbp=");pi(st.n_wbp);ps(" Wbe=");pi(st.n_wbe);ps(" Wbv=");pi(st.n_wbv);ps(" Wbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wahlenbergia Admin Demo ===\n\n");wbg_init();
ps("Wahlenbergia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wbg_planning(t,c,878+(i*17),867+(i*14),847+(i*10),829+(i*6),2020+(i%5));}
ps("\nWahlenbergia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wbg_execution(t,c,867+(i*15),856+(i*12),838+(i*8),825+(i*5),2021+(i%4));}
ps("\nWahlenbergia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wbg_evaluation(t,c,859+(i*13),848+(i*10),832+(i*7),821+(i*4),2022+(i%3));}
ps("\nWahlenbergia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wbg_accessory(t,c,851+(i*11),842+(i*9),828+(i*6),818+(i*3),2023+(i%2));}
ps("\nWahlenbergia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wbg_market(t,c,845+(i*9),836+(i*7),823+(i*5),815+(i*3),2024);}
ps("\n");wbg_report();wbg_state();ps("\n=== Demo Complete ===\n");return 0;}
