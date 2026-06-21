/* gypsophila_admin: Gypsophila management technology administration (v1.0)
 * Gypsophila planning, gypsophila execution, gypsophila evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gy_t;
typedef struct{int n_gyp,n_gye,n_gyv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gy_state_t;
static gy_t gypp[N],gyss[N-2],gyvss[N-4],gyas[N-6],gymks[N-6]; static gy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GYP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gyp_init(void){if(init)return -1;st.n_gyp=0;st.n_gye=0;st.n_gyv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gypp[i].active=0;for(int i=0;i<N-2;i++)gyss[i].active=0;for(int i=0;i<N-4;i++)gyvss[i].active=0;for(int i=0;i<N-6;i++)gyas[i].active=0;for(int i=0;i<N-6;i++)gymks[i].active=0;init=1;ps("[GYP] Gypsophila initialized\n");return 0;}
int gyp_planning(int t,int c,int a,int b,int d,int e,int y){return add(gypp,&st.n_gyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gyp_execution(int t,int c,int a,int b,int d,int e,int y){return add(gyss,&st.n_gye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gyp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gyvss,&st.n_gyv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gyp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gyp_market(int t,int c,int a,int b,int d,int e,int y){return add(gymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gyp_report(void){ps("[GYP] Gyp: ");pi(st.n_gyp);ps(" PCS=");pi(st.t_f1);ps("\nGye: ");pi(st.n_gye);ps(" PCS=");pi(st.t_f2);ps("\nGyv: ");pi(st.n_gyv);ps(" PCS=");pi(st.t_f3);ps("\nGyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gyp_state(void){ps("[GYP] Gyp=");pi(st.n_gyp);ps(" Gye=");pi(st.n_gye);ps(" Gyv=");pi(st.n_gyv);ps(" Gyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gypsophila Admin Demo ===\n\n");gyp_init();
ps("Gypsophila planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gyp_planning(t,c,734+(i*17),723+(i*14),703+(i*10),685+(i*6),2020+(i%5));}
ps("\nGypsophila execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gyp_execution(t,c,723+(i*15),712+(i*12),694+(i*8),681+(i*5),2021+(i%4));}
ps("\nGypsophila evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gyp_evaluation(t,c,715+(i*13),704+(i*10),688+(i*7),677+(i*4),2022+(i%3));}
ps("\nGypsophila accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gyp_accessory(t,c,707+(i*11),698+(i*9),684+(i*6),674+(i*3),2023+(i%2));}
ps("\nGypsophila marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gyp_market(t,c,701+(i*9),692+(i*7),679+(i*5),671+(i*3),2024);}
ps("\n");gyp_report();gyp_state();ps("\n=== Demo Complete ===\n");return 0;}
