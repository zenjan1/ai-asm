/* aviatech_admin: Aviation technology administration (v1.0)
 * Aircraft design, aero engine, avionics, aviation materials, flight training
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} avi_t;
typedef struct{int n_ds,n_eg,n_av,n_mt,n_tr,t_f1,t_f2,t_f3,t_f4,t_f5;} avi_state_t;
static avi_t dss[N],egs[N-2],avs[N-4],mts[N-6],trs[N-6]; static avi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(avi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;avi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AVI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int avi_init(void){if(init)return -1;st.n_ds=0;st.n_eg=0;st.n_av=0;st.n_mt=0;st.n_tr=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dss[i].active=0;for(int i=0;i<N-2;i++)egs[i].active=0;for(int i=0;i<N-4;i++)avs[i].active=0;for(int i=0;i<N-6;i++)mts[i].active=0;for(int i=0;i<N-6;i++)trs[i].active=0;init=1;ps("[AVI] Aviatech initialized\n");return 0;}
int avi_design(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f1,N,t,c,a,b,d,e,y);}
int avi_engine(int t,int c,int a,int b,int d,int e,int y){return add(egs,&st.n_eg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int avi_avionics(int t,int c,int a,int b,int d,int e,int y){return add(avs,&st.n_av,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int avi_material(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int avi_training(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void avi_report(void){ps("[AVI] Design: ");pi(st.n_ds);ps(" CFD=");pi(st.t_f1);ps("\nEngine: ");pi(st.n_eg);ps(" Thrust=");pi(st.t_f2);ps("\nAvion: ");pi(st.n_av);ps(" FCS=");pi(st.t_f3);ps("\nMat: ");pi(st.n_mt);ps(" Comp=");pi(st.t_f4);ps("\nTrain: ");pi(st.n_tr);ps(" Sim=");pi(st.t_f5);ps("\n");}
void avi_state(void){ps("[AVI] Ds=");pi(st.n_ds);ps(" Eg=");pi(st.n_eg);ps(" Av=");pi(st.n_av);ps(" Mt=");pi(st.n_mt);ps(" Tr=");pi(st.n_tr);ps("\n");}
int main(void){
ps("=== Aviation Tech Admin Demo ===\n\n");avi_init();
ps("Aircraft design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;avi_design(t,c,158+(i*17),143+(i*14),123+(i*10),106+(i*6),2020+(i%5));}
ps("\nAero engine...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;avi_engine(t,c,147+(i*15),133+(i*12),115+(i*8),102+(i*5),2021+(i%4));}
ps("\nAvionics...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;avi_avionics(t,c,139+(i*13),125+(i*10),109+(i*7),98+(i*4),2022+(i%3));}
ps("\nAviation materials...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;avi_material(t,c,131+(i*11),119+(i*9),105+(i*6),95+(i*3),2023+(i%2));}
ps("\nFlight training...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;avi_training(t,c,125+(i*9),114+(i*7),101+(i*5),93+(i*3),2024);}
ps("\n");avi_report();avi_state();ps("\n=== Demo Complete ===\n");return 0;}
