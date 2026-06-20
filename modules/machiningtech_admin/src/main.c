/* machiningtech_admin: Machining technology administration (v1.0)
 * Turning, milling, drilling/boring, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mcn_t;
typedef struct{int n_tn,n_ml,n_dr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mcn_state_t;
static mcn_t tns[N],mls[N-2],drs[N-4],acs[N-6],mks[N-6]; static mcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mcn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mcn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mcn_init(void){if(init)return -1;st.n_tn=0;st.n_ml=0;st.n_dr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tns[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)drs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MCN] Machiningtech initialized\n");return 0;}
int mcn_turning(int t,int c,int a,int b,int d,int e,int y){return add(tns,&st.n_tn,&st.t_f1,N,t,c,a,b,d,e,y);}
int mcn_milling(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mcn_drilling(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mcn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mcn_report(void){ps("[MCN] Tn: ");pi(st.n_tn);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nDr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mcn_state(void){ps("[MCN] Tn=");pi(st.n_tn);ps(" Ml=");pi(st.n_ml);ps(" Dr=");pi(st.n_dr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Machining Tech Admin Demo ===\n\n");mcn_init();
ps("Turning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mcn_turning(t,c,300+(i*17),285+(i*14),265+(i*10),247+(i*6),2020+(i%5));}
ps("\nMilling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mcn_milling(t,c,289+(i*15),275+(i*12),257+(i*8),244+(i*5),2021+(i%4));}
ps("\nDrilling/Boring...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mcn_drilling(t,c,281+(i*13),267+(i*10),251+(i*7),240+(i*4),2022+(i%3));}
ps("\nMachining accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcn_accessory(t,c,273+(i*11),261+(i*9),247+(i*6),237+(i*3),2023+(i%2));}
ps("\nMachining marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcn_market(t,c,267+(i*9),256+(i*7),243+(i*5),235+(i*3),2024);}
ps("\n");mcn_report();mcn_state();ps("\n=== Demo Complete ===\n");return 0;}
