/* abrasive_admin: Abrasive technology administration (v1.0)
 * Bonded abrasive, coated abrasive, superhard abrasive, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} abr_t;
typedef struct{int n_bd,n_ct,n_sh,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} abr_state_t;
static abr_t bds[N],cts[N-2],shs[N-4],acs[N-6],mks[N-6]; static abr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(abr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;abr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ABR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int abr_init(void){if(init)return -1;st.n_bd=0;st.n_ct=0;st.n_sh=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bds[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)shs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ABR] Abrasive initialized\n");return 0;}
int abr_bonded(int t,int c,int a,int b,int d,int e,int y){return add(bds,&st.n_bd,&st.t_f1,N,t,c,a,b,d,e,y);}
int abr_coated(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int abr_superhard(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int abr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int abr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void abr_report(void){ps("[ABR] Bd: ");pi(st.n_bd);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nSh: ");pi(st.n_sh);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void abr_state(void){ps("[ABR] Bd=");pi(st.n_bd);ps(" Ct=");pi(st.n_ct);ps(" Sh=");pi(st.n_sh);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Abrasive Admin Demo ===\n\n");abr_init();
ps("Bonded abrasives...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;abr_bonded(t,c,389+(i*17),374+(i*14),354+(i*10),336+(i*6),2020+(i%5));}
ps("\nCoated abrasives...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;abr_coated(t,c,378+(i*15),364+(i*12),346+(i*8),333+(i*5),2021+(i%4));}
ps("\nSuperhard abrasives...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;abr_superhard(t,c,370+(i*13),356+(i*10),340+(i*7),329+(i*4),2022+(i%3));}
ps("\nAbrasive accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abr_accessory(t,c,362+(i*11),350+(i*9),336+(i*6),326+(i*3),2023+(i%2));}
ps("\nAbrasive marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abr_market(t,c,356+(i*9),345+(i*7),332+(i*5),324+(i*3),2024);}
ps("\n");abr_report();abr_state();ps("\n=== Demo Complete ===\n");return 0;}
