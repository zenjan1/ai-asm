/* verbascum_admin: Verbascum management technology administration (v1.0)
 * Verbascum planning, verbascum execution, verbascum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vrm_t;
typedef struct{int n_vrp,n_vre,n_vrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vrm_state_t;
static vrm_t vrmss[N],vrmess[N-2],vrmvss[N-4],vrmnas[N-6],vrmrms[N-6]; static vrm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vrm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vrm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vrm_init(void){if(init)return -1;st.n_vrp=0;st.n_vre=0;st.n_vrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vrmss[i].active=0;for(int i=0;i<N-2;i++)vrmess[i].active=0;for(int i=0;i<N-4;i++)vrmvss[i].active=0;for(int i=0;i<N-6;i++)vrmnas[i].active=0;for(int i=0;i<N-6;i++)vrmrms[i].active=0;init=1;ps("[VRM] Verbascum initialized\n");return 0;}
int vrm_planning(int t,int c,int a,int b,int d,int e,int y){return add(vrmss,&st.n_vrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vrm_execution(int t,int c,int a,int b,int d,int e,int y){return add(vrmess,&st.n_vre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vrm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vrmvss,&st.n_vrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vrm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vrmnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vrm_market(int t,int c,int a,int b,int d,int e,int y){return add(vrmrms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vrm_report(void){ps("[VRM] Vrp: ");pi(st.n_vrp);ps(" PCS=");pi(st.t_f1);ps("\nVre: ");pi(st.n_vre);ps(" PCS=");pi(st.t_f2);ps("\nVrv: ");pi(st.n_vrv);ps(" PCS=");pi(st.t_f3);ps("\nVrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vrm_state(void){ps("[VRM] Vrp=");pi(st.n_vrp);ps(" Vre=");pi(st.n_vre);ps(" Vrv=");pi(st.n_vrv);ps(" Vrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Verbascum Admin Demo ===\n\n");vrm_init();
ps("Verbascum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vrm_planning(t,c,876+(i*17),865+(i*14),845+(i*10),827+(i*6),2020+(i%5));}
ps("\nVerbascum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vrm_execution(t,c,865+(i*15),854+(i*12),836+(i*8),823+(i*5),2021+(i%4));}
ps("\nVerbascum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vrm_evaluation(t,c,857+(i*13),846+(i*10),830+(i*7),819+(i*4),2022+(i%3));}
ps("\nVerbascum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vrm_accessory(t,c,849+(i*11),840+(i*9),826+(i*6),816+(i*3),2023+(i%2));}
ps("\nVerbascum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vrm_market(t,c,843+(i*9),834+(i*7),821+(i*5),813+(i*3),2024);}
ps("\n");vrm_report();vrm_state();ps("\n=== Demo Complete ===\n");return 0;}
