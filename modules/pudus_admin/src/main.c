/* pudus_admin: Pudus management technology administration (v1.0)
 * Pudus planning, pudus execution, pudus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pud_t;
typedef struct{int n_pudp,n_pude,n_pud2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pud_state_t;
static pud_t puds[N],pudess[N-2],pudvss[N-4],pudas[N-6],pudmks[N-6]; static pud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(pud_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pud_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PUD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pud_init(void){if(init)return -1;st.n_pudp=0;st.n_pude=0;st.n_pud2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)puds[i].active=0;for(int i=0;i<N-2;i++)pudess[i].active=0;for(int i=0;i<N-4;i++)pudvss[i].active=0;for(int i=0;i<N-6;i++)pudas[i].active=0;for(int i=0;i<N-6;i++)pudmks[i].active=0;init=1;ps("[PUD] Pudus initialized\n");return 0;}
int pud_planning(int t,int c,int a,int b,int d,int e,int y){return add(puds,&st.n_pudp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pud_execution(int t,int c,int a,int b,int d,int e,int y){return add(pudess,&st.n_pude,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pud_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pudvss,&st.n_pud2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pud_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pudas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pud_market(int t,int c,int a,int b,int d,int e,int y){return add(pudmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pud_report(void){ps("[PUD] Pudp: ");pi(st.n_pudp);ps(" PCS=");pi(st.t_f1);ps("\nPude: ");pi(st.n_pude);ps(" PCS=");pi(st.t_f2);ps("\nPudv: ");pi(st.n_pud2);ps(" PCS=");pi(st.t_f3);ps("\nPudc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pud_state(void){ps("[PUD] Pudp=");pi(st.n_pudp);ps(" Pude=");pi(st.n_pude);ps(" Pudv=");pi(st.n_pud2);ps(" Pudc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pudus Admin Demo ===\n\n");pud_init();
ps("Pudus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pud_planning(t,c,1333+(i*17),1322+(i*14),1302+(i*10),1284+(i*6),2020+(i%5));}
ps("\nPudus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pud_execution(t,c,1322+(i*15),1311+(i*12),1293+(i*8),1280+(i*5),2021+(i%4));}
ps("\nPudus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pud_evaluation(t,c,1314+(i*13),1303+(i*10),1287+(i*7),1276+(i*4),2022+(i%3));}
ps("\nPudus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pud_accessory(t,c,1306+(i*11),1297+(i*9),1283+(i*6),1273+(i*3),2023+(i%2));}
ps("\nPudus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pud_market(t,c,1300+(i*9),1291+(i*7),1278+(i*5),1270+(i*3),2024);}
ps("\n");pud_report();pud_state();ps("\n=== Demo Complete ===\n");return 0;}
