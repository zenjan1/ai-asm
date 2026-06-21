/* nicotiana_admin: Nicotiana management technology administration (v1.0)
 * Nicotiana planning, nicotiana execution, nicotiana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nc_t;
typedef struct{int n_ncp,n_nce,n_ncv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nc_state_t;
static nc_t ncps[N],ncss[N-2],ncvss[N-4],ncas[N-6],ncmks[N-6]; static nc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ncn_init(void){if(init)return -1;st.n_ncp=0;st.n_nce=0;st.n_ncv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ncps[i].active=0;for(int i=0;i<N-2;i++)ncss[i].active=0;for(int i=0;i<N-4;i++)ncvss[i].active=0;for(int i=0;i<N-6;i++)ncas[i].active=0;for(int i=0;i<N-6;i++)ncmks[i].active=0;init=1;ps("[NCN] Nicotiana initialized\n");return 0;}
int ncn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ncps,&st.n_ncp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ncn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ncss,&st.n_nce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ncn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ncvss,&st.n_ncv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ncn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ncas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ncn_market(int t,int c,int a,int b,int d,int e,int y){return add(ncmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ncn_report(void){ps("[NCN] Ncp: ");pi(st.n_ncp);ps(" PCS=");pi(st.t_f1);ps("\nNce: ");pi(st.n_nce);ps(" PCS=");pi(st.t_f2);ps("\nNcv: ");pi(st.n_ncv);ps(" PCS=");pi(st.t_f3);ps("\nNcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ncn_state(void){ps("[NCN] Ncp=");pi(st.n_ncp);ps(" Nce=");pi(st.n_nce);ps(" Ncv=");pi(st.n_ncv);ps(" Ncc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nicotiana Admin Demo ===\n\n");ncn_init();
ps("Nicotiana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ncn_planning(t,c,747+(i*17),736+(i*14),716+(i*10),698+(i*6),2020+(i%5));}
ps("\nNicotiana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ncn_execution(t,c,736+(i*15),725+(i*12),707+(i*8),694+(i*5),2021+(i%4));}
ps("\nNicotiana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ncn_evaluation(t,c,728+(i*13),717+(i*10),701+(i*7),690+(i*4),2022+(i%3));}
ps("\nNicotiana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ncn_accessory(t,c,720+(i*11),711+(i*9),697+(i*6),687+(i*3),2023+(i%2));}
ps("\nNicotiana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ncn_market(t,c,714+(i*9),705+(i*7),692+(i*5),684+(i*3),2024);}
ps("\n");ncn_report();ncn_state();ps("\n=== Demo Complete ===\n");return 0;}
