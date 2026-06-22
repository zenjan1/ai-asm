/* clerodendrum_admin: Clerodendrum management technology administration (v1.0)
 * Clerodendrum planning, clerodendrum execution, clerodendrum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clr_t;
typedef struct{int n_clrp,n_clre,n_clr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} clr_state_t;
static clr_t clrps[N],clrss[N-2],clrvss[N-4],clras[N-6],clrmks[N-6]; static clr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clr_init(void){if(init)return -1;st.n_clrp=0;st.n_clre=0;st.n_clr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)clrps[i].active=0;for(int i=0;i<N-2;i++)clrss[i].active=0;for(int i=0;i<N-4;i++)clrvss[i].active=0;for(int i=0;i<N-6;i++)clras[i].active=0;for(int i=0;i<N-6;i++)clrmks[i].active=0;init=1;ps("[CLR] Clerodendrum initialized\n");return 0;}
int clr_planning(int t,int c,int a,int b,int d,int e,int y){return add(clrps,&st.n_clrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int clr_execution(int t,int c,int a,int b,int d,int e,int y){return add(clrss,&st.n_clre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(clrvss,&st.n_clr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(clras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clr_market(int t,int c,int a,int b,int d,int e,int y){return add(clrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clr_report(void){ps("[CLR] Clepp: ");pi(st.n_clrp);ps(" PCS=");pi(st.t_f1);ps("\nClee: ");pi(st.n_clre);ps(" PCS=");pi(st.t_f2);ps("\nClev: ");pi(st.n_clr2);ps(" PCS=");pi(st.t_f3);ps("\nClec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clr_state(void){ps("[CLR] Clepp=");pi(st.n_clrp);ps(" Clee=");pi(st.n_clre);ps(" Clev=");pi(st.n_clr2);ps(" Clec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clerodendrum Admin Demo ===\n\n");clr_init();
ps("Clerodendrum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clr_planning(t,c,1130+(i*17),1119+(i*14),1099+(i*10),1081+(i*6),2020+(i%5));}
ps("\nClerodendrum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clr_execution(t,c,1119+(i*15),1108+(i*12),1090+(i*8),1077+(i*5),2021+(i%4));}
ps("\nClerodendrum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clr_evaluation(t,c,1111+(i*13),1100+(i*10),1084+(i*7),1073+(i*4),2022+(i%3));}
ps("\nClerodendrum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clr_accessory(t,c,1103+(i*11),1094+(i*9),1080+(i*6),1070+(i*3),2023+(i%2));}
ps("\nClerodendrum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clr_market(t,c,1097+(i*9),1088+(i*7),1075+(i*5),1067+(i*3),2024);}
ps("\n");clr_report();clr_state();ps("\n=== Demo Complete ===\n");return 0;}
