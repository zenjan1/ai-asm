/* circular_admin: Circular management technology administration (v1.0)
 * Circular planning, circular building, circular evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crn_t;
typedef struct{int n_cp,n_cb,n_cv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crn_state_t;
static crn_t cps[N],cbs[N-2],cvs[N-4],acs[N-6],mks[N-6]; static crn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crn_init(void){if(init)return -1;st.n_cp=0;st.n_cb=0;st.n_cv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cps[i].active=0;for(int i=0;i<N-2;i++)cbs[i].active=0;for(int i=0;i<N-4;i++)cvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRN] Circular initialized\n");return 0;}
int crn_planning(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f1,N,t,c,a,b,d,e,y);}
int crn_building(int t,int c,int a,int b,int d,int e,int y){return add(cbs,&st.n_cb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crn_report(void){ps("[CRN] Cp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f1);ps("\nCb: ");pi(st.n_cb);ps(" PCS=");pi(st.t_f2);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crn_state(void){ps("[CRN] Cp=");pi(st.n_cp);ps(" Cb=");pi(st.n_cb);ps(" Cv=");pi(st.n_cv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Circular Admin Demo ===\n\n");crn_init();
ps("Circular planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crn_planning(t,c,488+(i*17),477+(i*14),457+(i*10),439+(i*6),2020+(i%5));}
ps("\nCircular building...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crn_building(t,c,477+(i*15),466+(i*12),448+(i*8),435+(i*5),2021+(i%4));}
ps("\nCircular evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crn_evaluation(t,c,469+(i*13),458+(i*10),442+(i*7),431+(i*4),2022+(i%3));}
ps("\nCircular accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crn_accessory(t,c,461+(i*11),452+(i*9),438+(i*6),428+(i*3),2023+(i%2));}
ps("\nCircular marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crn_market(t,c,455+(i*9),446+(i*7),433+(i*5),425+(i*3),2024);}
ps("\n");crn_report();crn_state();ps("\n=== Demo Complete ===\n");return 0;}
