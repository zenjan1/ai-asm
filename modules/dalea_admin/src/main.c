/* dalea_admin: Dalea management technology administration (v1.0)
 * Dalea planning, dalea execution, dalea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dal_t;
typedef struct{int n_dalp,n_dale,n_dal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dal_state_t;
static dal_t dalps[N],dalss[N-2],dalvss[N-4],dalas[N-6],dalmks[N-6]; static dal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dal_init(void){if(init)return -1;st.n_dalp=0;st.n_dale=0;st.n_dal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dalps[i].active=0;for(int i=0;i<N-2;i++)dalss[i].active=0;for(int i=0;i<N-4;i++)dalvss[i].active=0;for(int i=0;i<N-6;i++)dalas[i].active=0;for(int i=0;i<N-6;i++)dalmks[i].active=0;init=1;ps("[DAL] Dalea initialized\n");return 0;}
int dal_planning(int t,int c,int a,int b,int d,int e,int y){return add(dalps,&st.n_dalp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dal_execution(int t,int c,int a,int b,int d,int e,int y){return add(dalss,&st.n_dale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dalvss,&st.n_dal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dalas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dal_market(int t,int c,int a,int b,int d,int e,int y){return add(dalmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dal_report(void){ps("[DAL] Dalpp: ");pi(st.n_dalp);ps(" PCS=");pi(st.t_f1);ps("\nDalpe: ");pi(st.n_dale);ps(" PCS=");pi(st.t_f2);ps("\nDalv: ");pi(st.n_dal2);ps(" PCS=");pi(st.t_f3);ps("\nDalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dal_state(void){ps("[DAL] Dalpp=");pi(st.n_dalp);ps(" Dalpe=");pi(st.n_dale);ps(" Dalv=");pi(st.n_dal2);ps(" Dalc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dalea Admin Demo ===\n\n");dal_init();
ps("Dalea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dal_planning(t,c,1255+(i*17),1244+(i*14),1224+(i*10),1206+(i*6),2020+(i%5));}
ps("\nDalea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dal_execution(t,c,1244+(i*15),1233+(i*12),1215+(i*8),1202+(i*5),2021+(i%4));}
ps("\nDalea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dal_evaluation(t,c,1236+(i*13),1225+(i*10),1209+(i*7),1198+(i*4),2022+(i%3));}
ps("\nDalea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dal_accessory(t,c,1228+(i*11),1219+(i*9),1205+(i*6),1195+(i*3),2023+(i%2));}
ps("\nDalea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dal_market(t,c,1222+(i*9),1213+(i*7),1200+(i*5),1192+(i*3),2024);}
ps("\n");dal_report();dal_state();ps("\n=== Demo Complete ===\n");return 0;}
