/* victoria_admin: Victoria management technology administration (v1.0)
 * Victoria planning, victoria execution, victoria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vict_t;
typedef struct{int n_victp,n_vict,n_vict2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vict_state_t;
static vict_t victs[N],victess[N-2],victvss[N-4],victas[N-6],victmks[N-6]; static vict_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vict_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vict_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VICT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vict_init(void){if(init)return -1;st.n_victp=0;st.n_vict=0;st.n_vict2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)victs[i].active=0;for(int i=0;i<N-2;i++)victess[i].active=0;for(int i=0;i<N-4;i++)victvss[i].active=0;for(int i=0;i<N-6;i++)victas[i].active=0;for(int i=0;i<N-6;i++)victmks[i].active=0;init=1;ps("[VICT] Victoria initialized\n");return 0;}
int vict_planning(int t,int c,int a,int b,int d,int e,int y){return add(victs,&st.n_victp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vict_execution(int t,int c,int a,int b,int d,int e,int y){return add(victess,&st.n_vict,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vict_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(victvss,&st.n_vict2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vict_accessory(int t,int c,int a,int b,int d,int e,int y){return add(victas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vict_market(int t,int c,int a,int b,int d,int e,int y){return add(victmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vict_report(void){ps("[VICT] Victp: ");pi(st.n_victp);ps(" PCS=");pi(st.t_f1);ps("\nVicte: ");pi(st.n_vict);ps(" PCS=");pi(st.t_f2);ps("\nVicv: ");pi(st.n_vict2);ps(" PCS=");pi(st.t_f3);ps("\nVic2: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vict_state(void){ps("[VICT] Victp=");pi(st.n_victp);ps(" Victe=");pi(st.n_vict);ps(" Vicv=");pi(st.n_vict2);ps(" Vic2=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Victoria Admin Demo ===\n\n");vict_init();
ps("Victoria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vict_planning(t,c,1359+(i*17),1348+(i*14),1328+(i*10),1310+(i*6),2020+(i%5));}
ps("\nVictoria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vict_execution(t,c,1348+(i*15),1337+(i*12),1319+(i*8),1306+(i*5),2021+(i%4));}
ps("\nVictoria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vict_evaluation(t,c,1340+(i*13),1329+(i*10),1313+(i*7),1302+(i*4),2022+(i%3));}
ps("\nVictoria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vict_accessory(t,c,1332+(i*11),1323+(i*9),1309+(i*6),1299+(i*3),2023+(i%2));}
ps("\nVictoria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vict_market(t,c,1326+(i*9),1317+(i*7),1304+(i*5),1296+(i*3),2024);}
ps("\n");vict_report();vict_state();ps("\n=== Demo Complete ===\n");return 0;}
