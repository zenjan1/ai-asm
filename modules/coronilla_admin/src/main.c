/* coronilla_admin: Coronilla management technology administration (v1.0)
 * Coronilla planning, coronilla execution, coronilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ron_t;
typedef struct{int n_ronp,n_rone,n_ron2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ron_state_t;
static ron_t ronps[N],ronss[N-2],ronvss[N-4],ronas[N-6],ronmks[N-6]; static ron_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ron_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ron_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ron_init(void){if(init)return -1;st.n_ronp=0;st.n_rone=0;st.n_ron2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ronps[i].active=0;for(int i=0;i<N-2;i++)ronss[i].active=0;for(int i=0;i<N-4;i++)ronvss[i].active=0;for(int i=0;i<N-6;i++)ronas[i].active=0;for(int i=0;i<N-6;i++)ronmks[i].active=0;init=1;ps("[RON] Coronilla initialized\n");return 0;}
int ron_planning(int t,int c,int a,int b,int d,int e,int y){return add(ronps,&st.n_ronp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ron_execution(int t,int c,int a,int b,int d,int e,int y){return add(ronss,&st.n_rone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ron_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ronvss,&st.n_ron2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ron_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ronas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ron_market(int t,int c,int a,int b,int d,int e,int y){return add(ronmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ron_report(void){ps("[RON] Corpp: ");pi(st.n_ronp);ps(" PCS=");pi(st.t_f1);ps("\nCorpe: ");pi(st.n_rone);ps(" PCS=");pi(st.t_f2);ps("\nCorv: ");pi(st.n_ron2);ps(" PCS=");pi(st.t_f3);ps("\nCorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ron_state(void){ps("[RON] Corpp=");pi(st.n_ronp);ps(" Corpe=");pi(st.n_rone);ps(" Corv=");pi(st.n_ron2);ps(" Corc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coronilla Admin Demo ===\n\n");ron_init();
ps("Coronilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ron_planning(t,c,1245+(i*17),1234+(i*14),1214+(i*10),1196+(i*6),2020+(i%5));}
ps("\nCoronilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ron_execution(t,c,1234+(i*15),1223+(i*12),1205+(i*8),1192+(i*5),2021+(i%4));}
ps("\nCoronilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ron_evaluation(t,c,1226+(i*13),1215+(i*10),1199+(i*7),1188+(i*4),2022+(i%3));}
ps("\nCoronilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ron_accessory(t,c,1218+(i*11),1209+(i*9),1195+(i*6),1185+(i*3),2023+(i%2));}
ps("\nCoronilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ron_market(t,c,1212+(i*9),1203+(i*7),1190+(i*5),1182+(i*3),2024);}
ps("\n");ron_report();ron_state();ps("\n=== Demo Complete ===\n");return 0;}
