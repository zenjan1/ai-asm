/* leonotis_admin: Leonotis management technology administration (v1.0)
 * Leonotis planning, leonotis execution, leonotis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lc_t;
typedef struct{int n_lnp,n_lne,n_lnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lc_state_t;
static lc_t lnps[N],lnss[N-2],lnvss[N-4],lnas[N-6],lnmks[N-6]; static lc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lcn_init(void){if(init)return -1;st.n_lnp=0;st.n_lne=0;st.n_lnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lnps[i].active=0;for(int i=0;i<N-2;i++)lnss[i].active=0;for(int i=0;i<N-4;i++)lnvss[i].active=0;for(int i=0;i<N-6;i++)lnas[i].active=0;for(int i=0;i<N-6;i++)lnmks[i].active=0;init=1;ps("[LCN] Leonotis initialized\n");return 0;}
int lcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(lnps,&st.n_lnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(lnss,&st.n_lne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lnvss,&st.n_lnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lcn_market(int t,int c,int a,int b,int d,int e,int y){return add(lnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lcn_report(void){ps("[LCN] Lnp: ");pi(st.n_lnp);ps(" PCS=");pi(st.t_f1);ps("\nLne: ");pi(st.n_lne);ps(" PCS=");pi(st.t_f2);ps("\nLnv: ");pi(st.n_lnv);ps(" PCS=");pi(st.t_f3);ps("\nLnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lcn_state(void){ps("[LCN] Lnp=");pi(st.n_lnp);ps(" Lne=");pi(st.n_lne);ps(" Lnv=");pi(st.n_lnv);ps(" Lnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leonotis Admin Demo ===\n\n");lcn_init();
ps("Leonotis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lcn_planning(t,c,746+(i*17),735+(i*14),715+(i*10),697+(i*6),2020+(i%5));}
ps("\nLeonotis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lcn_execution(t,c,735+(i*15),724+(i*12),706+(i*8),693+(i*5),2021+(i%4));}
ps("\nLeonotis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lcn_evaluation(t,c,727+(i*13),716+(i*10),700+(i*7),689+(i*4),2022+(i%3));}
ps("\nLeonotis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lcn_accessory(t,c,719+(i*11),710+(i*9),696+(i*6),686+(i*3),2023+(i%2));}
ps("\nLeonotis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lcn_market(t,c,713+(i*9),704+(i*7),691+(i*5),683+(i*3),2024);}
ps("\n");lcn_report();lcn_state();ps("\n=== Demo Complete ===\n");return 0;}
