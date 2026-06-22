/* luffa_admin: Luffa management technology administration (v1.0)
 * Luffa planning, luffa execution, luffa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} luf_t;
typedef struct{int n_lufp,n_lufe,n_luf2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} luf_state_t;
static luf_t lufps[N],lufess[N-2],lufvss[N-4],lufas[N-6],lufmks[N-6]; static luf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(luf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;luf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int luf_init(void){if(init)return -1;st.n_lufp=0;st.n_lufe=0;st.n_luf2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lufps[i].active=0;for(int i=0;i<N-2;i++)lufess[i].active=0;for(int i=0;i<N-4;i++)lufvss[i].active=0;for(int i=0;i<N-6;i++)lufas[i].active=0;for(int i=0;i<N-6;i++)lufmks[i].active=0;init=1;ps("[LUF] Luffa initialized\n");return 0;}
int luf_planning(int t,int c,int a,int b,int d,int e,int y){return add(lufps,&st.n_lufp,&st.t_f1,N,t,c,a,b,d,e,y);}
int luf_execution(int t,int c,int a,int b,int d,int e,int y){return add(lufess,&st.n_lufe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int luf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lufvss,&st.n_luf2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int luf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lufas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int luf_market(int t,int c,int a,int b,int d,int e,int y){return add(lufmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void luf_report(void){ps("[LUF] Lufp: ");pi(st.n_lufp);ps(" PCS=");pi(st.t_f1);ps("\nLufe: ");pi(st.n_lufe);ps(" PCS=");pi(st.t_f2);ps("\nLufv: ");pi(st.n_luf2);ps(" PCS=");pi(st.t_f3);ps("\nLufc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void luf_state(void){ps("[LUF] Lufp=");pi(st.n_lufp);ps(" Lufe=");pi(st.n_lufe);ps(" Lufv=");pi(st.n_luf2);ps(" Lufc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Luffa Admin Demo ===\n\n");luf_init();
ps("Luffa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;luf_planning(t,c,1311+(i*17),1300+(i*14),1280+(i*10),1262+(i*6),2020+(i%5));}
ps("\nLuffa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;luf_execution(t,c,1300+(i*15),1289+(i*12),1271+(i*8),1258+(i*5),2021+(i%4));}
ps("\nLuffa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;luf_evaluation(t,c,1292+(i*13),1281+(i*10),1265+(i*7),1254+(i*4),2022+(i%3));}
ps("\nLuffa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;luf_accessory(t,c,1284+(i*11),1275+(i*9),1261+(i*6),1251+(i*3),2023+(i%2));}
ps("\nLuffa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;luf_market(t,c,1278+(i*9),1269+(i*7),1256+(i*5),1248+(i*3),2024);}
ps("\n");luf_report();luf_state();ps("\n=== Demo Complete ===\n");return 0;}
