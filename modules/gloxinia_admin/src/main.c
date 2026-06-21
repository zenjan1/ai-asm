/* gloxinia_admin: Gloxinia management technology administration (v1.0)
 * Gloxinia planning, gloxinia execution, gloxinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gx_t;
typedef struct{int n_gxp,n_gxe,n_gxv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gx_state_t;
static gx_t gxps[N],gxss[N-2],gxvss[N-4],gxs[N-6],gxmks[N-6]; static gx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[Gxn] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gxn_init(void){if(init)return -1;st.n_gxp=0;st.n_gxe=0;st.n_gxv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gxps[i].active=0;for(int i=0;i<N-2;i++)gxss[i].active=0;for(int i=0;i<N-4;i++)gxvss[i].active=0;for(int i=0;i<N-6;i++)gxs[i].active=0;for(int i=0;i<N-6;i++)gxmks[i].active=0;init=1;ps("[Gxn] Gloxinia initialized\n");return 0;}
int gxn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gxps,&st.n_gxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gxn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gxss,&st.n_gxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gxn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gxvss,&st.n_gxv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gxn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gxs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gxn_market(int t,int c,int a,int b,int d,int e,int y){return add(gxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gxn_report(void){ps("[Gxn] Gxp: ");pi(st.n_gxp);ps(" PCS=");pi(st.t_f1);ps("\nGxe: ");pi(st.n_gxe);ps(" PCS=");pi(st.t_f2);ps("\nGxv: ");pi(st.n_gxv);ps(" PCS=");pi(st.t_f3);ps("\nGxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gxn_state(void){ps("[Gxn] Gxp=");pi(st.n_gxp);ps(" Gxe=");pi(st.n_gxe);ps(" Gxv=");pi(st.n_gxv);ps(" Gxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gloxinia Admin Demo ===\n\n");gxn_init();
ps("Gloxinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gxn_planning(t,c,779+(i*17),768+(i*14),748+(i*10),730+(i*6),2020+(i%5));}
ps("\nGloxinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gxn_execution(t,c,768+(i*15),757+(i*12),739+(i*8),726+(i*5),2021+(i%4));}
ps("\nGloxinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gxn_evaluation(t,c,760+(i*13),749+(i*10),735+(i*7),724+(i*4),2022+(i%3));}
ps("\nGloxinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gxn_accessory(t,c,752+(i*11),743+(i*9),729+(i*6),719+(i*3),2023+(i%2));}
ps("\nGloxinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gxn_market(t,c,746+(i*9),737+(i*7),724+(i*5),716+(i*3),2024);}
ps("\n");gxn_report();gxn_state();ps("\n=== Demo Complete ===\n");return 0;}
