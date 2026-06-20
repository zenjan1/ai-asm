/* ndt_admin: Non-destructive testing technology administration (v1.0)
 * Ultrasonic testing, radiographic testing, surface testing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ndt_t;
typedef struct{int n_ut,n_rt,n_st,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ndt_state_t;
static ndt_t uts[N],rts[N-2],sts[N-4],acs[N-6],mks[N-6]; static ndt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ndt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ndt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ndt_init(void){if(init)return -1;st.n_ut=0;st.n_rt=0;st.n_st=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uts[i].active=0;for(int i=0;i<N-2;i++)rts[i].active=0;for(int i=0;i<N-4;i++)sts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NDT] NDT initialized\n");return 0;}
int ndt_ultrasonic(int t,int c,int a,int b,int d,int e,int y){return add(uts,&st.n_ut,&st.t_f1,N,t,c,a,b,d,e,y);}
int ndt_radiographic(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ndt_surface(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ndt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ndt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ndt_report(void){ps("[NDT] Ut: ");pi(st.n_ut);ps(" PCS=");pi(st.t_f1);ps("\nRt: ");pi(st.n_rt);ps(" PCS=");pi(st.t_f2);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ndt_state(void){ps("[NDT] Ut=");pi(st.n_ut);ps(" Rt=");pi(st.n_rt);ps(" St=");pi(st.n_st);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== NDT Admin Demo ===\n\n");ndt_init();
ps("Ultrasonic testing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ndt_ultrasonic(t,c,421+(i*17),410+(i*14),390+(i*10),372+(i*6),2020+(i%5));}
ps("\nRadiographic testing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ndt_radiographic(t,c,410+(i*15),399+(i*12),381+(i*8),368+(i*5),2021+(i%4));}
ps("\nSurface testing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ndt_surface(t,c,402+(i*13),391+(i*10),375+(i*7),364+(i*4),2022+(i%3));}
ps("\nNDT accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ndt_accessory(t,c,394+(i*11),385+(i*9),371+(i*6),361+(i*3),2023+(i%2));}
ps("\nNDT marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ndt_market(t,c,388+(i*9),379+(i*7),366+(i*5),358+(i*3),2024);}
ps("\n");ndt_report();ndt_state();ps("\n=== Demo Complete ===\n");return 0;}
