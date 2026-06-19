/* agricultural_economics_admin: Agricultural economics administration (v1.0)
 * Agricultural production, farm product markets, land economics, rural development, agricultural policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} agr_t;
typedef struct{int n_ap,n_fm,n_le,n_rd,n_po,t_f1,t_f2,t_f3,t_f4,t_f5;} agr_state_t;
static agr_t aps[N],fms[N-2],les[N-4],rds[N-6],pos[N-6]; static agr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(agr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;agr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int agr_init(void){if(init)return -1;st.n_ap=0;st.n_fm=0;st.n_le=0;st.n_rd=0;st.n_po=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aps[i].active=0;for(int i=0;i<N-2;i++)fms[i].active=0;for(int i=0;i<N-4;i++)les[i].active=0;for(int i=0;i<N-6;i++)rds[i].active=0;for(int i=0;i<N-6;i++)pos[i].active=0;init=1;ps("[AGR] Agricultural economics initialized\n");return 0;}
int agr_production(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f1,N,t,c,a,b,d,e,y);}
int agr_farm(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int agr_land(int t,int c,int a,int b,int d,int e,int y){return add(les,&st.n_le,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int agr_rural(int t,int c,int a,int b,int d,int e,int y){return add(rds,&st.n_rd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int agr_policy(int t,int c,int a,int b,int d,int e,int y){return add(pos,&st.n_po,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void agr_report(void){ps("[AGR] Product: ");pi(st.n_ap);ps(" Yield=");pi(st.t_f1);ps("\nFarm: ");pi(st.n_fm);ps(" Market=");pi(st.t_f2);ps("\nLand: ");pi(st.n_le);ps(" Transfer=");pi(st.t_f3);ps("\nRural: ");pi(st.n_rd);ps(" Income=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_po);ps(" Subsidy=");pi(st.t_f5);ps("\n");}
void agr_state(void){ps("[AGR] Ap=");pi(st.n_ap);ps(" Fm=");pi(st.n_fm);ps(" Le=");pi(st.n_le);ps(" Rd=");pi(st.n_rd);ps(" Po=");pi(st.n_po);ps("\n");}
int main(void){
ps("=== Agricultural Economics Admin Demo ===\n\n");agr_init();
ps("Agricultural production...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;agr_production(t,c,102+(i*17),87+(i*14),67+(i*10),50+(i*6),2020+(i%5));}
ps("\nFarm product markets...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;agr_farm(t,c,91+(i*15),77+(i*12),59+(i*8),46+(i*5),2021+(i%4));}
ps("\nLand economics...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;agr_land(t,c,83+(i*13),69+(i*10),53+(i*7),42+(i*4),2022+(i%3));}
ps("\nRural development...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agr_rural(t,c,75+(i*11),63+(i*9),49+(i*6),39+(i*3),2023+(i%2));}
ps("\nAgricultural policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agr_policy(t,c,69+(i*9),58+(i*7),45+(i*5),37+(i*3),2024);}
ps("\n");agr_report();agr_state();ps("\n=== Demo Complete ===\n");return 0;}
