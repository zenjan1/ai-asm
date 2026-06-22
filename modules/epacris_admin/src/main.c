/* epacris_admin: Epacris management technology administration (v1.0)
 * Epacris planning, epacris execution, epacris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} epa_t;
typedef struct{int n_epap,n_epae,n_epa2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} epa_state_t;
static epa_t epaps[N],epass[N-2],epavss[N-4],epaas[N-6],epamks[N-6]; static epa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(epa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;epa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int epa_init(void){if(init)return -1;st.n_epap=0;st.n_epae=0;st.n_epa2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epaps[i].active=0;for(int i=0;i<N-2;i++)epass[i].active=0;for(int i=0;i<N-4;i++)epavss[i].active=0;for(int i=0;i<N-6;i++)epaas[i].active=0;for(int i=0;i<N-6;i++)epamks[i].active=0;init=1;ps("[EPA] Epacris initialized\n");return 0;}
int epa_planning(int t,int c,int a,int b,int d,int e,int y){return add(epaps,&st.n_epap,&st.t_f1,N,t,c,a,b,d,e,y);}
int epa_execution(int t,int c,int a,int b,int d,int e,int y){return add(epass,&st.n_epae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int epa_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(epavss,&st.n_epa2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int epa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(epaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int epa_market(int t,int c,int a,int b,int d,int e,int y){return add(epamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void epa_report(void){ps("[EPA] Epap: ");pi(st.n_epap);ps(" PCS=");pi(st.t_f1);ps("\nEpae: ");pi(st.n_epae);ps(" PCS=");pi(st.t_f2);ps("\nEpav: ");pi(st.n_epa2);ps(" PCS=");pi(st.t_f3);ps("\nEpac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void epa_state(void){ps("[EPA] Epap=");pi(st.n_epap);ps(" Epae=");pi(st.n_epae);ps(" Epav=");pi(st.n_epa2);ps(" Epac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Epacris Admin Demo ===\n\n");epa_init();
ps("Epacris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;epa_planning(t,c,1276+(i*17),1265+(i*14),1245+(i*10),1227+(i*6),2020+(i%5));}
ps("\nEpacris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;epa_execution(t,c,1265+(i*15),1254+(i*12),1236+(i*8),1223+(i*5),2021+(i%4));}
ps("\nEpacris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;epa_evaluation(t,c,1257+(i*13),1246+(i*10),1230+(i*7),1219+(i*4),2022+(i%3));}
ps("\nEpacris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epa_accessory(t,c,1249+(i*11),1240+(i*9),1226+(i*6),1216+(i*3),2023+(i%2));}
ps("\nEpacris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epa_market(t,c,1243+(i*9),1234+(i*7),1221+(i*5),1213+(i*3),2024);}
ps("\n");epa_report();epa_state();ps("\n=== Demo Complete ===\n");return 0;}
