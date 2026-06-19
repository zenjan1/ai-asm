/* edtech_admin: Edtech administration (v1.0)
 * Online education, smart tutoring, education platform, education assessment, campus management
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} edt_t;
typedef struct{int n_ol,n_st,n_ep,n_ea,n_cm,t_f1,t_f2,t_f3,t_f4,t_f5;} edt_state_t;
static edt_t ols[N],sts[N-2],eps[N-4],eas[N-6],cms[N-6]; static edt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(edt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;edt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int edt_init(void){if(init)return -1;st.n_ol=0;st.n_st=0;st.n_ep=0;st.n_ea=0;st.n_cm=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ols[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)eps[i].active=0;for(int i=0;i<N-6;i++)eas[i].active=0;for(int i=0;i<N-6;i++)cms[i].active=0;init=1;ps("[EDT] Edtech initialized\n");return 0;}
int edt_online(int t,int c,int a,int b,int d,int e,int y){return add(ols,&st.n_ol,&st.t_f1,N,t,c,a,b,d,e,y);}
int edt_smart(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int edt_platform(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int edt_assess(int t,int c,int a,int b,int d,int e,int y){return add(eas,&st.n_ea,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int edt_campus(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void edt_report(void){ps("[EDT] Online: ");pi(st.n_ol);ps(" MOOC=");pi(st.t_f1);ps("\nSmart: ");pi(st.n_st);ps(" AI=");pi(st.t_f2);ps("\nPlat: ");pi(st.n_ep);ps(" K12=");pi(st.t_f3);ps("\nAssess: ");pi(st.n_ea);ps(" Cert=");pi(st.t_f4);ps("\nCampus: ");pi(st.n_cm);ps(" SIS=");pi(st.t_f5);ps("\n");}
void edt_state(void){ps("[EDT] Ol=");pi(st.n_ol);ps(" St=");pi(st.n_st);ps(" Ep=");pi(st.n_ep);ps(" Ea=");pi(st.n_ea);ps(" Cm=");pi(st.n_cm);ps("\n");}
int main(void){
ps("=== Edtech Admin Demo ===\n\n");edt_init();
ps("Online education...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;edt_online(t,c,139+(i*17),124+(i*14),104+(i*10),87+(i*6),2020+(i%5));}
ps("\nSmart tutoring...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;edt_smart(t,c,128+(i*15),114+(i*12),96+(i*8),83+(i*5),2021+(i%4));}
ps("\nEducation platform...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;edt_platform(t,c,120+(i*13),106+(i*10),90+(i*7),79+(i*4),2022+(i%3));}
ps("\nEducation assessment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edt_assess(t,c,112+(i*11),100+(i*9),86+(i*6),76+(i*3),2023+(i%2));}
ps("\nCampus management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edt_campus(t,c,106+(i*9),95+(i*7),82+(i*5),74+(i*3),2024);}
ps("\n");edt_report();edt_state();ps("\n=== Demo Complete ===\n");return 0;}
