/* nyctago_admin: Nyctago management technology administration (v1.0)
 * Nyctago planning, nyctago execution, nyctago evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nyc_t;
typedef struct{int n_nycp,n_nyce,n_nyc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nyc_state_t;
static nyc_t nycps[N],nycss[N-2],nycvss[N-4],nycas[N-6],nycmks[N-6]; static nyc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nyc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nyc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nyc_init(void){if(init)return -1;st.n_nycp=0;st.n_nyce=0;st.n_nyc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nycps[i].active=0;for(int i=0;i<N-2;i++)nycss[i].active=0;for(int i=0;i<N-4;i++)nycvss[i].active=0;for(int i=0;i<N-6;i++)nycas[i].active=0;for(int i=0;i<N-6;i++)nycmks[i].active=0;init=1;ps("[NYC] Nyctago initialized\n");return 0;}
int nyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(nycps,&st.n_nycp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(nycss,&st.n_nyce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nycvss,&st.n_nyc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nycas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nyc_market(int t,int c,int a,int b,int d,int e,int y){return add(nycmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nyc_report(void){ps("[NYC] Nycc: ");pi(st.n_nycp);ps(" PCS=");pi(st.t_f1);ps("\nNyce: ");pi(st.n_nyce);ps(" PCS=");pi(st.t_f2);ps("\nNycv: ");pi(st.n_nyc2);ps(" PCS=");pi(st.t_f3);ps("\nNycc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nyc_state(void){ps("[NYC] Nycc=");pi(st.n_nycp);ps(" Nyce=");pi(st.n_nyce);ps(" Nycv=");pi(st.n_nyc2);ps(" Nycc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nyctago Admin Demo ===\n\n");nyc_init();
ps("Nyctago planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nyc_planning(t,c,1069+(i*17),1058+(i*14),1038+(i*10),1020+(i*6),2020+(i%5));}
ps("\nNyctago execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nyc_execution(t,c,1058+(i*15),1047+(i*12),1029+(i*8),1016+(i*5),2021+(i%4));}
ps("\nNyctago evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nyc_evaluation(t,c,1050+(i*13),1039+(i*10),1023+(i*7),1012+(i*4),2022+(i%3));}
ps("\nNyctago accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nyc_accessory(t,c,1042+(i*11),1033+(i*9),1019+(i*6),1009+(i*3),2023+(i%2));}
ps("\nNyctago marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nyc_market(t,c,1036+(i*9),1027+(i*7),1014+(i*5),1006+(i*3),2024);}
ps("\n");nyc_report();nyc_state();ps("\n=== Demo Complete ===\n");return 0;}
