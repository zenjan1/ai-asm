/* avens_admin: Avens management technology administration (v1.0)
 * Avens planning, avens execution, avens evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ave_t;
typedef struct{int n_avep,n_avee,n_ave2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ave_state_t;
static ave_t aveps[N],avess[N-2],avevss[N-4],aveas[N-6],avemks[N-6]; static ave_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ave_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ave_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AVE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ave_init(void){if(init)return -1;st.n_avep=0;st.n_avee=0;st.n_ave2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aveps[i].active=0;for(int i=0;i<N-2;i++)avess[i].active=0;for(int i=0;i<N-4;i++)avevss[i].active=0;for(int i=0;i<N-6;i++)aveas[i].active=0;for(int i=0;i<N-6;i++)avemks[i].active=0;init=1;ps("[AVE] Avens initialized\n");return 0;}
int ave_planning(int t,int c,int a,int b,int d,int e,int y){return add(aveps,&st.n_avep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ave_execution(int t,int c,int a,int b,int d,int e,int y){return add(avess,&st.n_avee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ave_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(avevss,&st.n_ave2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ave_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aveas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ave_market(int t,int c,int a,int b,int d,int e,int y){return add(avemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ave_report(void){ps("[AVE] Avepp: ");pi(st.n_avep);ps(" PCS=");pi(st.t_f1);ps("\nAvepe: ");pi(st.n_avee);ps(" PCS=");pi(st.t_f2);ps("\nAvev: ");pi(st.n_ave2);ps(" PCS=");pi(st.t_f3);ps("\nAvec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ave_state(void){ps("[AVE] Avepp=");pi(st.n_avep);ps(" Avepe=");pi(st.n_avee);ps(" Avev=");pi(st.n_ave2);ps(" Avec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Avens Admin Demo ===\n\n");ave_init();
ps("Avens planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ave_planning(t,c,1218+(i*17),1207+(i*14),1187+(i*10),1169+(i*6),2020+(i%5));}
ps("\nAvens execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ave_execution(t,c,1207+(i*15),1196+(i*12),1178+(i*8),1165+(i*5),2021+(i%4));}
ps("\nAvens evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ave_evaluation(t,c,1199+(i*13),1188+(i*10),1172+(i*7),1161+(i*4),2022+(i%3));}
ps("\nAvens accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ave_accessory(t,c,1191+(i*11),1182+(i*9),1168+(i*6),1158+(i*3),2023+(i%2));}
ps("\nAvens marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ave_market(t,c,1185+(i*9),1176+(i*7),1163+(i*5),1155+(i*3),2024);}
ps("\n");ave_report();ave_state();ps("\n=== Demo Complete ===\n");return 0;}
