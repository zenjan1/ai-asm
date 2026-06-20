/* wool_admin: Wool management technology administration (v1.0)
 * Wool planning, wool execution, wool evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wol_t;
typedef struct{int n_wp,n_we,n_wv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wol_state_t;
static wol_t wps[N],wes[N-2],wvs[N-4],acs[N-6],mks[N-6]; static wol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wol_init(void){if(init)return -1;st.n_wp=0;st.n_we=0;st.n_wv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wps[i].active=0;for(int i=0;i<N-2;i++)wes[i].active=0;for(int i=0;i<N-4;i++)wvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WOL] Wool initialized\n");return 0;}
int wol_planning(int t,int c,int a,int b,int d,int e,int y){return add(wps,&st.n_wp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wol_execution(int t,int c,int a,int b,int d,int e,int y){return add(wes,&st.n_we,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wvs,&st.n_wv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wol_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wol_report(void){ps("[WOL] Wp: ");pi(st.n_wp);ps(" PCS=");pi(st.t_f1);ps("\nWe: ");pi(st.n_we);ps(" PCS=");pi(st.t_f2);ps("\nWv: ");pi(st.n_wv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wol_state(void){ps("[WOL] Wp=");pi(st.n_wp);ps(" We=");pi(st.n_we);ps(" Wv=");pi(st.n_wv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wool Admin Demo ===\n\n");wol_init();
ps("Wool planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wol_planning(t,c,526+(i*17),515+(i*14),495+(i*10),477+(i*6),2020+(i%5));}
ps("\nWool execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wol_execution(t,c,515+(i*15),504+(i*12),486+(i*8),473+(i*5),2021+(i%4));}
ps("\nWool evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wol_evaluation(t,c,507+(i*13),496+(i*10),480+(i*7),469+(i*4),2022+(i%3));}
ps("\nWool accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wol_accessory(t,c,499+(i*11),490+(i*9),476+(i*6),466+(i*3),2023+(i%2));}
ps("\nWool marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wol_market(t,c,493+(i*9),484+(i*7),471+(i*5),463+(i*3),2024);}
ps("\n");wol_report();wol_state();ps("\n=== Demo Complete ===\n");return 0;}
