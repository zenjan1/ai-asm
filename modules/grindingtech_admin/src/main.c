/* grindingtech_admin: Grinding technology administration (v1.0)
 * External grinding, internal grinding, surface grinding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} grd_t;
typedef struct{int n_ex,n_in,n_sf,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} grd_state_t;
static grd_t exs[N],ins[N-2],sfs[N-4],acs[N-6],mks[N-6]; static grd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;grd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GRD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int grd_init(void){if(init)return -1;st.n_ex=0;st.n_in=0;st.n_sf=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)exs[i].active=0;for(int i=0;i<N-2;i++)ins[i].active=0;for(int i=0;i<N-4;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GRD] Grindingtech initialized\n");return 0;}
int grd_external(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f1,N,t,c,a,b,d,e,y);}
int grd_internal(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int grd_surface(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int grd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int grd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void grd_report(void){ps("[GRD] Ex: ");pi(st.n_ex);ps(" PCS=");pi(st.t_f1);ps("\nIn: ");pi(st.n_in);ps(" PCS=");pi(st.t_f2);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void grd_state(void){ps("[GRD] Ex=");pi(st.n_ex);ps(" In=");pi(st.n_in);ps(" Sf=");pi(st.n_sf);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Grinding Tech Admin Demo ===\n\n");grd_init();
ps("External grinding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;grd_external(t,c,274+(i*17),259+(i*14),239+(i*10),221+(i*6),2020+(i%5));}
ps("\nInternal grinding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;grd_internal(t,c,263+(i*15),249+(i*12),231+(i*8),218+(i*5),2021+(i%4));}
ps("\nSurface grinding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;grd_surface(t,c,255+(i*13),241+(i*10),225+(i*7),214+(i*4),2022+(i%3));}
ps("\nGrinding accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grd_accessory(t,c,247+(i*11),235+(i*9),221+(i*6),211+(i*3),2023+(i%2));}
ps("\nGrinding marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grd_market(t,c,241+(i*9),230+(i*7),217+(i*5),209+(i*3),2024);}
ps("\n");grd_report();grd_state();ps("\n=== Demo Complete ===\n");return 0;}
