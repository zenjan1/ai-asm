/* sinapis_admin: Sinapis management technology administration (v1.0)
 * Sinapis planning, sinapis execution, sinapis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sin_t;
typedef struct{int n_sinp,n_sine,n_sin2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sin_state_t;
static sin_t sinps[N],sinss[N-2],sinvss[N-4],sinas[N-6],sinmks[N-6]; static sin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sin_init(void){if(init)return -1;st.n_sinp=0;st.n_sine=0;st.n_sin2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sinps[i].active=0;for(int i=0;i<N-2;i++)sinss[i].active=0;for(int i=0;i<N-4;i++)sinvss[i].active=0;for(int i=0;i<N-6;i++)sinas[i].active=0;for(int i=0;i<N-6;i++)sinmks[i].active=0;init=1;ps("[SIN] Sinapis initialized\n");return 0;}
int sin_planning(int t,int c,int a,int b,int d,int e,int y){return add(sinps,&st.n_sinp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sin_execution(int t,int c,int a,int b,int d,int e,int y){return add(sinss,&st.n_sine,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sinvss,&st.n_sin2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sinas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sin_market(int t,int c,int a,int b,int d,int e,int y){return add(sinmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sin_report(void){ps("[SIN] Sinpp: ");pi(st.n_sinp);ps(" PCS=");pi(st.t_f1);ps("\nSine: ");pi(st.n_sine);ps(" PCS=");pi(st.t_f2);ps("\nSinv: ");pi(st.n_sin2);ps(" PCS=");pi(st.t_f3);ps("\nSinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sin_state(void){ps("[SIN] Sinpp=");pi(st.n_sinp);ps(" Sine=");pi(st.n_sine);ps(" Sinv=");pi(st.n_sin2);ps(" Sinc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sinapis Admin Demo ===\n\n");sin_init();
ps("Sinapis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sin_planning(t,c,1192+(i*17),1181+(i*14),1161+(i*10),1143+(i*6),2020+(i%5));}
ps("\nSinapis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sin_execution(t,c,1181+(i*15),1170+(i*12),1152+(i*8),1139+(i*5),2021+(i%4));}
ps("\nSinapis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sin_evaluation(t,c,1173+(i*13),1162+(i*10),1146+(i*7),1135+(i*4),2022+(i%3));}
ps("\nSinapis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sin_accessory(t,c,1165+(i*11),1156+(i*9),1142+(i*6),1132+(i*3),2023+(i%2));}
ps("\nSinapis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sin_market(t,c,1159+(i*9),1150+(i*7),1137+(i*5),1129+(i*3),2024);}
ps("\n");sin_report();sin_state();ps("\n=== Demo Complete ===\n");return 0;}
