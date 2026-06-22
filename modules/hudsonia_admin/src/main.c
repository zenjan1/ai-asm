/* hudsonia_admin: Hudsonia management technology administration (v1.0)
 * Hudsonia planning, hudsonia execution, hudsonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} huds_t;
typedef struct{int n_hudsp,n_huds,n_huds2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} huds_state_t;
static huds_t hudss[N],huds_e[N-2],huds2[N-4],hudsac[N-6],hudsam[N-6]; static huds_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(huds_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;huds_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUDS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int huds_init(void){if(init)return -1;st.n_hudsp=0;st.n_huds=0;st.n_huds2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hudss[i].active=0;for(int i=0;i<N-2;i++)huds_e[i].active=0;for(int i=0;i<N-4;i++)huds2[i].active=0;for(int i=0;i<N-6;i++)hudsac[i].active=0;for(int i=0;i<N-6;i++)hudsam[i].active=0;init=1;ps("[HUDS] Hudsonia initialized\n");return 0;}
int huds_planning(int t,int c,int a,int b,int d,int e,int y){return add(hudss,&st.n_hudsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int huds_execution(int t,int c,int a,int b,int d,int e,int y){return add(huds_e,&st.n_huds,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int huds_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(huds2,&st.n_huds2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int huds_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hudsac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int huds_market(int t,int c,int a,int b,int d,int e,int y){return add(hudsam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void huds_report(void){ps("[HUDS] Hudsp: ");pi(st.n_hudsp);ps(" PCS=");pi(st.t_f1);ps("\nHuds: ");pi(st.n_huds);ps(" PCS=");pi(st.t_f2);ps("\nHuds2: ");pi(st.n_huds2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void huds_state(void){ps("[HUDS] Hudsp=");pi(st.n_hudsp);ps(" Huds=");pi(st.n_huds);ps(" Huds2=");pi(st.n_huds2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hudsonia Admin Demo ===\n\n");huds_init();
ps("Hudsonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;huds_planning(t,c,1536+(i*17),1525+(i*14),1505+(i*10),1487+(i*6),2020+(i%5));}
ps("\nHudsonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;huds_execution(t,c,1525+(i*15),1514+(i*12),1496+(i*8),1483+(i*5),2021+(i%4));}
ps("\nHudsonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;huds_evaluation(t,c,1517+(i*13),1506+(i*10),1490+(i*7),1479+(i*4),2022+(i%3));}
ps("\nHudsonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;huds_accessory(t,c,1509+(i*11),1500+(i*9),1486+(i*6),1476+(i*3),2023+(i%2));}
ps("\nHudsonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;huds_market(t,c,1503+(i*9),1494+(i*7),1481+(i*5),1473+(i*3),2024);}
ps("\n");huds_report();huds_state();ps("\n=== Demo Complete ===\n");return 0;}
