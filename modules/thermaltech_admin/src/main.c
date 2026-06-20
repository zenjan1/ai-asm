/* thermaltech_admin: Thermal technology administration (v1.0)
 * Heat treatment, thermal spray, thermal cutting, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} thm_t;
typedef struct{int n_ht,n_ts,n_tc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} thm_state_t;
static thm_t hts[N],tss[N-2],tcs[N-4],acs[N-6],mks[N-6]; static thm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;thm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int thm_init(void){if(init)return -1;st.n_ht=0;st.n_ts=0;st.n_tc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hts[i].active=0;for(int i=0;i<N-2;i++)tss[i].active=0;for(int i=0;i<N-4;i++)tcs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[THM] Thermaltech initialized\n");return 0;}
int thm_heattreat(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f1,N,t,c,a,b,d,e,y);}
int thm_spray(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int thm_cutting(int t,int c,int a,int b,int d,int e,int y){return add(tcs,&st.n_tc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int thm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int thm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void thm_report(void){ps("[THM] Ht: ");pi(st.n_ht);ps(" PCS=");pi(st.t_f1);ps("\nTs: ");pi(st.n_ts);ps(" PCS=");pi(st.t_f2);ps("\nTc: ");pi(st.n_tc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void thm_state(void){ps("[THM] Ht=");pi(st.n_ht);ps(" Ts=");pi(st.n_ts);ps(" Tc=");pi(st.n_tc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thermal Tech Admin Demo ===\n\n");thm_init();
ps("Heat treatment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;thm_heattreat(t,c,303+(i*17),288+(i*14),268+(i*10),250+(i*6),2020+(i%5));}
ps("\nThermal spray...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;thm_spray(t,c,292+(i*15),278+(i*12),260+(i*8),247+(i*5),2021+(i%4));}
ps("\nThermal cutting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;thm_cutting(t,c,284+(i*13),270+(i*10),254+(i*7),243+(i*4),2022+(i%3));}
ps("\nThermal accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thm_accessory(t,c,276+(i*11),264+(i*9),250+(i*6),240+(i*3),2023+(i%2));}
ps("\nThermal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thm_market(t,c,270+(i*9),259+(i*7),246+(i*5),238+(i*3),2024);}
ps("\n");thm_report();thm_state();ps("\n=== Demo Complete ===\n");return 0;}
