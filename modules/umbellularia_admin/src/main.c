/* umbellularia_admin: Umbellularia management technology administration (v1.0)
 * Umbellularia planning, umbellularia execution, umbellularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} umb_t;
typedef struct{int n_umbp,n_umbe,n_umb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} umb_state_t;
static umb_t umbps[N],umbss[N-2],umbvss[N-4],umbas[N-6],umbmks[N-6]; static umb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(umb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;umb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UMB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int umb_init(void){if(init)return -1;st.n_umbp=0;st.n_umbe=0;st.n_umb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)umbps[i].active=0;for(int i=0;i<N-2;i++)umbss[i].active=0;for(int i=0;i<N-4;i++)umbvss[i].active=0;for(int i=0;i<N-6;i++)umbas[i].active=0;for(int i=0;i<N-6;i++)umbmks[i].active=0;init=1;ps("[UMB] Umbellularia initialized\n");return 0;}
int umb_planning(int t,int c,int a,int b,int d,int e,int y){return add(umbps,&st.n_umbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int umb_execution(int t,int c,int a,int b,int d,int e,int y){return add(umbss,&st.n_umbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int umb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(umbvss,&st.n_umb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int umb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(umbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int umb_market(int t,int c,int a,int b,int d,int e,int y){return add(umbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void umb_report(void){ps("[UMB] Umbpp: ");pi(st.n_umbp);ps(" PCS=");pi(st.t_f1);ps("\nUmbe: ");pi(st.n_umbe);ps(" PCS=");pi(st.t_f2);ps("\nUmbv: ");pi(st.n_umb2);ps(" PCS=");pi(st.t_f3);ps("\nUmbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void umb_state(void){ps("[UMB] Umbpp=");pi(st.n_umbp);ps(" Umbe=");pi(st.n_umbe);ps(" Umbv=");pi(st.n_umb2);ps(" Umbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Umbellularia Admin Demo ===\n\n");umb_init();
ps("Umbellularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;umb_planning(t,c,1109+(i*17),1098+(i*14),1078+(i*10),1060+(i*6),2020+(i%5));}
ps("\nUmbellularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;umb_execution(t,c,1098+(i*15),1087+(i*12),1069+(i*8),1056+(i*5),2021+(i%4));}
ps("\nUmbellularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;umb_evaluation(t,c,1090+(i*13),1079+(i*10),1063+(i*7),1052+(i*4),2022+(i%3));}
ps("\nUmbellularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;umb_accessory(t,c,1082+(i*11),1073+(i*9),1059+(i*6),1049+(i*3),2023+(i%2));}
ps("\nUmbellularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;umb_market(t,c,1076+(i*9),1067+(i*7),1054+(i*5),1046+(i*3),2024);}
ps("\n");umb_report();umb_state();ps("\n=== Demo Complete ===\n");return 0;}
