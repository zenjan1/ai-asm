/* meadowsweet_admin: Meadowsweet management technology administration (v1.0)
 * Meadowsweet planning, meadowsweet execution, meadowsweet evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} md_t;
typedef struct{int n_mdp,n_mde,n_mdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} md_state_t;
static md_t mdps[N],mdss[N-2],mdvss[N-4],mdas[N-6],mdmks[N-6]; static md_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(md_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;md_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int md_init(void){if(init)return -1;st.n_mdp=0;st.n_mde=0;st.n_mdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mdps[i].active=0;for(int i=0;i<N-2;i++)mdss[i].active=0;for(int i=0;i<N-4;i++)mdvss[i].active=0;for(int i=0;i<N-6;i++)mdas[i].active=0;for(int i=0;i<N-6;i++)mdmks[i].active=0;init=1;ps("[MDS] Meadowsweet initialized\n");return 0;}
int md_planning(int t,int c,int a,int b,int d,int e,int y){return add(mdps,&st.n_mdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int md_execution(int t,int c,int a,int b,int d,int e,int y){return add(mdss,&st.n_mde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int md_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mdvss,&st.n_mdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int md_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int md_market(int t,int c,int a,int b,int d,int e,int y){return add(mdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void md_report(void){ps("[MDS] Mdp: ");pi(st.n_mdp);ps(" PCS=");pi(st.t_f1);ps("\nMde: ");pi(st.n_mde);ps(" PCS=");pi(st.t_f2);ps("\nMdv: ");pi(st.n_mdv);ps(" PCS=");pi(st.t_f3);ps("\nMdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void md_state(void){ps("[MDS] Mdp=");pi(st.n_mdp);ps(" Mde=");pi(st.n_mde);ps(" Mdv=");pi(st.n_mdv);ps(" Mdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Meadowsweet Admin Demo ===\n\n");md_init();
ps("Meadowsweet planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;md_planning(t,c,708+(i*17),697+(i*14),677+(i*10),659+(i*6),2020+(i%5));}
ps("\nMeadowsweet execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;md_execution(t,c,697+(i*15),686+(i*12),668+(i*8),655+(i*5),2021+(i%4));}
ps("\nMeadowsweet evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;md_evaluation(t,c,689+(i*13),678+(i*10),662+(i*7),651+(i*4),2022+(i%3));}
ps("\nMeadowsweet accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;md_accessory(t,c,681+(i*11),672+(i*9),658+(i*6),648+(i*3),2023+(i%2));}
ps("\nMeadowsweet marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;md_market(t,c,675+(i*9),666+(i*7),653+(i*5),645+(i*3),2024);}
ps("\n");md_report();md_state();ps("\n=== Demo Complete ===\n");return 0;}
