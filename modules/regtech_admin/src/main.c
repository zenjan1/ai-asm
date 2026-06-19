/* regtech_admin: Regtech administration (v1.0)
 * Regulatory reporting, compliance automation, reg change management, supervisory tech, reg data analytics
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rta_t;
typedef struct{int n_rr,n_ca,n_rc,n_st,n_da,t_f1,t_f2,t_f3,t_f4,t_f5;} rta_state_t;
static rta_t rrs[N],cas[N-2],rcs[N-4],sts[N-6],das[N-6]; static rta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rta_init(void){if(init)return -1;st.n_rr=0;st.n_ca=0;st.n_rc=0;st.n_st=0;st.n_da=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rrs[i].active=0;for(int i=0;i<N-2;i++)cas[i].active=0;for(int i=0;i<N-4;i++)rcs[i].active=0;for(int i=0;i<N-6;i++)sts[i].active=0;for(int i=0;i<N-6;i++)das[i].active=0;init=1;ps("[RTA] Regtech initialized\n");return 0;}
int rta_reporting(int t,int c,int a,int b,int d,int e,int y){return add(rrs,&st.n_rr,&st.t_f1,N,t,c,a,b,d,e,y);}
int rta_compliance(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rta_change(int t,int c,int a,int b,int d,int e,int y){return add(rcs,&st.n_rc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rta_supervise(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rta_analytics(int t,int c,int a,int b,int d,int e,int y){return add(das,&st.n_da,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rta_report(void){ps("[RTA] Rep: ");pi(st.n_rr);ps(" Auto=");pi(st.t_f1);ps("\nComp: ");pi(st.n_ca);ps(" Rule=");pi(st.t_f2);ps("\nChg: ");pi(st.n_rc);ps(" Track=");pi(st.t_f3);ps("\nSuper: ");pi(st.n_st);ps(" Mon=");pi(st.t_f4);ps("\nAnal: ");pi(st.n_da);ps(" BI=");pi(st.t_f5);ps("\n");}
void rta_state(void){ps("[RTA] Rr=");pi(st.n_rr);ps(" Ca=");pi(st.n_ca);ps(" Rc=");pi(st.n_rc);ps(" St=");pi(st.n_st);ps(" Da=");pi(st.n_da);ps("\n");}
int main(void){
ps("=== Regtech Admin Demo ===\n\n");rta_init();
ps("Regulatory reporting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rta_reporting(t,c,135+(i*17),120+(i*14),100+(i*10),83+(i*6),2020+(i%5));}
ps("\nCompliance automation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rta_compliance(t,c,124+(i*15),110+(i*12),92+(i*8),79+(i*5),2021+(i%4));}
ps("\nReg change management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rta_change(t,c,116+(i*13),102+(i*10),86+(i*7),75+(i*4),2022+(i%3));}
ps("\nSupervisory tech...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rta_supervise(t,c,108+(i*11),96+(i*9),82+(i*6),72+(i*3),2023+(i%2));}
ps("\nReg data analytics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rta_analytics(t,c,102+(i*9),91+(i*7),78+(i*5),70+(i*3),2024);}
ps("\n");rta_report();rta_state();ps("\n=== Demo Complete ===\n");return 0;}
