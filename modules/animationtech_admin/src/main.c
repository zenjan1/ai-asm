/* animationtech_admin: Animation technology administration (v1.0)
 * Animation production, comic creation, IP operation, derivative products, virtual idol
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ant_t;
typedef struct{int n_an,n_co,n_ip,n_dp,n_vi,t_f1,t_f2,t_f3,t_f4,t_f5;} ant_state_t;
static ant_t ans[N],cos[N-2],ips[N-4],dps[N-6],vis[N-6]; static ant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ant_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ant_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ant_init(void){if(init)return -1;st.n_an=0;st.n_co=0;st.n_ip=0;st.n_dp=0;st.n_vi=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ans[i].active=0;for(int i=0;i<N-2;i++)cos[i].active=0;for(int i=0;i<N-4;i++)ips[i].active=0;for(int i=0;i<N-6;i++)dps[i].active=0;for(int i=0;i<N-6;i++)vis[i].active=0;init=1;ps("[ANT] Animationtech initialized\n");return 0;}
int ant_anim(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f1,N,t,c,a,b,d,e,y);}
int ant_comic(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ant_ip(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ant_deriv(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ant_virtual(int t,int c,int a,int b,int d,int e,int y){return add(vis,&st.n_vi,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ant_report(void){ps("[ANT] Anim: ");pi(st.n_an);ps(" FPS=");pi(st.t_f1);ps("\nComic: ");pi(st.n_co);ps(" Page=");pi(st.t_f2);ps("\nIP: ");pi(st.n_ip);ps(" Lic=");pi(st.t_f3);ps("\nDeriv: ");pi(st.n_dp);ps(" SKU=");pi(st.t_f4);ps("\nVIdol: ");pi(st.n_vi);ps(" Fan=");pi(st.t_f5);ps("\n");}
void ant_state(void){ps("[ANT] An=");pi(st.n_an);ps(" Co=");pi(st.n_co);ps(" Ip=");pi(st.n_ip);ps(" Dp=");pi(st.n_dp);ps(" Vi=");pi(st.n_vi);ps("\n");}
int main(void){
ps("=== Animation Tech Admin Demo ===\n\n");ant_init();
ps("Animation production...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ant_anim(t,c,170+(i*17),155+(i*14),135+(i*10),118+(i*6),2020+(i%5));}
ps("\nComic creation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ant_comic(t,c,159+(i*15),145+(i*12),127+(i*8),114+(i*5),2021+(i%4));}
ps("\nIP operation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ant_ip(t,c,151+(i*13),137+(i*10),121+(i*7),110+(i*4),2022+(i%3));}
ps("\nDerivative products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ant_deriv(t,c,143+(i*11),131+(i*9),117+(i*6),107+(i*3),2023+(i%2));}
ps("\nVirtual idol...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ant_virtual(t,c,137+(i*9),126+(i*7),113+(i*5),105+(i*3),2024);}
ps("\n");ant_report();ant_state();ps("\n=== Demo Complete ===\n");return 0;}
