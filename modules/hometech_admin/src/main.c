/* hometech_admin: Smart home technology administration (v1.0)
 * Smart lighting, smart security, smart appliances, smart control, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hmt_t;
typedef struct{int n_lt,n_sc,n_ap,n_ct,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hmt_state_t;
static hmt_t lts[N],scs[N-2],aps[N-4],cts[N-6],mks[N-6]; static hmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hmt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hmt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HMT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hmt_init(void){if(init)return -1;st.n_lt=0;st.n_sc=0;st.n_ap=0;st.n_ct=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lts[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)aps[i].active=0;for(int i=0;i<N-6;i++)cts[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HMT] Hometech initialized\n");return 0;}
int hmt_lighting(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f1,N,t,c,a,b,d,e,y);}
int hmt_security(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hmt_appliance(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hmt_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hmt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hmt_report(void){ps("[HMT] Lt: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f1);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f2);ps("\nAp: ");pi(st.n_ap);ps(" PCS=");pi(st.t_f3);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hmt_state(void){ps("[HMT] Lt=");pi(st.n_lt);ps(" Sc=");pi(st.n_sc);ps(" Ap=");pi(st.n_ap);ps(" Ct=");pi(st.n_ct);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Smart Home Tech Admin Demo ===\n\n");hmt_init();
ps("Smart lighting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hmt_lighting(t,c,252+(i*17),237+(i*14),217+(i*10),199+(i*6),2020+(i%5));}
ps("\nSmart security...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hmt_security(t,c,241+(i*15),227+(i*12),209+(i*8),196+(i*5),2021+(i%4));}
ps("\nSmart appliances...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hmt_appliance(t,c,233+(i*13),219+(i*10),203+(i*7),192+(i*4),2022+(i%3));}
ps("\nSmart control...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hmt_control(t,c,225+(i*11),213+(i*9),199+(i*6),189+(i*3),2023+(i%2));}
ps("\nSmart home marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hmt_market(t,c,219+(i*9),208+(i*7),195+(i*5),187+(i*3),2024);}
ps("\n");hmt_report();hmt_state();ps("\n=== Demo Complete ===\n");return 0;}
