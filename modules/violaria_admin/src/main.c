/* violaria_admin: Violaria management technology administration (v1.0)
 * Violaria planning, violaria execution, violaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} viol_t;
typedef struct{int n_violp,n_viol,n_viol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} viol_state_t;
static viol_t viols[N],violess[N-2],violcss[N-4],violas[N-6],violmks[N-6]; static viol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(viol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;viol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VIOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int viol_init(void){if(init)return -1;st.n_violp=0;st.n_viol=0;st.n_viol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)viols[i].active=0;for(int i=0;i<N-2;i++)violess[i].active=0;for(int i=0;i<N-4;i++)violcss[i].active=0;for(int i=0;i<N-6;i++)violas[i].active=0;for(int i=0;i<N-6;i++)violmks[i].active=0;init=1;ps("[VIOL] Violaria initialized\n");return 0;}
int viol_planning(int t,int c,int a,int b,int d,int e,int y){return add(viols,&st.n_violp,&st.t_f1,N,t,c,a,b,d,e,y);}
int viol_execution(int t,int c,int a,int b,int d,int e,int y){return add(violess,&st.n_viol,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int viol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(violcss,&st.n_viol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int viol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(violas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int viol_market(int t,int c,int a,int b,int d,int e,int y){return add(violmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void viol_report(void){ps("[VIOL] Violp: ");pi(st.n_violp);ps(" PCS=");pi(st.t_f1);ps("\nViole: ");pi(st.n_viol);ps(" PCS=");pi(st.t_f2);ps("\nViolv: ");pi(st.n_viol2);ps(" PCS=");pi(st.t_f3);ps("\nViolc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void viol_state(void){ps("[VIOL] Violp=");pi(st.n_violp);ps(" Viole=");pi(st.n_viol);ps(" Violv=");pi(st.n_viol2);ps(" Violc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Violaria Admin Demo ===\n\n");viol_init();
ps("Violaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;viol_planning(t,c,1362+(i*17),1351+(i*14),1331+(i*10),1313+(i*6),2020+(i%5));}
ps("\nViolaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;viol_execution(t,c,1351+(i*15),1340+(i*12),1322+(i*8),1309+(i*5),2021+(i%4));}
ps("\nViolaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;viol_evaluation(t,c,1343+(i*13),1332+(i*10),1316+(i*7),1305+(i*4),2022+(i%3));}
ps("\nViolaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;viol_accessory(t,c,1335+(i*11),1326+(i*9),1312+(i*6),1302+(i*3),2023+(i%2));}
ps("\nViolaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;viol_market(t,c,1329+(i*9),1320+(i*7),1307+(i*5),1299+(i*3),2024);}
ps("\n");viol_report();viol_state();ps("\n=== Demo Complete ===\n");return 0;}
