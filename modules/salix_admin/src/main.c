/* salix_admin: Salix management technology administration (v1.0)
 * Salix planning, salix execution, salix evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} slx_t;
typedef struct{int n_slp,n_sle,n_slv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} slx_state_t;
static slx_t slxps[N],slxss[N-2],slxvss[N-4],slxas[N-6],slxmks[N-6]; static slx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(slx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;slx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SLX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int slx_init(void){if(init)return -1;st.n_slp=0;st.n_sle=0;st.n_slv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)slxps[i].active=0;for(int i=0;i<N-2;i++)slxss[i].active=0;for(int i=0;i<N-4;i++)slxvss[i].active=0;for(int i=0;i<N-6;i++)slxas[i].active=0;for(int i=0;i<N-6;i++)slxmks[i].active=0;init=1;ps("[SLX] Salix initialized\n");return 0;}
int slx_planning(int t,int c,int a,int b,int d,int e,int y){return add(slxps,&st.n_slp,&st.t_f1,N,t,c,a,b,d,e,y);}
int slx_execution(int t,int c,int a,int b,int d,int e,int y){return add(slxss,&st.n_sle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int slx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(slxvss,&st.n_slv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int slx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(slxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int slx_market(int t,int c,int a,int b,int d,int e,int y){return add(slxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void slx_report(void){ps("[SLX] Slp: ");pi(st.n_slp);ps(" PCS=");pi(st.t_f1);ps("\nSle: ");pi(st.n_sle);ps(" PCS=");pi(st.t_f2);ps("\nSlv: ");pi(st.n_slv);ps(" PCS=");pi(st.t_f3);ps("\nSlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void slx_state(void){ps("[SLX] Slp=");pi(st.n_slp);ps(" Sle=");pi(st.n_sle);ps(" Slv=");pi(st.n_slv);ps(" Slc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Salix Admin Demo ===\n\n");slx_init();
ps("Salix planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;slx_planning(t,c,859+(i*17),848+(i*14),828+(i*10),810+(i*6),2020+(i%5));}
ps("\nSalix execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;slx_execution(t,c,848+(i*15),837+(i*12),819+(i*8),806+(i*5),2021+(i%4));}
ps("\nSalix evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;slx_evaluation(t,c,840+(i*13),829+(i*10),813+(i*7),802+(i*4),2022+(i%3));}
ps("\nSalix accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;slx_accessory(t,c,832+(i*11),823+(i*9),809+(i*6),799+(i*3),2023+(i%2));}
ps("\nSalix marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;slx_market(t,c,826+(i*9),817+(i*7),804+(i*5),796+(i*3),2024);}
ps("\n");slx_report();slx_state();ps("\n=== Demo Complete ===\n");return 0;}
