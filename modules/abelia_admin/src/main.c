/* abelia_admin: Abelia management technology administration (v1.0)
 * Abelia planning, abelia execution, abelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} abe_t;
typedef struct{int n_abep,n_abee,n_abe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} abe_state_t;
static abe_t abeps[N],abess[N-2],abevss[N-4],abeas[N-6],abemks[N-6]; static abe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(abe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;abe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ABE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int abe_init(void){if(init)return -1;st.n_abep=0;st.n_abee=0;st.n_abe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)abeps[i].active=0;for(int i=0;i<N-2;i++)abess[i].active=0;for(int i=0;i<N-4;i++)abevss[i].active=0;for(int i=0;i<N-6;i++)abeas[i].active=0;for(int i=0;i<N-6;i++)abemks[i].active=0;init=1;ps("[ABE] Abelia initialized\n");return 0;}
int abe_planning(int t,int c,int a,int b,int d,int e,int y){return add(abeps,&st.n_abep,&st.t_f1,N,t,c,a,b,d,e,y);}
int abe_execution(int t,int c,int a,int b,int d,int e,int y){return add(abess,&st.n_abee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int abe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(abevss,&st.n_abe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int abe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(abeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int abe_market(int t,int c,int a,int b,int d,int e,int y){return add(abemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void abe_report(void){ps("[ABE] Abep: ");pi(st.n_abep);ps(" PCS=");pi(st.t_f1);ps("\nAbee: ");pi(st.n_abee);ps(" PCS=");pi(st.t_f2);ps("\nAbev: ");pi(st.n_abe2);ps(" PCS=");pi(st.t_f3);ps("\nAbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void abe_state(void){ps("[ABE] Abep=");pi(st.n_abep);ps(" Abee=");pi(st.n_abee);ps(" Abev=");pi(st.n_abe2);ps(" Abc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Abelia Admin Demo ===\n\n");abe_init();
ps("Abelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;abe_planning(t,c,889+(i*17),878+(i*14),858+(i*10),840+(i*6),2020+(i%5));}
ps("\nAbelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;abe_execution(t,c,878+(i*15),867+(i*12),849+(i*8),836+(i*5),2021+(i%4));}
ps("\nAbelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;abe_evaluation(t,c,870+(i*13),859+(i*10),843+(i*7),832+(i*4),2022+(i%3));}
ps("\nAbelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abe_accessory(t,c,862+(i*11),853+(i*9),839+(i*6),829+(i*3),2023+(i%2));}
ps("\nAbelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abe_market(t,c,856+(i*9),847+(i*7),834+(i*5),826+(i*3),2024);}
ps("\n");abe_report();abe_state();ps("\n=== Demo Complete ===\n");return 0;}
