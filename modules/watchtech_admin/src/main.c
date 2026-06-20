/* watchtech_admin: Watch technology administration (v1.0)
 * Mechanical watches, quartz watches, smartwatches, wall clocks, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wat_t;
typedef struct{int n_mw,n_qw,n_sw,n_wc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wat_state_t;
static wat_t mws[N],qws[N-2],sws[N-4],wcs[N-6],mks[N-6]; static wat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wat_init(void){if(init)return -1;st.n_mw=0;st.n_qw=0;st.n_sw=0;st.n_wc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mws[i].active=0;for(int i=0;i<N-2;i++)qws[i].active=0;for(int i=0;i<N-4;i++)sws[i].active=0;for(int i=0;i<N-6;i++)wcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WAT] Watchtech initialized\n");return 0;}
int wat_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mws,&st.n_mw,&st.t_f1,N,t,c,a,b,d,e,y);}
int wat_quartz(int t,int c,int a,int b,int d,int e,int y){return add(qws,&st.n_qw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wat_smart(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wat_wall(int t,int c,int a,int b,int d,int e,int y){return add(wcs,&st.n_wc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wat_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wat_report(void){ps("[WAT] Mech: ");pi(st.n_mw);ps(" PCS=");pi(st.t_f1);ps("\nQtz: ");pi(st.n_qw);ps(" PCS=");pi(st.t_f2);ps("\nSmart: ");pi(st.n_sw);ps(" PCS=");pi(st.t_f3);ps("\nWall: ");pi(st.n_wc);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wat_state(void){ps("[WAT] Mw=");pi(st.n_mw);ps(" Qw=");pi(st.n_qw);ps(" Sw=");pi(st.n_sw);ps(" Wc=");pi(st.n_wc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Watch Tech Admin Demo ===\n\n");wat_init();
ps("Mechanical watches...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wat_mechanical(t,c,202+(i*17),187+(i*14),167+(i*10),149+(i*6),2020+(i%5));}
ps("\nQuartz watches...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wat_quartz(t,c,191+(i*15),177+(i*12),159+(i*8),146+(i*5),2021+(i%4));}
ps("\nSmartwatches...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wat_smart(t,c,183+(i*13),169+(i*10),153+(i*7),142+(i*4),2022+(i%3));}
ps("\nWall clocks...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wat_wall(t,c,175+(i*11),163+(i*9),149+(i*6),139+(i*3),2023+(i%2));}
ps("\nWatch marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wat_market(t,c,169+(i*9),158+(i*7),145+(i*5),137+(i*3),2024);}
ps("\n");wat_report();wat_state();ps("\n=== Demo Complete ===\n");return 0;}
