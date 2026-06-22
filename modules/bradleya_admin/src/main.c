/* bradleya_admin: Bradleya management technology administration (v1.0)
 * Bradleya planning, bradleya execution, bradleya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brad_t;
typedef struct{int n_bradp,n_brad,n_brad2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brad_state_t;
static brad_t brads[N],brade[N-2],brad2[N-4],bradac[N-6],bradam[N-6]; static brad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(brad_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brad_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRAD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brad_init(void){if(init)return -1;st.n_bradp=0;st.n_brad=0;st.n_brad2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brads[i].active=0;for(int i=0;i<N-2;i++)brade[i].active=0;for(int i=0;i<N-4;i++)brad2[i].active=0;for(int i=0;i<N-6;i++)bradac[i].active=0;for(int i=0;i<N-6;i++)bradam[i].active=0;init=1;ps("[BRAD] Bradleya initialized\n");return 0;}
int brad_planning(int t,int c,int a,int b,int d,int e,int y){return add(brads,&st.n_bradp,&st.t_f1,N,t,c,a,b,d,e,y);}
int brad_execution(int t,int c,int a,int b,int d,int e,int y){return add(brade,&st.n_brad,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brad_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(brad2,&st.n_brad2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brad_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bradac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brad_market(int t,int c,int a,int b,int d,int e,int y){return add(bradam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brad_report(void){ps("[BRAD] Rap: ");pi(st.n_bradp);ps(" PCS=");pi(st.t_f1);ps("\nRade: ");pi(st.n_brad);ps(" PCS=");pi(st.t_f2);ps("\nRad2: ");pi(st.n_brad2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brad_state(void){ps("[BRAD] Rap=");pi(st.n_bradp);ps(" Rad=");pi(st.n_brad);ps(" Rad2=");pi(st.n_brad2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bradleya Admin Demo ===\n\n");brad_init();
ps("Bradleya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brad_planning(t,c,1442+(i*17),1431+(i*14),1411+(i*10),1393+(i*6),2020+(i%5));}
ps("\nBradleya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brad_execution(t,c,1431+(i*15),1420+(i*12),1402+(i*8),1389+(i*5),2021+(i%4));}
ps("\nBradleya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brad_evaluation(t,c,1423+(i*13),1412+(i*10),1396+(i*7),1385+(i*4),2022+(i%3));}
ps("\nBradleya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brad_accessory(t,c,1415+(i*11),1406+(i*9),1392+(i*6),1382+(i*3),2023+(i%2));}
ps("\nBradleya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brad_market(t,c,1409+(i*9),1400+(i*7),1387+(i*5),1379+(i*3),2024);}
ps("\n");brad_report();brad_state();ps("\n=== Demo Complete ===\n");return 0;}
