/* cytisus_admin: Cytisus management technology administration (v1.0)
 * Cytisus planning, cytisus execution, cytisus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tis_t;
typedef struct{int n_tisp,n_tise,n_tis2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tis_state_t;
static tis_t tisps[N],tisss[N-2],tisvss[N-4],tisas[N-6],tismks[N-6]; static tis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(tis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tis_init(void){if(init)return -1;st.n_tisp=0;st.n_tise=0;st.n_tis2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tisps[i].active=0;for(int i=0;i<N-2;i++)tisss[i].active=0;for(int i=0;i<N-4;i++)tisvss[i].active=0;for(int i=0;i<N-6;i++)tisas[i].active=0;for(int i=0;i<N-6;i++)tismks[i].active=0;init=1;ps("[TIS] Cytisus initialized\n");return 0;}
int tis_planning(int t,int c,int a,int b,int d,int e,int y){return add(tisps,&st.n_tisp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tis_execution(int t,int c,int a,int b,int d,int e,int y){return add(tisss,&st.n_tise,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tisvss,&st.n_tis2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tisas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tis_market(int t,int c,int a,int b,int d,int e,int y){return add(tismks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tis_report(void){ps("[TIS] Cytpp: ");pi(st.n_tisp);ps(" PCS=");pi(st.t_f1);ps("\nCytpe: ");pi(st.n_tise);ps(" PCS=");pi(st.t_f2);ps("\nCytv: ");pi(st.n_tis2);ps(" PCS=");pi(st.t_f3);ps("\nCytc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tis_state(void){ps("[TIS] Cytpp=");pi(st.n_tisp);ps(" Cytpe=");pi(st.n_tise);ps(" Cytv=");pi(st.n_tis2);ps(" Cytc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cytisus Admin Demo ===\n\n");tis_init();
ps("Cytisus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tis_planning(t,c,1254+(i*17),1243+(i*14),1223+(i*10),1205+(i*6),2020+(i%5));}
ps("\nCytisus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tis_execution(t,c,1243+(i*15),1232+(i*12),1214+(i*8),1201+(i*5),2021+(i%4));}
ps("\nCytisus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tis_evaluation(t,c,1235+(i*13),1224+(i*10),1208+(i*7),1197+(i*4),2022+(i%3));}
ps("\nCytisus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tis_accessory(t,c,1227+(i*11),1218+(i*9),1204+(i*6),1194+(i*3),2023+(i%2));}
ps("\nCytisus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tis_market(t,c,1221+(i*9),1212+(i*7),1199+(i*5),1191+(i*3),2024);}
ps("\n");tis_report();tis_state();ps("\n=== Demo Complete ===\n");return 0;}
