/* ligularia_admin: Ligularia management technology administration (v1.0)
 * Ligularia planning, ligularia execution, ligularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lr_t;
typedef struct{int n_lgp,n_lge,n_lgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lr_state_t;
static lr_t lgps[N],lgss[N-2],lgvss[N-4],lgas[N-6],lgmks[N-6]; static lr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LGR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lgr_init(void){if(init)return -1;st.n_lgp=0;st.n_lge=0;st.n_lgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lgps[i].active=0;for(int i=0;i<N-2;i++)lgss[i].active=0;for(int i=0;i<N-4;i++)lgvss[i].active=0;for(int i=0;i<N-6;i++)lgas[i].active=0;for(int i=0;i<N-6;i++)lgmks[i].active=0;init=1;ps("[LGR] Ligularia initialized\n");return 0;}
int lgr_planning(int t,int c,int a,int b,int d,int e,int y){return add(lgps,&st.n_lgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lgr_execution(int t,int c,int a,int b,int d,int e,int y){return add(lgss,&st.n_lge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lgr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lgvss,&st.n_lgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lgr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lgr_market(int t,int c,int a,int b,int d,int e,int y){return add(lgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lgr_report(void){ps("[LGR] Lgp: ");pi(st.n_lgp);ps(" PCS=");pi(st.t_f1);ps("\nLge: ");pi(st.n_lge);ps(" PCS=");pi(st.t_f2);ps("\nLgv: ");pi(st.n_lgv);ps(" PCS=");pi(st.t_f3);ps("\nLgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lgr_state(void){ps("[LGR] Lgp=");pi(st.n_lgp);ps(" Lge=");pi(st.n_lge);ps(" Lgv=");pi(st.n_lgv);ps(" Lgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ligularia Admin Demo ===\n\n");lgr_init();
ps("Ligularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lgr_planning(t,c,739+(i*17),728+(i*14),708+(i*10),690+(i*6),2020+(i%5));}
ps("\nLigularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lgr_execution(t,c,728+(i*15),717+(i*12),699+(i*8),686+(i*5),2021+(i%4));}
ps("\nLigularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lgr_evaluation(t,c,720+(i*13),709+(i*10),693+(i*7),682+(i*4),2022+(i%3));}
ps("\nLigularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgr_accessory(t,c,712+(i*11),703+(i*9),689+(i*6),679+(i*3),2023+(i%2));}
ps("\nLigularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgr_market(t,c,706+(i*9),697+(i*7),684+(i*5),676+(i*3),2024);}
ps("\n");lgr_report();lgr_state();ps("\n=== Demo Complete ===\n");return 0;}
