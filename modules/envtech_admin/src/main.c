/* envtech_admin: Environmental technology administration (v1.0)
 * Water treatment, air purification, waste treatment, environmental monitoring, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} env_t;
typedef struct{int n_wt,n_ap,n_wst,n_em,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} env_state_t;
static env_t wts[N],aps[N-2],wsts[N-4],ems[N-6],mks[N-6]; static env_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(env_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;env_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ENV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int env_init(void){if(init)return -1;st.n_wt=0;st.n_ap=0;st.n_wst=0;st.n_em=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wts[i].active=0;for(int i=0;i<N-2;i++)aps[i].active=0;for(int i=0;i<N-4;i++)wsts[i].active=0;for(int i=0;i<N-6;i++)ems[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ENV] Envtech initialized\n");return 0;}
int env_water(int t,int c,int a,int b,int d,int e,int y){return add(wts,&st.n_wt,&st.t_f1,N,t,c,a,b,d,e,y);}
int env_air(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int env_waste(int t,int c,int a,int b,int d,int e,int y){return add(wsts,&st.n_wst,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int env_monitor(int t,int c,int a,int b,int d,int e,int y){return add(ems,&st.n_em,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int env_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void env_report(void){ps("[ENV] Wt: ");pi(st.n_wt);ps(" Ton=");pi(st.t_f1);ps("\nAp: ");pi(st.n_ap);ps(" PCS=");pi(st.t_f2);ps("\nWst: ");pi(st.n_wst);ps(" Ton=");pi(st.t_f3);ps("\nEm: ");pi(st.n_em);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void env_state(void){ps("[ENV] Wt=");pi(st.n_wt);ps(" Ap=");pi(st.n_ap);ps(" Wst=");pi(st.n_wst);ps(" Em=");pi(st.n_em);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Environmental Tech Admin Demo ===\n\n");env_init();
ps("Water treatment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;env_water(t,c,230+(i*17),215+(i*14),195+(i*10),177+(i*6),2020+(i%5));}
ps("\nAir purification...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;env_air(t,c,219+(i*15),205+(i*12),187+(i*8),174+(i*5),2021+(i%4));}
ps("\nWaste treatment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;env_waste(t,c,211+(i*13),197+(i*10),181+(i*7),170+(i*4),2022+(i%3));}
ps("\nEnvironmental monitoring...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;env_monitor(t,c,203+(i*11),191+(i*9),177+(i*6),167+(i*3),2023+(i%2));}
ps("\nEnvironmental marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;env_market(t,c,197+(i*9),186+(i*7),173+(i*5),165+(i*3),2024);}
ps("\n");env_report();env_state();ps("\n=== Demo Complete ===\n");return 0;}
