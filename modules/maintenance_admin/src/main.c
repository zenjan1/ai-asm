/* maintenance_admin: Maintenance management technology administration (v1.0)
 * Preventive maintenance, corrective maintenance, TPM, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mnx_t;
typedef struct{int n_pv,n_cm,n_tp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mnx_state_t;
static mnx_t pvs[N],cms[N-2],tps[N-4],acs[N-6],mks[N-6]; static mnx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mnx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mnx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MNX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mnx_init(void){if(init)return -1;st.n_pv=0;st.n_cm=0;st.n_tp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pvs[i].active=0;for(int i=0;i<N-2;i++)cms[i].active=0;for(int i=0;i<N-4;i++)tps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MNX] Maintenance initialized\n");return 0;}
int mnx_preventive(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f1,N,t,c,a,b,d,e,y);}
int mnx_corrective(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mnx_tpm(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mnx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mnx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mnx_report(void){ps("[MNX] Pv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f1);ps("\nCm: ");pi(st.n_cm);ps(" PCS=");pi(st.t_f2);ps("\nTp: ");pi(st.n_tp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mnx_state(void){ps("[MNX] Pv=");pi(st.n_pv);ps(" Cm=");pi(st.n_cm);ps(" Tp=");pi(st.n_tp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Maintenance Admin Demo ===\n\n");mnx_init();
ps("Preventive maintenance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mnx_preventive(t,c,440+(i*17),429+(i*14),409+(i*10),391+(i*6),2020+(i%5));}
ps("\nCorrective maintenance...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mnx_corrective(t,c,429+(i*15),418+(i*12),400+(i*8),387+(i*5),2021+(i%4));}
ps("\nTPM...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mnx_tpm(t,c,421+(i*13),410+(i*10),394+(i*7),383+(i*4),2022+(i%3));}
ps("\nMaintenance accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnx_accessory(t,c,413+(i*11),404+(i*9),390+(i*6),380+(i*3),2023+(i%2));}
ps("\nMaintenance marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnx_market(t,c,407+(i*9),398+(i*7),385+(i*5),377+(i*3),2024);}
ps("\n");mnx_report();mnx_state();ps("\n=== Demo Complete ===\n");return 0;}
