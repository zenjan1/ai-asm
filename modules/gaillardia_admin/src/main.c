/* gaillardia_admin: Gaillardia management technology administration (v1.0)
 * Gaillardia planning, gaillardia execution, gaillardia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gai_t;
typedef struct{int n_gaip,n_gaie,n_gai2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gai_state_t;
static gai_t gaips[N],gaiss[N-2],gaivss[N-4],gaias[N-6],gaimks[N-6]; static gai_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gai_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gai_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gai_init(void){if(init)return -1;st.n_gaip=0;st.n_gaie=0;st.n_gai2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gaips[i].active=0;for(int i=0;i<N-2;i++)gaiss[i].active=0;for(int i=0;i<N-4;i++)gaivss[i].active=0;for(int i=0;i<N-6;i++)gaias[i].active=0;for(int i=0;i<N-6;i++)gaimks[i].active=0;init=1;ps("[GAI] Gaillardia initialized\n");return 0;}
int gai_planning(int t,int c,int a,int b,int d,int e,int y){return add(gaips,&st.n_gaip,&st.t_f1,N,t,c,a,b,d,e,y);}
int gai_execution(int t,int c,int a,int b,int d,int e,int y){return add(gaiss,&st.n_gaie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gai_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gaivss,&st.n_gai2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gai_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gaias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gai_market(int t,int c,int a,int b,int d,int e,int y){return add(gaimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gai_report(void){ps("[GAI] Gaip: ");pi(st.n_gaip);ps(" PCS=");pi(st.t_f1);ps("\nGaie: ");pi(st.n_gaie);ps(" PCS=");pi(st.t_f2);ps("\nGaiv: ");pi(st.n_gai2);ps(" PCS=");pi(st.t_f3);ps("\nGaic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gai_state(void){ps("[GAI] Gaip=");pi(st.n_gaip);ps(" Gaie=");pi(st.n_gaie);ps(" Gaiv=");pi(st.n_gai2);ps(" Gaic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gaillardia Admin Demo ===\n\n");gai_init();
ps("Gaillardia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gai_planning(t,c,1009+(i*17),998+(i*14),978+(i*10),960+(i*6),2020+(i%5));}
ps("\nGaillardia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gai_execution(t,c,998+(i*15),987+(i*12),969+(i*8),956+(i*5),2021+(i%4));}
ps("\nGaillardia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gai_evaluation(t,c,990+(i*13),979+(i*10),963+(i*7),952+(i*4),2022+(i%3));}
ps("\nGaillardia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gai_accessory(t,c,982+(i*11),973+(i*9),959+(i*6),949+(i*3),2023+(i%2));}
ps("\nGaillardia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gai_market(t,c,976+(i*9),967+(i*7),954+(i*5),946+(i*3),2024);}
ps("\n");gai_report();gai_state();ps("\n=== Demo Complete ===\n");return 0;}
