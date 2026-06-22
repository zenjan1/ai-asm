/* bloom_admin: Bloom management technology administration (v1.0)
 * Bloom planning, bloom execution, bloom evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blo_t;
typedef struct{int n_blop,n_bloe,n_blo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blo_state_t;
static blo_t blops[N],bloss[N-2],blovss[N-4],bloas[N-6],blomks[N-6]; static blo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(blo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blo_init(void){if(init)return -1;st.n_blop=0;st.n_bloe=0;st.n_blo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)blops[i].active=0;for(int i=0;i<N-2;i++)bloss[i].active=0;for(int i=0;i<N-4;i++)blovss[i].active=0;for(int i=0;i<N-6;i++)bloas[i].active=0;for(int i=0;i<N-6;i++)blomks[i].active=0;init=1;ps("[BLO] Bloom initialized\n");return 0;}
int blo_planning(int t,int c,int a,int b,int d,int e,int y){return add(blops,&st.n_blop,&st.t_f1,N,t,c,a,b,d,e,y);}
int blo_execution(int t,int c,int a,int b,int d,int e,int y){return add(bloss,&st.n_bloe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(blovss,&st.n_blo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bloas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blo_market(int t,int c,int a,int b,int d,int e,int y){return add(blomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blo_report(void){ps("[BLO] Blopp: ");pi(st.n_blop);ps(" PCS=");pi(st.t_f1);ps("\nBlope: ");pi(st.n_bloe);ps(" PCS=");pi(st.t_f2);ps("\nBlov: ");pi(st.n_blo2);ps(" PCS=");pi(st.t_f3);ps("\nBloc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blo_state(void){ps("[BLO] Blopp=");pi(st.n_blop);ps(" Blope=");pi(st.n_bloe);ps(" Blov=");pi(st.n_blo2);ps(" Bloc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bloom Admin Demo ===\n\n");blo_init();
ps("Bloom planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blo_planning(t,c,1221+(i*17),1210+(i*14),1190+(i*10),1172+(i*6),2020+(i%5));}
ps("\nBloom execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blo_execution(t,c,1210+(i*15),1199+(i*12),1181+(i*8),1168+(i*5),2021+(i%4));}
ps("\nBloom evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blo_evaluation(t,c,1202+(i*13),1191+(i*10),1175+(i*7),1164+(i*4),2022+(i%3));}
ps("\nBloom accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blo_accessory(t,c,1194+(i*11),1185+(i*9),1171+(i*6),1161+(i*3),2023+(i%2));}
ps("\nBloom marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blo_market(t,c,1188+(i*9),1179+(i*7),1166+(i*5),1158+(i*3),2024);}
ps("\n");blo_report();blo_state();ps("\n=== Demo Complete ===\n");return 0;}
