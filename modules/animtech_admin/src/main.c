/* animtech_admin: Animation technology administration (v1.0)
 * 2D animation, 3D animation, animation film and TV, animation derivatives, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ant_t;
typedef struct{int n_2d,n_3d,n_at,n_ad,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ant_state_t;
static ant_t twods[N],thds[N-2],ats[N-4],ads[N-6],mks[N-6]; static ant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ant_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ant_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ant_init(void){if(init)return -1;st.n_2d=0;st.n_3d=0;st.n_at=0;st.n_ad=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)twods[i].active=0;for(int i=0;i<N-2;i++)thds[i].active=0;for(int i=0;i<N-4;i++)ats[i].active=0;for(int i=0;i<N-6;i++)ads[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ANT] Animtech initialized\n");return 0;}
int ant_2d(int t,int c,int a,int b,int d,int e,int y){return add(twods,&st.n_2d,&st.t_f1,N,t,c,a,b,d,e,y);}
int ant_3d(int t,int c,int a,int b,int d,int e,int y){return add(thds,&st.n_3d,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ant_film(int t,int c,int a,int b,int d,int e,int y){return add(ats,&st.n_at,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ant_derivative(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ant_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ant_report(void){ps("[ANT] 2d: ");pi(st.n_2d);ps(" PCS=");pi(st.t_f1);ps("\n3d: ");pi(st.n_3d);ps(" PCS=");pi(st.t_f2);ps("\nAt: ");pi(st.n_at);ps(" PCS=");pi(st.t_f3);ps("\nAd: ");pi(st.n_ad);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ant_state(void){ps("[ANT] 2d=");pi(st.n_2d);ps(" 3d=");pi(st.n_3d);ps(" At=");pi(st.n_at);ps(" Ad=");pi(st.n_ad);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Animation Tech Admin Demo ===\n\n");ant_init();
ps("2D animation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ant_2d(t,c,236+(i*17),221+(i*14),201+(i*10),183+(i*6),2020+(i%5));}
ps("\n3D animation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ant_3d(t,c,225+(i*15),211+(i*12),193+(i*8),180+(i*5),2021+(i%4));}
ps("\nAnimation film and TV...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ant_film(t,c,217+(i*13),203+(i*10),187+(i*7),176+(i*4),2022+(i%3));}
ps("\nAnimation derivatives...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ant_derivative(t,c,209+(i*11),197+(i*9),183+(i*6),173+(i*3),2023+(i%2));}
ps("\nAnimation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ant_market(t,c,203+(i*9),192+(i*7),179+(i*5),171+(i*3),2024);}
ps("\n");ant_report();ant_state();ps("\n=== Demo Complete ===\n");return 0;}
