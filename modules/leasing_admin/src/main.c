/* leasing_admin: Leasing administration (v1.0)
 * Equipment leasing, vehicle leasing, real estate leasing, lease accounting, lease pricing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lea_t;
typedef struct{int n_el,n_vl,n_rl,n_la,n_lp,t_f1,t_f2,t_f3,t_f4,t_f5;} lea_state_t;
static lea_t els[N],vls[N-2],rls[N-4],las[N-6],lps[N-6]; static lea_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lea_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lea_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lea_init(void){if(init)return -1;st.n_el=0;st.n_vl=0;st.n_rl=0;st.n_la=0;st.n_lp=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)els[i].active=0;for(int i=0;i<N-2;i++)vls[i].active=0;for(int i=0;i<N-4;i++)rls[i].active=0;for(int i=0;i<N-6;i++)las[i].active=0;for(int i=0;i<N-6;i++)lps[i].active=0;init=1;ps("[LEA] Leasing initialized\n");return 0;}
int lea_equipment(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f1,N,t,c,a,b,d,e,y);}
int lea_vehicle(int t,int c,int a,int b,int d,int e,int y){return add(vls,&st.n_vl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lea_realestate(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lea_account(int t,int c,int a,int b,int d,int e,int y){return add(las,&st.n_la,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lea_pricing(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lea_report(void){ps("[LEA] Equip: ");pi(st.n_el);ps(" Capex=");pi(st.t_f1);ps("\nVeh: ");pi(st.n_vl);ps(" Fleet=");pi(st.t_f2);ps("\nReal: ");pi(st.n_rl);ps(" Sqm=");pi(st.t_f3);ps("\nAcc: ");pi(st.n_la);ps(" IFRS=");pi(st.t_f4);ps("\nPrice: ");pi(st.n_lp);ps(" RV=");pi(st.t_f5);ps("\n");}
void lea_state(void){ps("[LEA] El=");pi(st.n_el);ps(" Vl=");pi(st.n_vl);ps(" Rl=");pi(st.n_rl);ps(" La=");pi(st.n_la);ps(" Lp=");pi(st.n_lp);ps("\n");}
int main(void){
ps("=== Leasing Admin Demo ===\n\n");lea_init();
ps("Equipment leasing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lea_equipment(t,c,123+(i*17),108+(i*14),88+(i*10),71+(i*6),2020+(i%5));}
ps("\nVehicle leasing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lea_vehicle(t,c,112+(i*15),98+(i*12),80+(i*8),67+(i*5),2021+(i%4));}
ps("\nReal estate leasing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lea_realestate(t,c,104+(i*13),90+(i*10),74+(i*7),63+(i*4),2022+(i%3));}
ps("\nLease accounting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lea_account(t,c,96+(i*11),84+(i*9),70+(i*6),60+(i*3),2023+(i%2));}
ps("\nLease pricing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lea_pricing(t,c,90+(i*9),79+(i*7),66+(i*5),58+(i*3),2024);}
ps("\n");lea_report();lea_state();ps("\n=== Demo Complete ===\n");return 0;}
