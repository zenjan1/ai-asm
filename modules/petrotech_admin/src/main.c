/* petrotech_admin: Petroleum technology administration (v1.0)
 * Oil & gas exploration, oil & gas development, storage & transport, refining, petrochemical
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pet_t;
typedef struct{int n_ex,n_dv,n_st,n_rf,n_pc,t_f1,t_f2,t_f3,t_f4,t_f5;} pet_state_t;
static pet_t exs[N],dvs[N-2],sts[N-4],rfs[N-6],pcs[N-6]; static pet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pet_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pet_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pet_init(void){if(init)return -1;st.n_ex=0;st.n_dv=0;st.n_st=0;st.n_rf=0;st.n_pc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)exs[i].active=0;for(int i=0;i<N-2;i++)dvs[i].active=0;for(int i=0;i<N-4;i++)sts[i].active=0;for(int i=0;i<N-6;i++)rfs[i].active=0;for(int i=0;i<N-6;i++)pcs[i].active=0;init=1;ps("[PET] Petrotech initialized\n");return 0;}
int pet_explore(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f1,N,t,c,a,b,d,e,y);}
int pet_develop(int t,int c,int a,int b,int d,int e,int y){return add(dvs,&st.n_dv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pet_storage(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pet_refine(int t,int c,int a,int b,int d,int e,int y){return add(rfs,&st.n_rf,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pet_petrochem(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pet_report(void){ps("[PET] Expl: ");pi(st.n_ex);ps(" Well=");pi(st.t_f1);ps("\nDev: ");pi(st.n_dv);ps(" Bbl=");pi(st.t_f2);ps("\nStore: ");pi(st.n_st);ps(" LNG=");pi(st.t_f3);ps("\nRefine: ");pi(st.n_rf);ps(" CDU=");pi(st.t_f4);ps("\nPetro: ");pi(st.n_pc);ps(" Eth=");pi(st.t_f5);ps("\n");}
void pet_state(void){ps("[PET] Ex=");pi(st.n_ex);ps(" Dv=");pi(st.n_dv);ps(" St=");pi(st.n_st);ps(" Rf=");pi(st.n_rf);ps(" Pc=");pi(st.n_pc);ps("\n");}
int main(void){
ps("=== Petroleum Tech Admin Demo ===\n\n");pet_init();
ps("Oil & gas exploration...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pet_explore(t,c,162+(i*17),147+(i*14),127+(i*10),110+(i*6),2020+(i%5));}
ps("\nOil & gas development...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pet_develop(t,c,151+(i*15),137+(i*12),119+(i*8),106+(i*5),2021+(i%4));}
ps("\nStorage & transport...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pet_storage(t,c,143+(i*13),129+(i*10),113+(i*7),102+(i*4),2022+(i%3));}
ps("\nPetroleum refining...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pet_refine(t,c,135+(i*11),123+(i*9),109+(i*6),99+(i*3),2023+(i%2));}
ps("\nPetrochemical...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pet_petrochem(t,c,129+(i*9),118+(i*7),105+(i*5),97+(i*3),2024);}
ps("\n");pet_report();pet_state();ps("\n=== Demo Complete ===\n");return 0;}
