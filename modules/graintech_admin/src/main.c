/* graintech_admin: Grain technology administration (v1.0)
 * Wheat planting, rice planting, corn planting, grain processing, grain storage
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} grn_t;
typedef struct{int n_wh,n_rc,n_cn,n_gp,n_gs,t_f1,t_f2,t_f3,t_f4,t_f5;} grn_state_t;
static grn_t whs[N],rcs[N-2],cns[N-4],gps[N-6],gss[N-6]; static grn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;grn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int grn_init(void){if(init)return -1;st.n_wh=0;st.n_rc=0;st.n_cn=0;st.n_gp=0;st.n_gs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)whs[i].active=0;for(int i=0;i<N-2;i++)rcs[i].active=0;for(int i=0;i<N-4;i++)cns[i].active=0;for(int i=0;i<N-6;i++)gps[i].active=0;for(int i=0;i<N-6;i++)gss[i].active=0;init=1;ps("[GRN] Graintech initialized\n");return 0;}
int grn_wheat(int t,int c,int a,int b,int d,int e,int y){return add(whs,&st.n_wh,&st.t_f1,N,t,c,a,b,d,e,y);}
int grn_rice(int t,int c,int a,int b,int d,int e,int y){return add(rcs,&st.n_rc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int grn_corn(int t,int c,int a,int b,int d,int e,int y){return add(cns,&st.n_cn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int grn_process(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int grn_storage(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void grn_report(void){ps("[GRN] Wheat: ");pi(st.n_wh);ps(" ha=");pi(st.t_f1);ps("\nRice: ");pi(st.n_rc);ps(" ha=");pi(st.t_f2);ps("\nCorn: ");pi(st.n_cn);ps(" ha=");pi(st.t_f3);ps("\nProc: ");pi(st.n_gp);ps(" Ton=");pi(st.t_f4);ps("\nStore: ");pi(st.n_gs);ps(" Ton=");pi(st.t_f5);ps("\n");}
void grn_state(void){ps("[GRN] Wh=");pi(st.n_wh);ps(" Rc=");pi(st.n_rc);ps(" Cn=");pi(st.n_cn);ps(" Gp=");pi(st.n_gp);ps(" Gs=");pi(st.n_gs);ps("\n");}
int main(void){
ps("=== Grain Tech Admin Demo ===\n\n");grn_init();
ps("Wheat planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;grn_wheat(t,c,179+(i*17),164+(i*14),144+(i*10),126+(i*6),2020+(i%5));}
ps("\nRice planting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;grn_rice(t,c,168+(i*15),154+(i*12),136+(i*8),123+(i*5),2021+(i%4));}
ps("\nCorn planting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;grn_corn(t,c,160+(i*13),146+(i*10),130+(i*7),119+(i*4),2022+(i%3));}
ps("\nGrain processing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grn_process(t,c,152+(i*11),140+(i*9),126+(i*6),116+(i*3),2023+(i%2));}
ps("\nGrain storage...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grn_storage(t,c,146+(i*9),135+(i*7),122+(i*5),114+(i*3),2024);}
ps("\n");grn_report();grn_state();ps("\n=== Demo Complete ===\n");return 0;}
