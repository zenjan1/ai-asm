/* echinacea_admin: Echinacea management technology administration (v1.0)
 * Echinacea planning, echinacea execution, echinacea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} en_t;
typedef struct{int n_ecp,n_ece,n_ecv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} en_state_t;
static en_t ecps[N],ecss[N-2],ecvss[N-4],ecas[N-6],ecmks[N-6]; static en_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(en_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;en_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ecn_init(void){if(init)return -1;st.n_ecp=0;st.n_ece=0;st.n_ecv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ecps[i].active=0;for(int i=0;i<N-2;i++)ecss[i].active=0;for(int i=0;i<N-4;i++)ecvss[i].active=0;for(int i=0;i<N-6;i++)ecas[i].active=0;for(int i=0;i<N-6;i++)ecmks[i].active=0;init=1;ps("[ECN] Echinacea initialized\n");return 0;}
int ecn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ecps,&st.n_ecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ecn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ecss,&st.n_ece,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ecn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ecvss,&st.n_ecv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ecn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ecas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ecn_market(int t,int c,int a,int b,int d,int e,int y){return add(ecmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ecn_report(void){ps("[ECN] Ecp: ");pi(st.n_ecp);ps(" PCS=");pi(st.t_f1);ps("\nEce: ");pi(st.n_ece);ps(" PCS=");pi(st.t_f2);ps("\nEcv: ");pi(st.n_ecv);ps(" PCS=");pi(st.t_f3);ps("\nEcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ecn_state(void){ps("[ECN] Ecp=");pi(st.n_ecp);ps(" Ece=");pi(st.n_ece);ps(" Ecv=");pi(st.n_ecv);ps(" Ecc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Echinacea Admin Demo ===\n\n");ecn_init();
ps("Echinacea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ecn_planning(t,c,725+(i*17),714+(i*14),694+(i*10),676+(i*6),2020+(i%5));}
ps("\nEchinacea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ecn_execution(t,c,714+(i*15),703+(i*12),685+(i*8),672+(i*5),2021+(i%4));}
ps("\nEchinacea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ecn_evaluation(t,c,706+(i*13),695+(i*10),679+(i*7),668+(i*4),2022+(i%3));}
ps("\nEchinacea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecn_accessory(t,c,698+(i*11),689+(i*9),675+(i*6),665+(i*3),2023+(i%2));}
ps("\nEchinacea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecn_market(t,c,692+(i*9),683+(i*7),670+(i*5),662+(i*3),2024);}
ps("\n");ecn_report();ecn_state();ps("\n=== Demo Complete ===\n");return 0;}
