/* zinnia_admin: Zinnia management technology administration (v1.0)
 * Zinnia planning, zinnia execution, zinnia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zin_t;
typedef struct{int n_zp,n_ze,n_zv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zin_state_t;
static zin_t zps[N],zes[N-2],zvs[N-4],acs[N-6],mks[N-6]; static zin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zin_init(void){if(init)return -1;st.n_zp=0;st.n_ze=0;st.n_zv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zps[i].active=0;for(int i=0;i<N-2;i++)zes[i].active=0;for(int i=0;i<N-4;i++)zvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ZIN] Zinnia initialized\n");return 0;}
int zin_planning(int t,int c,int a,int b,int d,int e,int y){return add(zps,&st.n_zp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zin_execution(int t,int c,int a,int b,int d,int e,int y){return add(zes,&st.n_ze,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zvs,&st.n_zv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zin_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zin_report(void){ps("[ZIN] Zp: ");pi(st.n_zp);ps(" PCS=");pi(st.t_f1);ps("\nZe: ");pi(st.n_ze);ps(" PCS=");pi(st.t_f2);ps("\nZv: ");pi(st.n_zv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zin_state(void){ps("[ZIN] Zp=");pi(st.n_zp);ps(" Ze=");pi(st.n_ze);ps(" Zv=");pi(st.n_zv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zinnia Admin Demo ===\n\n");zin_init();
ps("Zinnia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zin_planning(t,c,557+(i*17),546+(i*14),526+(i*10),508+(i*6),2020+(i%5));}
ps("\nZinnia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zin_execution(t,c,546+(i*15),535+(i*12),517+(i*8),504+(i*5),2021+(i%4));}
ps("\nZinnia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zin_evaluation(t,c,538+(i*13),527+(i*10),511+(i*7),500+(i*4),2022+(i%3));}
ps("\nZinnia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zin_accessory(t,c,530+(i*11),521+(i*9),507+(i*6),497+(i*3),2023+(i%2));}
ps("\nZinnia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zin_market(t,c,524+(i*9),515+(i*7),502+(i*5),494+(i*3),2024);}
ps("\n");zin_report();zin_state();ps("\n=== Demo Complete ===\n");return 0;}
