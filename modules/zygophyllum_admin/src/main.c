/* zygophyllum_admin: Zygophyllum management technology administration (v1.0)
 * Zygophyllum planning, zygophyllum execution, zygophyllum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zyg_t;
typedef struct{int n_zygp,n_zyge,n_zyg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zyg_state_t;
static zyg_t zygps[N],zygss[N-2],zygvss[N-4],zygas[N-6],zygmks[N-6]; static zyg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zyg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zyg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZYG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zyg_init(void){if(init)return -1;st.n_zygp=0;st.n_zyge=0;st.n_zyg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zygps[i].active=0;for(int i=0;i<N-2;i++)zygss[i].active=0;for(int i=0;i<N-4;i++)zygvss[i].active=0;for(int i=0;i<N-6;i++)zygas[i].active=0;for(int i=0;i<N-6;i++)zygmks[i].active=0;init=1;ps("[ZYG] Zygophyllum initialized\n");return 0;}
int zyg_planning(int t,int c,int a,int b,int d,int e,int y){return add(zygps,&st.n_zygp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zyg_execution(int t,int c,int a,int b,int d,int e,int y){return add(zygss,&st.n_zyge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zyg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zygvss,&st.n_zyg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zyg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zygas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zyg_market(int t,int c,int a,int b,int d,int e,int y){return add(zygmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zyg_report(void){ps("[ZYG] Zygpp: ");pi(st.n_zygp);ps(" PCS=");pi(st.t_f1);ps("\nZygpe: ");pi(st.n_zyge);ps(" PCS=");pi(st.t_f2);ps("\nZygv: ");pi(st.n_zyg2);ps(" PCS=");pi(st.t_f3);ps("\nZygc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zyg_state(void){ps("[ZYG] Zygpp=");pi(st.n_zygp);ps(" Zygpe=");pi(st.n_zyge);ps(" Zygv=");pi(st.n_zyg2);ps(" Zygc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zygophyllum Admin Demo ===\n\n");zyg_init();
ps("Zygophyllum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zyg_planning(t,c,1208+(i*17),1197+(i*14),1177+(i*10),1159+(i*6),2020+(i%5));}
ps("\nZygophyllum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zyg_execution(t,c,1197+(i*15),1186+(i*12),1168+(i*8),1155+(i*5),2021+(i%4));}
ps("\nZygophyllum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zyg_evaluation(t,c,1189+(i*13),1178+(i*10),1162+(i*7),1151+(i*4),2022+(i%3));}
ps("\nZygophyllum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zyg_accessory(t,c,1181+(i*11),1172+(i*9),1158+(i*6),1148+(i*3),2023+(i%2));}
ps("\nZygophyllum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zyg_market(t,c,1175+(i*9),1166+(i*7),1153+(i*5),1145+(i*3),2024);}
ps("\n");zyg_report();zyg_state();ps("\n=== Demo Complete ===\n");return 0;}
