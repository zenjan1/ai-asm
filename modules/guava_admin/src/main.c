/* guava_admin: Guava management technology administration (v1.0)
 * Guava planning, guava execution, guava evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} guav_t;
typedef struct{int n_guavp,n_guav,n_guav2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} guav_state_t;
static guav_t guavs[N],guave[N-2],guav2[N-4],guavac[N-6],guavam[N-6]; static guav_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(guav_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;guav_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUAV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int guav_init(void){if(init)return -1;st.n_guavp=0;st.n_guav=0;st.n_guav2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)guavs[i].active=0;for(int i=0;i<N-2;i++)guave[i].active=0;for(int i=0;i<N-4;i++)guav2[i].active=0;for(int i=0;i<N-6;i++)guavac[i].active=0;for(int i=0;i<N-6;i++)guavam[i].active=0;init=1;ps("[GUAV] Guava initialized\n");return 0;}
int guav_planning(int t,int c,int a,int b,int d,int e,int y){return add(guavs,&st.n_guavp,&st.t_f1,N,t,c,a,b,d,e,y);}
int guav_execution(int t,int c,int a,int b,int d,int e,int y){return add(guave,&st.n_guav,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int guav_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(guav2,&st.n_guav2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int guav_accessory(int t,int c,int a,int b,int d,int e,int y){return add(guavac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int guav_market(int t,int c,int a,int b,int d,int e,int y){return add(guavam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void guav_report(void){ps("[GUAV] Guvp: ");pi(st.n_guavp);ps(" PCS=");pi(st.t_f1);ps("\nGuve: ");pi(st.n_guav);ps(" PCS=");pi(st.t_f2);ps("\nGuv2: ");pi(st.n_guav2);ps(" PCS=");pi(st.t_f3);ps("\nGuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void guav_state(void){ps("[GUAV] Guvp=");pi(st.n_guavp);ps(" Guv=");pi(st.n_guav);ps(" Guv2=");pi(st.n_guav2);ps(" Guac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Guava Admin Demo ===\n\n");guav_init();
ps("Guava planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;guav_planning(t,c,1522+(i*17),1511+(i*14),1491+(i*10),1473+(i*6),2020+(i%5));}
ps("\nGuava execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;guav_execution(t,c,1511+(i*15),1500+(i*12),1482+(i*8),1469+(i*5),2021+(i%4));}
ps("\nGuava evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;guav_evaluation(t,c,1503+(i*13),1492+(i*10),1476+(i*7),1465+(i*4),2022+(i%3));}
ps("\nGuava accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;guav_accessory(t,c,1495+(i*11),1486+(i*9),1472+(i*6),1462+(i*3),2023+(i%2));}
ps("\nGuava marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;guav_market(t,c,1489+(i*9),1480+(i*7),1467+(i*5),1459+(i*3),2024);}
ps("\n");guav_report();guav_state();ps("\n=== Demo Complete ===\n");return 0;}
