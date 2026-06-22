/* tecoma_admin: Tecoma management technology administration (v1.0)
 * Tecoma planning, tecoma execution, tecoma evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tec_t;
typedef struct{int n_tecp,n_tece,n_tec2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tec_state_t;
static tec_t tecps[N],tecss[N-2],tecvss[N-4],tecas[N-6],tecmks[N-6]; static tec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tec_init(void){if(init)return -1;st.n_tecp=0;st.n_tece=0;st.n_tec2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tecps[i].active=0;for(int i=0;i<N-2;i++)tecss[i].active=0;for(int i=0;i<N-4;i++)tecvss[i].active=0;for(int i=0;i<N-6;i++)tecas[i].active=0;for(int i=0;i<N-6;i++)tecmks[i].active=0;init=1;ps("[TEC] Tecoma initialized\n");return 0;}
int tec_planning(int t,int c,int a,int b,int d,int e,int y){return add(tecps,&st.n_tecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tec_execution(int t,int c,int a,int b,int d,int e,int y){return add(tecss,&st.n_tece,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tec_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tecvss,&st.n_tec2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tec_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tecas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tec_market(int t,int c,int a,int b,int d,int e,int y){return add(tecmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tec_report(void){ps("[TEC] Tecpp: ");pi(st.n_tecp);ps(" PCS=");pi(st.t_f1);ps("\nTece: ");pi(st.n_tece);ps(" PCS=");pi(st.t_f2);ps("\nTecv: ");pi(st.n_tec2);ps(" PCS=");pi(st.t_f3);ps("\nTecc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tec_state(void){ps("[TEC] Tecpp=");pi(st.n_tecp);ps(" Tece=");pi(st.n_tece);ps(" Tecv=");pi(st.n_tec2);ps(" Tecc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tecoma Admin Demo ===\n\n");tec_init();
ps("Tecoma planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tec_planning(t,c,1196+(i*17),1185+(i*14),1165+(i*10),1147+(i*6),2020+(i%5));}
ps("\nTecoma execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tec_execution(t,c,1185+(i*15),1174+(i*12),1156+(i*8),1143+(i*5),2021+(i%4));}
ps("\nTecoma evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tec_evaluation(t,c,1177+(i*13),1166+(i*10),1150+(i*7),1139+(i*4),2022+(i%3));}
ps("\nTecoma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tec_accessory(t,c,1169+(i*11),1160+(i*9),1146+(i*6),1136+(i*3),2023+(i%2));}
ps("\nTecoma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tec_market(t,c,1163+(i*9),1154+(i*7),1141+(i*5),1133+(i*3),2024);}
ps("\n");tec_report();tec_state();ps("\n=== Demo Complete ===\n");return 0;}
