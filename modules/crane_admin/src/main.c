/* crane_admin: Crane technology administration (v1.0)
 * Bridge cranes, gantry cranes, jib cranes, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crx_t;
typedef struct{int n_br,n_gn,n_jb,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crx_state_t;
static crx_t brs[N],gns[N-2],jbs[N-4],acs[N-6],mks[N-6]; static crx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crx_init(void){if(init)return -1;st.n_br=0;st.n_gn=0;st.n_jb=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brs[i].active=0;for(int i=0;i<N-2;i++)gns[i].active=0;for(int i=0;i<N-4;i++)jbs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRX] Crane initialized\n");return 0;}
int crx_bridge(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f1,N,t,c,a,b,d,e,y);}
int crx_gantry(int t,int c,int a,int b,int d,int e,int y){return add(gns,&st.n_gn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crx_jib(int t,int c,int a,int b,int d,int e,int y){return add(jbs,&st.n_jb,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crx_report(void){ps("[CRX] Br: ");pi(st.n_br);ps(" PCS=");pi(st.t_f1);ps("\nGn: ");pi(st.n_gn);ps(" PCS=");pi(st.t_f2);ps("\nJb: ");pi(st.n_jb);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crx_state(void){ps("[CRX] Br=");pi(st.n_br);ps(" Gn=");pi(st.n_gn);ps(" Jb=");pi(st.n_jb);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crane Admin Demo ===\n\n");crx_init();
ps("Bridge cranes...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crx_bridge(t,c,434+(i*17),423+(i*14),403+(i*10),385+(i*6),2020+(i%5));}
ps("\nGantry cranes...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crx_gantry(t,c,423+(i*15),412+(i*12),394+(i*8),381+(i*5),2021+(i%4));}
ps("\nJib cranes...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crx_jib(t,c,415+(i*13),404+(i*10),388+(i*7),377+(i*4),2022+(i%3));}
ps("\nCrane accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crx_accessory(t,c,407+(i*11),398+(i*9),384+(i*6),374+(i*3),2023+(i%2));}
ps("\nCrane marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crx_market(t,c,401+(i*9),392+(i*7),379+(i*5),371+(i*3),2024);}
ps("\n");crx_report();crx_state();ps("\n=== Demo Complete ===\n");return 0;}
