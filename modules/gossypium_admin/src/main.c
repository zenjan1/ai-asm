/* gossypium_admin: Gossypium management technology administration (v1.0)
 * Gossypium planning, gossypium execution, gossypium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gos_t;
typedef struct{int n_gosp,n_gose,n_gos2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gos_state_t;
static gos_t gosps[N],gosss[N-2],gosvss[N-4],gosas[N-6],gosmks[N-6]; static gos_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gos_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gos_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GOS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gos_init(void){if(init)return -1;st.n_gosp=0;st.n_gose=0;st.n_gos2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gosps[i].active=0;for(int i=0;i<N-2;i++)gosss[i].active=0;for(int i=0;i<N-4;i++)gosvss[i].active=0;for(int i=0;i<N-6;i++)gosas[i].active=0;for(int i=0;i<N-6;i++)gosmks[i].active=0;init=1;ps("[GOS] Gossypium initialized\n");return 0;}
int gos_planning(int t,int c,int a,int b,int d,int e,int y){return add(gosps,&st.n_gosp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gos_execution(int t,int c,int a,int b,int d,int e,int y){return add(gosss,&st.n_gose,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gos_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gosvss,&st.n_gos2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gos_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gosas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gos_market(int t,int c,int a,int b,int d,int e,int y){return add(gosmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gos_report(void){ps("[GOS] Gossp: ");pi(st.n_gosp);ps(" PCS=");pi(st.t_f1);ps("\nGose: ");pi(st.n_gose);ps(" PCS=");pi(st.t_f2);ps("\nGosv: ");pi(st.n_gos2);ps(" PCS=");pi(st.t_f3);ps("\nGosc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gos_state(void){ps("[GOS] Gossp=");pi(st.n_gosp);ps(" Gose=");pi(st.n_gose);ps(" Gosv=");pi(st.n_gos2);ps(" Gosc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gossypium Admin Demo ===\n\n");gos_init();
ps("Gossypium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gos_planning(t,c,1012+(i*17),1001+(i*14),981+(i*10),963+(i*6),2020+(i%5));}
ps("\nGossypium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gos_execution(t,c,1001+(i*15),990+(i*12),972+(i*8),959+(i*5),2021+(i%4));}
ps("\nGossypium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gos_evaluation(t,c,993+(i*13),982+(i*10),966+(i*7),955+(i*4),2022+(i%3));}
ps("\nGossypium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gos_accessory(t,c,985+(i*11),976+(i*9),962+(i*6),952+(i*3),2023+(i%2));}
ps("\nGossypium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gos_market(t,c,979+(i*9),970+(i*7),957+(i*5),949+(i*3),2024);}
ps("\n");gos_report();gos_state();ps("\n=== Demo Complete ===\n");return 0;}
