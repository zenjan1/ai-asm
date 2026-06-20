/* cuttech_admin: Cutting technology administration (v1.0)
 * Turning, milling, drilling, cutting tools, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctt_t;
typedef struct{int n_tn,n_ml,n_dr,n_tl,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctt_state_t;
static ctt_t tns[N],mls[N-2],drs[N-4],tls[N-6],mks[N-6]; static ctt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctt_init(void){if(init)return -1;st.n_tn=0;st.n_ml=0;st.n_dr=0;st.n_tl=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tns[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)drs[i].active=0;for(int i=0;i<N-6;i++)tls[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTT] Cuttech initialized\n");return 0;}
int ctt_turning(int t,int c,int a,int b,int d,int e,int y){return add(tns,&st.n_tn,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctt_milling(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctt_drilling(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctt_tool(int t,int c,int a,int b,int d,int e,int y){return add(tls,&st.n_tl,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctt_report(void){ps("[CTT] Tn: ");pi(st.n_tn);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nDr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f3);ps("\nTl: ");pi(st.n_tl);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctt_state(void){ps("[CTT] Tn=");pi(st.n_tn);ps(" Ml=");pi(st.n_ml);ps(" Dr=");pi(st.n_dr);ps(" Tl=");pi(st.n_tl);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cutting Tech Admin Demo ===\n\n");ctt_init();
ps("Turning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctt_turning(t,c,266+(i*17),251+(i*14),231+(i*10),213+(i*6),2020+(i%5));}
ps("\nMilling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctt_milling(t,c,255+(i*15),241+(i*12),223+(i*8),210+(i*5),2021+(i%4));}
ps("\nDrilling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctt_drilling(t,c,247+(i*13),233+(i*10),217+(i*7),206+(i*4),2022+(i%3));}
ps("\nCutting tools...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctt_tool(t,c,239+(i*11),227+(i*9),213+(i*6),203+(i*3),2023+(i%2));}
ps("\nCutting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctt_market(t,c,233+(i*9),222+(i*7),209+(i*5),201+(i*3),2024);}
ps("\n");ctt_report();ctt_state();ps("\n=== Demo Complete ===\n");return 0;}
