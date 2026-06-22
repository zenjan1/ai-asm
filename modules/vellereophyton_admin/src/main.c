/* vellereophyton_admin: Vellereophyton management technology administration (v1.0)
 * Vellereophyton planning, vellereophyton execution, vellereophyton evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vell_t;
typedef struct{int n_vellp,n_velle,n_vell2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vell_state_t;
static vell_t vells[N],velless[N-2],vellvss[N-4],vellas[N-6],vellmks[N-6]; static vell_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vell_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vell_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VELL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vell_init(void){if(init)return -1;st.n_vellp=0;st.n_velle=0;st.n_vell2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vells[i].active=0;for(int i=0;i<N-2;i++)velless[i].active=0;for(int i=0;i<N-4;i++)vellvss[i].active=0;for(int i=0;i<N-6;i++)vellas[i].active=0;for(int i=0;i<N-6;i++)vellmks[i].active=0;init=1;ps("[VELL] Vellereophyton initialized\n");return 0;}
int vell_planning(int t,int c,int a,int b,int d,int e,int y){return add(vells,&st.n_vellp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vell_execution(int t,int c,int a,int b,int d,int e,int y){return add(velless,&st.n_velle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vell_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vellvss,&st.n_vell2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vell_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vellas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vell_market(int t,int c,int a,int b,int d,int e,int y){return add(vellmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vell_report(void){ps("[VELL] Vellp: ");pi(st.n_vellp);ps(" PCS=");pi(st.t_f1);ps("\nVelle: ");pi(st.n_velle);ps(" PCS=");pi(st.t_f2);ps("\nVellv: ");pi(st.n_vell2);ps(" PCS=");pi(st.t_f3);ps("\nVellc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vell_state(void){ps("[VELL] Vellp=");pi(st.n_vellp);ps(" Velle=");pi(st.n_velle);ps(" Vellv=");pi(st.n_vell2);ps(" Vellc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vellereophyton Admin Demo ===\n\n");vell_init();
ps("Vellereophyton planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vell_planning(t,c,1354+(i*17),1343+(i*14),1323+(i*10),1305+(i*6),2020+(i%5));}
ps("\nVellereophyton execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vell_execution(t,c,1343+(i*15),1332+(i*12),1314+(i*8),1301+(i*5),2021+(i%4));}
ps("\nVellereophyton evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vell_evaluation(t,c,1335+(i*13),1324+(i*10),1308+(i*7),1297+(i*4),2022+(i%3));}
ps("\nVellereophyton accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vell_accessory(t,c,1327+(i*11),1318+(i*9),1304+(i*6),1294+(i*3),2023+(i%2));}
ps("\nVellereophyton marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vell_market(t,c,1321+(i*9),1312+(i*7),1299+(i*5),1291+(i*3),2024);}
ps("\n");vell_report();vell_state();ps("\n=== Demo Complete ===\n");return 0;}
