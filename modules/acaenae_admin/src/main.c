/* acaenae_admin: Acaenae management technology administration (v1.0)
 * Acaenae planning, acaenae execution, acaenae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acae_t;
typedef struct{int n_acaep,n_acae,n_acae2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acae_state_t;
static acae_t acaes[N],acaee[N-2],acaev[N-4],acaac[N-6],acaem[N-6]; static acae_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acae_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acae_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACAE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acae_init(void){if(init)return -1;st.n_acaep=0;st.n_acae=0;st.n_acae2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acaes[i].active=0;for(int i=0;i<N-2;i++)acaee[i].active=0;for(int i=0;i<N-4;i++)acaev[i].active=0;for(int i=0;i<N-6;i++)acaac[i].active=0;for(int i=0;i<N-6;i++)acaem[i].active=0;init=1;ps("[ACAE] Acaenae initialized\n");return 0;}
int acae_planning(int t,int c,int a,int b,int d,int e,int y){return add(acaes,&st.n_acaep,&st.t_f1,N,t,c,a,b,d,e,y);}
int acae_execution(int t,int c,int a,int b,int d,int e,int y){return add(acaee,&st.n_acae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acae_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acaev,&st.n_acae2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acae_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acae_market(int t,int c,int a,int b,int d,int e,int y){return add(acaem,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acae_report(void){ps("[ACAE] Caep: ");pi(st.n_acaep);ps(" PCS=");pi(st.t_f1);ps("\nCae: ");pi(st.n_acae);ps(" PCS=");pi(st.t_f2);ps("\nCav: ");pi(st.n_acae2);ps(" PCS=");pi(st.t_f3);ps("\nCaac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acae_state(void){ps("[ACAE] Caep=");pi(st.n_acaep);ps(" Cae=");pi(st.n_acae);ps(" Cav=");pi(st.n_acae2);ps(" Caac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acaenae Admin Demo ===\n\n");acae_init();
ps("Acaenae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acae_planning(t,c,1387+(i*17),1376+(i*14),1356+(i*10),1338+(i*6),2020+(i%5));}
ps("\nAcaenae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acae_execution(t,c,1376+(i*15),1365+(i*12),1347+(i*8),1334+(i*5),2021+(i%4));}
ps("\nAcaenae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acae_evaluation(t,c,1368+(i*13),1357+(i*10),1341+(i*7),1330+(i*4),2022+(i%3));}
ps("\nAcaenae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acae_accessory(t,c,1360+(i*11),1351+(i*9),1337+(i*6),1327+(i*3),2023+(i%2));}
ps("\nAcaenae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acae_market(t,c,1354+(i*9),1345+(i*7),1332+(i*5),1324+(i*3),2024);}
ps("\n");acae_report();acae_state();ps("\n=== Demo Complete ===\n");return 0;}
