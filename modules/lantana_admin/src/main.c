/* lantana_admin: Lantana management technology administration (v1.0)
 * Lantana planning, lantana execution, lantana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lan_t;
typedef struct{int n_lnp,n_lne,n_lnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lan_state_t;
static lan_t lanps[N],lans[N-2],lanvs[N-4],acs[N-6],mks[N-6]; static lan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lan_init(void){if(init)return -1;st.n_lnp=0;st.n_lne=0;st.n_lnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lanps[i].active=0;for(int i=0;i<N-2;i++)lans[i].active=0;for(int i=0;i<N-4;i++)lanvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LAN] Lantana initialized\n");return 0;}
int lan_planning(int t,int c,int a,int b,int d,int e,int y){return add(lanps,&st.n_lnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lan_execution(int t,int c,int a,int b,int d,int e,int y){return add(lans,&st.n_lne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lanvs,&st.n_lnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lan_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lan_report(void){ps("[LAN] Lnp: ");pi(st.n_lnp);ps(" PCS=");pi(st.t_f1);ps("\nLne: ");pi(st.n_lne);ps(" PCS=");pi(st.t_f2);ps("\nLnv: ");pi(st.n_lnv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lan_state(void){ps("[LAN] Lnp=");pi(st.n_lnp);ps(" Lne=");pi(st.n_lne);ps(" Lnv=");pi(st.n_lnv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lantana Admin Demo ===\n\n");lan_init();
ps("Lantana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lan_planning(t,c,598+(i*17),587+(i*14),567+(i*10),549+(i*6),2020+(i%5));}
ps("\nLantana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lan_execution(t,c,587+(i*15),576+(i*12),558+(i*8),545+(i*5),2021+(i%4));}
ps("\nLantana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lan_evaluation(t,c,579+(i*13),568+(i*10),552+(i*7),541+(i*4),2022+(i%3));}
ps("\nLantana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lan_accessory(t,c,571+(i*11),562+(i*9),548+(i*6),538+(i*3),2023+(i%2));}
ps("\nLantana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lan_market(t,c,565+(i*9),556+(i*7),543+(i*5),535+(i*3),2024);}
ps("\n");lan_report();lan_state();ps("\n=== Demo Complete ===\n");return 0;}
