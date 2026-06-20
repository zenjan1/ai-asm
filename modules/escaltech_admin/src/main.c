/* escaltech_admin: Escalator technology administration (v1.0)
 * Automatic escalators, moving walks, accessories, installation/maintenance, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} esc_t;
typedef struct{int n_es,n_mw,n_ac,n_im,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} esc_state_t;
static esc_t ess[N],mws[N-2],acs[N-4],ims[N-6],mks[N-6]; static esc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(esc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;esc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ESC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int esc_init(void){if(init)return -1;st.n_es=0;st.n_mw=0;st.n_ac=0;st.n_im=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ess[i].active=0;for(int i=0;i<N-2;i++)mws[i].active=0;for(int i=0;i<N-4;i++)acs[i].active=0;for(int i=0;i<N-6;i++)ims[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ESC] Escaltech initialized\n");return 0;}
int esc_escalator(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f1,N,t,c,a,b,d,e,y);}
int esc_walkway(int t,int c,int a,int b,int d,int e,int y){return add(mws,&st.n_mw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int esc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int esc_install(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int esc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void esc_report(void){ps("[ESC] Es: ");pi(st.n_es);ps(" PCS=");pi(st.t_f1);ps("\nMw: ");pi(st.n_mw);ps(" PCS=");pi(st.t_f2);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f3);ps("\nIm: ");pi(st.n_im);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void esc_state(void){ps("[ESC] Es=");pi(st.n_es);ps(" Mw=");pi(st.n_mw);ps(" Ac=");pi(st.n_ac);ps(" Im=");pi(st.n_im);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Escalator Tech Admin Demo ===\n\n");esc_init();
ps("Automatic escalators...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;esc_escalator(t,c,251+(i*17),236+(i*14),216+(i*10),198+(i*6),2020+(i%5));}
ps("\nMoving walks...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;esc_walkway(t,c,240+(i*15),226+(i*12),208+(i*8),195+(i*5),2021+(i%4));}
ps("\nEscalator accessories...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;esc_accessory(t,c,232+(i*13),218+(i*10),202+(i*7),191+(i*4),2022+(i%3));}
ps("\nInstallation & maintenance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;esc_install(t,c,224+(i*11),212+(i*9),198+(i*6),188+(i*3),2023+(i%2));}
ps("\nEscalator marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;esc_market(t,c,218+(i*9),207+(i*7),194+(i*5),186+(i*3),2024);}
ps("\n");esc_report();esc_state();ps("\n=== Demo Complete ===\n");return 0;}
