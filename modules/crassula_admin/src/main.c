/* crassula_admin: Crassula management technology administration (v1.0)
 * Crassula planning, crassula execution, crassula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ras_t;
typedef struct{int n_rasp,n_rase,n_ras2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ras_state_t;
static ras_t rasps[N],rasss[N-2],rasvss[N-4],rasas[N-6],rasmks[N-6]; static ras_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ras_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ras_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ras_init(void){if(init)return -1;st.n_rasp=0;st.n_rase=0;st.n_ras2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rasps[i].active=0;for(int i=0;i<N-2;i++)rasss[i].active=0;for(int i=0;i<N-4;i++)rasvss[i].active=0;for(int i=0;i<N-6;i++)rasas[i].active=0;for(int i=0;i<N-6;i++)rasmks[i].active=0;init=1;ps("[RAS] Crassula initialized\n");return 0;}
int ras_planning(int t,int c,int a,int b,int d,int e,int y){return add(rasps,&st.n_rasp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ras_execution(int t,int c,int a,int b,int d,int e,int y){return add(rasss,&st.n_rase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ras_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rasvss,&st.n_ras2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ras_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rasas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ras_market(int t,int c,int a,int b,int d,int e,int y){return add(rasmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ras_report(void){ps("[RAS] Crapp: ");pi(st.n_rasp);ps(" PCS=");pi(st.t_f1);ps("\nCrape: ");pi(st.n_rase);ps(" PCS=");pi(st.t_f2);ps("\nCrav: ");pi(st.n_ras2);ps(" PCS=");pi(st.t_f3);ps("\nCrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ras_state(void){ps("[RAS] Crapp=");pi(st.n_rasp);ps(" Crape=");pi(st.n_rase);ps(" Crav=");pi(st.n_ras2);ps(" Crac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crassula Admin Demo ===\n\n");ras_init();
ps("Crassula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ras_planning(t,c,1246+(i*17),1235+(i*14),1215+(i*10),1197+(i*6),2020+(i%5));}
ps("\nCrassula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ras_execution(t,c,1235+(i*15),1224+(i*12),1206+(i*8),1193+(i*5),2021+(i%4));}
ps("\nCrassula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ras_evaluation(t,c,1227+(i*13),1216+(i*10),1200+(i*7),1189+(i*4),2022+(i%3));}
ps("\nCrassula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ras_accessory(t,c,1219+(i*11),1210+(i*9),1196+(i*6),1186+(i*3),2023+(i%2));}
ps("\nCrassula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ras_market(t,c,1213+(i*9),1204+(i*7),1191+(i*5),1183+(i*3),2024);}
ps("\n");ras_report();ras_state();ps("\n=== Demo Complete ===\n");return 0;}
