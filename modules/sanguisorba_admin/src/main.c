/* sanguisorba_admin: Sanguisorba management technology administration (v1.0)
 * Sanguisorba planning, sanguisorba execution, sanguisorba evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sgb_t;
typedef struct{int n_sgp,n_sge,n_sgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sgb_state_t;
static sgb_t sgbps[N],sgbss[N-2],sgbvss[N-4],sgbas[N-6],sgbmks[N-6]; static sgb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sgb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sgb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SGB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sgb_init(void){if(init)return -1;st.n_sgp=0;st.n_sge=0;st.n_sgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sgbps[i].active=0;for(int i=0;i<N-2;i++)sgbss[i].active=0;for(int i=0;i<N-4;i++)sgbvss[i].active=0;for(int i=0;i<N-6;i++)sgbas[i].active=0;for(int i=0;i<N-6;i++)sgbmks[i].active=0;init=1;ps("[SGB] Sanguisorba initialized\n");return 0;}
int sgb_planning(int t,int c,int a,int b,int d,int e,int y){return add(sgbps,&st.n_sgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sgb_execution(int t,int c,int a,int b,int d,int e,int y){return add(sgbss,&st.n_sge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sgb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sgbvss,&st.n_sgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sgb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sgbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sgb_market(int t,int c,int a,int b,int d,int e,int y){return add(sgbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sgb_report(void){ps("[SGB] Sgp: ");pi(st.n_sgp);ps(" PCS=");pi(st.t_f1);ps("\nSge: ");pi(st.n_sge);ps(" PCS=");pi(st.t_f2);ps("\nSgv: ");pi(st.n_sgv);ps(" PCS=");pi(st.t_f3);ps("\nSgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sgb_state(void){ps("[SGB] Sgp=");pi(st.n_sgp);ps(" Sge=");pi(st.n_sge);ps(" Sgv=");pi(st.n_sgv);ps(" Sgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sanguisorba Admin Demo ===\n\n");sgb_init();
ps("Sanguisorba planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sgb_planning(t,c,862+(i*17),851+(i*14),831+(i*10),813+(i*6),2020+(i%5));}
ps("\nSanguisorba execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sgb_execution(t,c,851+(i*15),840+(i*12),822+(i*8),809+(i*5),2021+(i%4));}
ps("\nSanguisorba evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sgb_evaluation(t,c,843+(i*13),832+(i*10),816+(i*7),805+(i*4),2022+(i%3));}
ps("\nSanguisorba accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sgb_accessory(t,c,835+(i*11),826+(i*9),812+(i*6),802+(i*3),2023+(i%2));}
ps("\nSanguisorba marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sgb_market(t,c,829+(i*9),820+(i*7),807+(i*5),799+(i*3),2024);}
ps("\n");sgb_report();sgb_state();ps("\n=== Demo Complete ===\n");return 0;}
