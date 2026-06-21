/* nerine_admin: Nerine management technology administration (v1.0)
 * Nerine planning, nerine execution, nerine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nrn_t;
typedef struct{int n_nrnp,n_nrne,n_nrnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nrn_state_t;
static nrn_t nrnps[N],nrnss[N-2],nrnvss[N-4],nrnas[N-6],nrnmks[N-6]; static nrn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nrn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nrn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nrn_init(void){if(init)return -1;st.n_nrnp=0;st.n_nrne=0;st.n_nrnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nrnps[i].active=0;for(int i=0;i<N-2;i++)nrnss[i].active=0;for(int i=0;i<N-4;i++)nrnvss[i].active=0;for(int i=0;i<N-6;i++)nrnas[i].active=0;for(int i=0;i<N-6;i++)nrnmks[i].active=0;init=1;ps("[NRN] Nerine initialized\n");return 0;}
int nrn_planning(int t,int c,int a,int b,int d,int e,int y){return add(nrnps,&st.n_nrnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nrn_execution(int t,int c,int a,int b,int d,int e,int y){return add(nrnss,&st.n_nrne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nrn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nrnvss,&st.n_nrnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nrn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nrnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nrn_market(int t,int c,int a,int b,int d,int e,int y){return add(nrnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nrn_report(void){ps("[NRN] Nrnp: ");pi(st.n_nrnp);ps(" PCS=");pi(st.t_f1);ps("\nNrne: ");pi(st.n_nrne);ps(" PCS=");pi(st.t_f2);ps("\nNrnv: ");pi(st.n_nrnv);ps(" PCS=");pi(st.t_f3);ps("\nNrnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nrn_state(void){ps("[NRN] Nrnp=");pi(st.n_nrnp);ps(" Nrne=");pi(st.n_nrne);ps(" Nrnv=");pi(st.n_nrnv);ps(" Nrnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nerine Admin Demo ===\n\n");nrn_init();
ps("Nerine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nrn_planning(t,c,842+(i*17),831+(i*14),811+(i*10),793+(i*6),2020+(i%5));}
ps("\nNerine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nrn_execution(t,c,831+(i*15),820+(i*12),802+(i*8),789+(i*5),2021+(i%4));}
ps("\nNerine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nrn_evaluation(t,c,823+(i*13),812+(i*10),796+(i*7),785+(i*4),2022+(i%3));}
ps("\nNerine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrn_accessory(t,c,815+(i*11),806+(i*9),792+(i*6),782+(i*3),2023+(i%2));}
ps("\nNerine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrn_market(t,c,809+(i*9),800+(i*7),787+(i*5),779+(i*3),2024);}
ps("\n");nrn_report();nrn_state();ps("\n=== Demo Complete ===\n");return 0;}
