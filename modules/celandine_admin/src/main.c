/* celandine_admin: Celandine management technology administration (v1.0)
 * Celandine planning, celandine execution, celandine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cel_t;
typedef struct{int n_clp,n_cle,n_clv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cel_state_t;
static cel_t clps[N],cles[N-2],clvs[N-4],acs[N-6],mks[N-6]; static cel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cel_init(void){if(init)return -1;st.n_clp=0;st.n_cle=0;st.n_clv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)clps[i].active=0;for(int i=0;i<N-2;i++)cles[i].active=0;for(int i=0;i<N-4;i++)clvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CEL] Celandine initialized\n");return 0;}
int cel_planning(int t,int c,int a,int b,int d,int e,int y){return add(clps,&st.n_clp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cel_execution(int t,int c,int a,int b,int d,int e,int y){return add(cles,&st.n_cle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(clvs,&st.n_clv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cel_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cel_report(void){ps("[CEL] Clp: ");pi(st.n_clp);ps(" PCS=");pi(st.t_f1);ps("\nCle: ");pi(st.n_cle);ps(" PCS=");pi(st.t_f2);ps("\nClv: ");pi(st.n_clv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cel_state(void){ps("[CEL] Clp=");pi(st.n_clp);ps(" Cle=");pi(st.n_cle);ps(" Clv=");pi(st.n_clv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Celandine Admin Demo ===\n\n");cel_init();
ps("Celandine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cel_planning(t,c,582+(i*17),571+(i*14),551+(i*10),533+(i*6),2020+(i%5));}
ps("\nCelandine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cel_execution(t,c,571+(i*15),560+(i*12),542+(i*8),529+(i*5),2021+(i%4));}
ps("\nCelandine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cel_evaluation(t,c,563+(i*13),552+(i*10),536+(i*7),525+(i*4),2022+(i%3));}
ps("\nCelandine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cel_accessory(t,c,555+(i*11),546+(i*9),532+(i*6),522+(i*3),2023+(i%2));}
ps("\nCelandine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cel_market(t,c,549+(i*9),540+(i*7),527+(i*5),519+(i*3),2024);}
ps("\n");cel_report();cel_state();ps("\n=== Demo Complete ===\n");return 0;}
