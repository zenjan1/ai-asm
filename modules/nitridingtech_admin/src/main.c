/* nitridingtech_admin: Nitriding technology administration (v1.0)
 * Gas nitriding, ion nitriding, salt bath nitriding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ntr_t;
typedef struct{int n_gs,n_io,n_sa,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ntr_state_t;
static ntr_t gss[N],ios[N-2],sas[N-4],acs[N-6],mks[N-6]; static ntr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ntr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ntr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ntr_init(void){if(init)return -1;st.n_gs=0;st.n_io=0;st.n_sa=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gss[i].active=0;for(int i=0;i<N-2;i++)ios[i].active=0;for(int i=0;i<N-4;i++)sas[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NTR] Nitridingtech initialized\n");return 0;}
int ntr_gas(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f1,N,t,c,a,b,d,e,y);}
int ntr_ion(int t,int c,int a,int b,int d,int e,int y){return add(ios,&st.n_io,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ntr_salt(int t,int c,int a,int b,int d,int e,int y){return add(sas,&st.n_sa,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ntr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ntr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ntr_report(void){ps("[NTR] Gs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f1);ps("\nIo: ");pi(st.n_io);ps(" PCS=");pi(st.t_f2);ps("\nSa: ");pi(st.n_sa);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ntr_state(void){ps("[NTR] Gs=");pi(st.n_gs);ps(" Io=");pi(st.n_io);ps(" Sa=");pi(st.n_sa);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nitriding Tech Admin Demo ===\n\n");ntr_init();
ps("Gas nitriding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ntr_gas(t,c,328+(i*17),313+(i*14),293+(i*10),275+(i*6),2020+(i%5));}
ps("\nIon nitriding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ntr_ion(t,c,317+(i*15),303+(i*12),285+(i*8),272+(i*5),2021+(i%4));}
ps("\nSalt bath nitriding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ntr_salt(t,c,309+(i*13),295+(i*10),279+(i*7),268+(i*4),2022+(i%3));}
ps("\nNitriding accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntr_accessory(t,c,301+(i*11),289+(i*9),275+(i*6),265+(i*3),2023+(i%2));}
ps("\nNitriding marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntr_market(t,c,295+(i*9),284+(i*7),271+(i*5),263+(i*3),2024);}
ps("\n");ntr_report();ntr_state();ps("\n=== Demo Complete ===\n");return 0;}
