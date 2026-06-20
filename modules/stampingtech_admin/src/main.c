/* stampingtech_admin: Stamping technology administration (v1.0)
 * Blanking, forming, precision stamping, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} stp_t;
typedef struct{int n_bl,n_fm,n_ps,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} stp_state_t;
static stp_t bls[N],fms[N-2],pss[N-4],acs[N-6],mks[N-6]; static stp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(stp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;stp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int stp_init(void){if(init)return -1;st.n_bl=0;st.n_fm=0;st.n_ps=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bls[i].active=0;for(int i=0;i<N-2;i++)fms[i].active=0;for(int i=0;i<N-4;i++)pss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[STP] Stampingtech initialized\n");return 0;}
int stp_blank(int t,int c,int a,int b,int d,int e,int y){return add(bls,&st.n_bl,&st.t_f1,N,t,c,a,b,d,e,y);}
int stp_form(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int stp_precision(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int stp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int stp_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void stp_report(void){ps("[STP] Bl: ");pi(st.n_bl);ps(" PCS=");pi(st.t_f1);ps("\nFm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f2);ps("\nPs: ");pi(st.n_ps);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void stp_state(void){ps("[STP] Bl=");pi(st.n_bl);ps(" Fm=");pi(st.n_fm);ps(" Ps=");pi(st.n_ps);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Stamping Tech Admin Demo ===\n\n");stp_init();
ps("Blanking...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;stp_blank(t,c,276+(i*17),261+(i*14),241+(i*10),223+(i*6),2020+(i%5));}
ps("\nForming...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;stp_form(t,c,265+(i*15),251+(i*12),233+(i*8),220+(i*5),2021+(i%4));}
ps("\nPrecision stamping...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;stp_precision(t,c,257+(i*13),243+(i*10),227+(i*7),216+(i*4),2022+(i%3));}
ps("\nStamping accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;stp_accessory(t,c,249+(i*11),237+(i*9),223+(i*6),213+(i*3),2023+(i%2));}
ps("\nStamping marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;stp_market(t,c,243+(i*9),232+(i*7),219+(i*5),211+(i*3),2024);}
ps("\n");stp_report();stp_state();ps("\n=== Demo Complete ===\n");return 0;}
