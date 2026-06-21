/* nigella_admin: Nigella management technology administration (v1.0)
 * Nigella planning, nigella execution, nigella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ngl_t;
typedef struct{int n_ngp,n_nge,n_ngv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ngl_state_t;
static ngl_t nglps[N],nglss[N-2],nglvss[N-4],nglas[N-6],nglmks[N-6]; static ngl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ngl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ngl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NGL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ngl_init(void){if(init)return -1;st.n_ngp=0;st.n_nge=0;st.n_ngv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nglps[i].active=0;for(int i=0;i<N-2;i++)nglss[i].active=0;for(int i=0;i<N-4;i++)nglvss[i].active=0;for(int i=0;i<N-6;i++)nglas[i].active=0;for(int i=0;i<N-6;i++)nglmks[i].active=0;init=1;ps("[NGL] Nigella initialized\n");return 0;}
int ngl_planning(int t,int c,int a,int b,int d,int e,int y){return add(nglps,&st.n_ngp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ngl_execution(int t,int c,int a,int b,int d,int e,int y){return add(nglss,&st.n_nge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ngl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nglvss,&st.n_ngv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ngl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nglas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ngl_market(int t,int c,int a,int b,int d,int e,int y){return add(nglmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ngl_report(void){ps("[NGL] Ngp: ");pi(st.n_ngp);ps(" PCS=");pi(st.t_f1);ps("\nNge: ");pi(st.n_nge);ps(" PCS=");pi(st.t_f2);ps("\nNgv: ");pi(st.n_ngv);ps(" PCS=");pi(st.t_f3);ps("\nNgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ngl_state(void){ps("[NGL] Ngp=");pi(st.n_ngp);ps(" Nge=");pi(st.n_nge);ps(" Ngv=");pi(st.n_ngv);ps(" Ngc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nigella Admin Demo ===\n\n");ngl_init();
ps("Nigella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ngl_planning(t,c,845+(i*17),834+(i*14),814+(i*10),796+(i*6),2020+(i%5));}
ps("\nNigella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ngl_execution(t,c,834+(i*15),823+(i*12),805+(i*8),792+(i*5),2021+(i%4));}
ps("\nNigella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ngl_evaluation(t,c,826+(i*13),815+(i*10),799+(i*7),788+(i*4),2022+(i%3));}
ps("\nNigella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ngl_accessory(t,c,818+(i*11),809+(i*9),795+(i*6),785+(i*3),2023+(i%2));}
ps("\nNigella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ngl_market(t,c,812+(i*9),803+(i*7),790+(i*5),782+(i*3),2024);}
ps("\n");ngl_report();ngl_state();ps("\n=== Demo Complete ===\n");return 0;}
