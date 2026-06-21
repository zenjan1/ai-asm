/* liriope_admin: Liriope management technology administration (v1.0)
 * Liriope planning, liriope execution, liriope evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lir_t;
typedef struct{int n_lirp,n_lire,n_lir2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lir_state_t;
static lir_t lirps[N],lirss[N-2],lirvss[N-4],liras[N-6],lirmks[N-6]; static lir_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lir_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lir_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LIR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lir_init(void){if(init)return -1;st.n_lirp=0;st.n_lire=0;st.n_lir2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lirps[i].active=0;for(int i=0;i<N-2;i++)lirss[i].active=0;for(int i=0;i<N-4;i++)lirvss[i].active=0;for(int i=0;i<N-6;i++)liras[i].active=0;for(int i=0;i<N-6;i++)lirmks[i].active=0;init=1;ps("[LIR] Liriope initialized\n");return 0;}
int lir_planning(int t,int c,int a,int b,int d,int e,int y){return add(lirps,&st.n_lirp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lir_execution(int t,int c,int a,int b,int d,int e,int y){return add(lirss,&st.n_lire,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lir_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lirvss,&st.n_lir2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lir_accessory(int t,int c,int a,int b,int d,int e,int y){return add(liras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lir_market(int t,int c,int a,int b,int d,int e,int y){return add(lirmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lir_report(void){ps("[LIR] Lirp: ");pi(st.n_lirp);ps(" PCS=");pi(st.t_f1);ps("\nLire: ");pi(st.n_lire);ps(" PCS=");pi(st.t_f2);ps("\nLirv: ");pi(st.n_lir2);ps(" PCS=");pi(st.t_f3);ps("\nLirc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lir_state(void){ps("[LIR] Lirp=");pi(st.n_lirp);ps(" Lire=");pi(st.n_lire);ps(" Lirv=");pi(st.n_lir2);ps(" Lirc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Liriope Admin Demo ===\n\n");lir_init();
ps("Liriope planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lir_planning(t,c,1057+(i*17),1046+(i*14),1026+(i*10),1008+(i*6),2020+(i%5));}
ps("\nLiriope execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lir_execution(t,c,1046+(i*15),1035+(i*12),1017+(i*8),1004+(i*5),2021+(i%4));}
ps("\nLiriope evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lir_evaluation(t,c,1038+(i*13),1027+(i*10),1011+(i*7),1000+(i*4),2022+(i%3));}
ps("\nLiriope accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lir_accessory(t,c,1030+(i*11),1021+(i*9),1007+(i*6),997+(i*3),2023+(i%2));}
ps("\nLiriope marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lir_market(t,c,1024+(i*9),1015+(i*7),1002+(i*5),994+(i*3),2024);}
ps("\n");lir_report();lir_state();ps("\n=== Demo Complete ===\n");return 0;}
