/* nudicaulis_admin: Nudicaulis management technology administration (v1.0)
 * Nudicaulis planning, nudicaulis execution, nudicaulis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nud_t;
typedef struct{int n_nudp,n_nude,n_nud2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nud_state_t;
static nud_t nudps[N],nudess[N-2],nudvss[N-4],nudas[N-6],nudmks[N-6]; static nud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nud_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nud_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NUD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nud_init(void){if(init)return -1;st.n_nudp=0;st.n_nude=0;st.n_nud2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nudps[i].active=0;for(int i=0;i<N-2;i++)nudess[i].active=0;for(int i=0;i<N-4;i++)nudvss[i].active=0;for(int i=0;i<N-6;i++)nudas[i].active=0;for(int i=0;i<N-6;i++)nudmks[i].active=0;init=1;ps("[NUD] Nudicaulis initialized\n");return 0;}
int nud_planning(int t,int c,int a,int b,int d,int e,int y){return add(nudps,&st.n_nudp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nud_execution(int t,int c,int a,int b,int d,int e,int y){return add(nudess,&st.n_nude,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nud_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nudvss,&st.n_nud2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nud_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nudas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nud_market(int t,int c,int a,int b,int d,int e,int y){return add(nudmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nud_report(void){ps("[NUD] Nudp: ");pi(st.n_nudp);ps(" PCS=");pi(st.t_f1);ps("\nNude: ");pi(st.n_nude);ps(" PCS=");pi(st.t_f2);ps("\nNudv: ");pi(st.n_nud2);ps(" PCS=");pi(st.t_f3);ps("\nNudc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nud_state(void){ps("[NUD] Nudp=");pi(st.n_nudp);ps(" Nude=");pi(st.n_nude);ps(" Nudv=");pi(st.n_nud2);ps(" Nudc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nudicaulis Admin Demo ===\n\n");nud_init();
ps("Nudicaulis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nud_planning(t,c,1320+(i*17),1309+(i*14),1289+(i*10),1271+(i*6),2020+(i%5));}
ps("\nNudicaulis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nud_execution(t,c,1309+(i*15),1298+(i*12),1280+(i*8),1267+(i*5),2021+(i%4));}
ps("\nNudicaulis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nud_evaluation(t,c,1301+(i*13),1290+(i*10),1274+(i*7),1263+(i*4),2022+(i%3));}
ps("\nNudicaulis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nud_accessory(t,c,1293+(i*11),1284+(i*9),1270+(i*6),1260+(i*3),2023+(i%2));}
ps("\nNudicaulis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nud_market(t,c,1288+(i*9),1279+(i*7),1266+(i*5),1258+(i*3),2024);}
ps("\n");nud_report();nud_state();ps("\n=== Demo Complete ===\n");return 0;}
