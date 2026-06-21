/* lamarckia_admin: Lamarckia management technology administration (v1.0)
 * Lamarckia planning, lamarckia execution, lamarckia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lam_t;
typedef struct{int n_lamp,n_lame,n_lam2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lam_state_t;
static lam_t lamps[N],lamss[N-2],lamvss[N-4],lamas[N-6],lammks[N-6]; static lam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lam_init(void){if(init)return -1;st.n_lamp=0;st.n_lame=0;st.n_lam2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lamps[i].active=0;for(int i=0;i<N-2;i++)lamss[i].active=0;for(int i=0;i<N-4;i++)lamvss[i].active=0;for(int i=0;i<N-6;i++)lamas[i].active=0;for(int i=0;i<N-6;i++)lammks[i].active=0;init=1;ps("[LAM] Lamarckia initialized\n");return 0;}
int lam_planning(int t,int c,int a,int b,int d,int e,int y){return add(lamps,&st.n_lamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lam_execution(int t,int c,int a,int b,int d,int e,int y){return add(lamss,&st.n_lame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lam_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lamvss,&st.n_lam2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lam_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lamas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lam_market(int t,int c,int a,int b,int d,int e,int y){return add(lammks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lam_report(void){ps("[LAM] Lamp: ");pi(st.n_lamp);ps(" PCS=");pi(st.t_f1);ps("\nLame: ");pi(st.n_lame);ps(" PCS=");pi(st.t_f2);ps("\nLamv: ");pi(st.n_lam2);ps(" PCS=");pi(st.t_f3);ps("\nLamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lam_state(void){ps("[LAM] Lamp=");pi(st.n_lamp);ps(" Lame=");pi(st.n_lame);ps(" Lamv=");pi(st.n_lam2);ps(" Lamc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lamarckia Admin Demo ===\n\n");lam_init();
ps("Lamarckia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lam_planning(t,c,1042+(i*17),1031+(i*14),1011+(i*10),993+(i*6),2020+(i%5));}
ps("\nLamarckia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lam_execution(t,c,1031+(i*15),1020+(i*12),1002+(i*8),989+(i*5),2021+(i%4));}
ps("\nLamarckia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lam_evaluation(t,c,1023+(i*13),1012+(i*10),996+(i*7),985+(i*4),2022+(i%3));}
ps("\nLamarckia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lam_accessory(t,c,1015+(i*11),1006+(i*9),992+(i*6),982+(i*3),2023+(i%2));}
ps("\nLamarckia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lam_market(t,c,1009+(i*9),1000+(i*7),987+(i*5),979+(i*3),2024);}
ps("\n");lam_report();lam_state();ps("\n=== Demo Complete ===\n");return 0;}
