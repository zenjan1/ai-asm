/* cercis_admin: Cercis management technology administration (v1.0)
 * Cercis planning, cercis execution, cercis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cerc_t;
typedef struct{int n_cercp,n_cerc,n_cerc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cerc_state_t;
static cerc_t cercs[N],cerce[N-2],cerc2[N-4],cercac[N-6],cercam[N-6]; static cerc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cerc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cerc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CERC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cerc_init(void){if(init)return -1;st.n_cercp=0;st.n_cerc=0;st.n_cerc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cercs[i].active=0;for(int i=0;i<N-2;i++)cerce[i].active=0;for(int i=0;i<N-4;i++)cerc2[i].active=0;for(int i=0;i<N-6;i++)cercac[i].active=0;for(int i=0;i<N-6;i++)cercam[i].active=0;init=1;ps("[CERC] Cercis initialized\n");return 0;}
int cerc_planning(int t,int c,int a,int b,int d,int e,int y){return add(cercs,&st.n_cercp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cerc_execution(int t,int c,int a,int b,int d,int e,int y){return add(cerce,&st.n_cerc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cerc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cerc2,&st.n_cerc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cerc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cercac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cerc_market(int t,int c,int a,int b,int d,int e,int y){return add(cercam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cerc_report(void){ps("[CERC] Erp: ");pi(st.n_cercp);ps(" PCS=");pi(st.t_f1);ps("\nEre: ");pi(st.n_cerc);ps(" PCS=");pi(st.t_f2);ps("\nEr2: ");pi(st.n_cerc2);ps(" PCS=");pi(st.t_f3);ps("\nErcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cerc_state(void){ps("[CERC] Erp=");pi(st.n_cercp);ps(" Err=");pi(st.n_cerc);ps(" Er2=");pi(st.n_cerc2);ps(" Ercac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cercis Admin Demo ===\n\n");cerc_init();
ps("Cercis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cerc_planning(t,c,1470+(i*17),1459+(i*14),1439+(i*10),1421+(i*6),2020+(i%5));}
ps("\nCercis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cerc_execution(t,c,1459+(i*15),1448+(i*12),1430+(i*8),1417+(i*5),2021+(i%4));}
ps("\nCercis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cerc_evaluation(t,c,1451+(i*13),1440+(i*10),1424+(i*7),1413+(i*4),2022+(i%3));}
ps("\nCercis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cerc_accessory(t,c,1443+(i*11),1434+(i*9),1420+(i*6),1410+(i*3),2023+(i%2));}
ps("\nCercis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cerc_market(t,c,1437+(i*9),1428+(i*7),1415+(i*5),1407+(i*3),2024);}
ps("\n");cerc_report();cerc_state();ps("\n=== Demo Complete ===\n");return 0;}
