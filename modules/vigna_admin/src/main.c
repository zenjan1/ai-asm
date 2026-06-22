/* vigna_admin: Vigna management technology administration (v1.0)
 * Vigna planning, vigna execution, vigna evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vign_t;
typedef struct{int n_vignp,n_vign,n_vign2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vign_state_t;
static vign_t vigns[N],vigness[N-2],vignvss[N-4],vignas[N-6],vignmks[N-6]; static vign_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vign_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vign_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VIGN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vign_init(void){if(init)return -1;st.n_vignp=0;st.n_vign=0;st.n_vign2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vigns[i].active=0;for(int i=0;i<N-2;i++)vigness[i].active=0;for(int i=0;i<N-4;i++)vignvss[i].active=0;for(int i=0;i<N-6;i++)vignas[i].active=0;for(int i=0;i<N-6;i++)vignmks[i].active=0;init=1;ps("[VIGN] Vigna initialized\n");return 0;}
int vign_planning(int t,int c,int a,int b,int d,int e,int y){return add(vigns,&st.n_vignp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vign_execution(int t,int c,int a,int b,int d,int e,int y){return add(vigness,&st.n_vign,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vign_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vignvss,&st.n_vign2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vign_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vignas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vign_market(int t,int c,int a,int b,int d,int e,int y){return add(vignmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vign_report(void){ps("[VIGN] Vignp: ");pi(st.n_vignp);ps(" PCS=");pi(st.t_f1);ps("\nVigne: ");pi(st.n_vign);ps(" PCS=");pi(st.t_f2);ps("\nVignv: ");pi(st.n_vign2);ps(" PCS=");pi(st.t_f3);ps("\nVignc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vign_state(void){ps("[VIGN] Vignp=");pi(st.n_vignp);ps(" Vigne=");pi(st.n_vign);ps(" Vignv=");pi(st.n_vign2);ps(" Vignc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vigna Admin Demo ===\n\n");vign_init();
ps("Vigna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vign_planning(t,c,1360+(i*17),1349+(i*14),1329+(i*10),1311+(i*6),2020+(i%5));}
ps("\nVigna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vign_execution(t,c,1349+(i*15),1338+(i*12),1320+(i*8),1307+(i*5),2021+(i%4));}
ps("\nVigna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vign_evaluation(t,c,1341+(i*13),1330+(i*10),1314+(i*7),1303+(i*4),2022+(i%3));}
ps("\nVigna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vign_accessory(t,c,1333+(i*11),1324+(i*9),1310+(i*6),1300+(i*3),2023+(i%2));}
ps("\nVigna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vign_market(t,c,1327+(i*9),1318+(i*7),1305+(i*5),1297+(i*3),2024);}
ps("\n");vign_report();vign_state();ps("\n=== Demo Complete ===\n");return 0;}
