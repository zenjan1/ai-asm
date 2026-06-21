/* galanthus_admin: Galanthus management technology administration (v1.0)
 * Galanthus planning, galanthus execution, galanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gal_t;
typedef struct{int n_gap,n_gae,n_gav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gal_state_t;
static gal_t gaps[N],gaes[N-2],gavs[N-4],acs[N-6],mks[N-6]; static gal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gal_init(void){if(init)return -1;st.n_gap=0;st.n_gae=0;st.n_gav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gaps[i].active=0;for(int i=0;i<N-2;i++)gaes[i].active=0;for(int i=0;i<N-4;i++)gavs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GAL] Galanthus initialized\n");return 0;}
int gal_planning(int t,int c,int a,int b,int d,int e,int y){return add(gaps,&st.n_gap,&st.t_f1,N,t,c,a,b,d,e,y);}
int gal_execution(int t,int c,int a,int b,int d,int e,int y){return add(gaes,&st.n_gae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gavs,&st.n_gav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gal_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gal_report(void){ps("[GAL] Gap: ");pi(st.n_gap);ps(" PCS=");pi(st.t_f1);ps("\nGae: ");pi(st.n_gae);ps(" PCS=");pi(st.t_f2);ps("\nGav: ");pi(st.n_gav);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gal_state(void){ps("[GAL] Gap=");pi(st.n_gap);ps(" Gae=");pi(st.n_gae);ps(" Gav=");pi(st.n_gav);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Galanthus Admin Demo ===\n\n");gal_init();
ps("Galanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gal_planning(t,c,613+(i*17),602+(i*14),582+(i*10),564+(i*6),2020+(i%5));}
ps("\nGalanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gal_execution(t,c,602+(i*15),591+(i*12),573+(i*8),560+(i*5),2021+(i%4));}
ps("\nGalanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gal_evaluation(t,c,594+(i*13),583+(i*10),567+(i*7),556+(i*4),2022+(i%3));}
ps("\nGalanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gal_accessory(t,c,586+(i*11),577+(i*9),563+(i*6),553+(i*3),2023+(i%2));}
ps("\nGalanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gal_market(t,c,580+(i*9),571+(i*7),558+(i*5),550+(i*3),2024);}
ps("\n");gal_report();gal_state();ps("\n=== Demo Complete ===\n");return 0;}
