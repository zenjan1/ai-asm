/* justicia_admin: Justicia management technology administration (v1.0)
 * Justicia planning, justicia execution, justicia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} just_t;
typedef struct{int n_justp,n_just,n_just2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} just_state_t;
static just_t justs[N],juste[N-2],just2[N-4],justac[N-6],justam[N-6]; static just_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(just_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;just_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int just_init(void){if(init)return -1;st.n_justp=0;st.n_just=0;st.n_just2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)justs[i].active=0;for(int i=0;i<N-2;i++)juste[i].active=0;for(int i=0;i<N-4;i++)just2[i].active=0;for(int i=0;i<N-6;i++)justac[i].active=0;for(int i=0;i<N-6;i++)justam[i].active=0;init=1;ps("[JUST] Justicia initialized\n");return 0;}
int just_planning(int t,int c,int a,int b,int d,int e,int y){return add(justs,&st.n_justp,&st.t_f1,N,t,c,a,b,d,e,y);}
int just_execution(int t,int c,int a,int b,int d,int e,int y){return add(juste,&st.n_just,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int just_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(just2,&st.n_just2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int just_accessory(int t,int c,int a,int b,int d,int e,int y){return add(justac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int just_market(int t,int c,int a,int b,int d,int e,int y){return add(justam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void just_report(void){ps("[JUST] Justp: ");pi(st.n_justp);ps(" PCS=");pi(st.t_f1);ps("\nJuste: ");pi(st.n_just);ps(" PCS=");pi(st.t_f2);ps("\nJust2: ");pi(st.n_just2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void just_state(void){ps("[JUST] Justp=");pi(st.n_justp);ps(" Just=");pi(st.n_just);ps(" Just2=");pi(st.n_just2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Justicia Admin Demo ===\n\n");just_init();
ps("Justicia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;just_planning(t,c,1548+(i*17),1537+(i*14),1517+(i*10),1499+(i*6),2020+(i%5));}
ps("\nJusticia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;just_execution(t,c,1537+(i*15),1526+(i*12),1508+(i*8),1495+(i*5),2021+(i%4));}
ps("\nJusticia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;just_evaluation(t,c,1529+(i*13),1518+(i*10),1502+(i*7),1491+(i*4),2022+(i%3));}
ps("\nJusticia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;just_accessory(t,c,1521+(i*11),1512+(i*9),1498+(i*6),1488+(i*3),2023+(i%2));}
ps("\nJusticia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;just_market(t,c,1515+(i*9),1506+(i*7),1493+(i*5),1485+(i*3),2024);}
ps("\n");just_report();just_state();ps("\n=== Demo Complete ===\n");return 0;}
