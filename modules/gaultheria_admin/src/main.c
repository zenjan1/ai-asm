/* gaultheria_admin: Gaultheria management technology administration (v1.0)
 * Gaultheria planning, gaultheria execution, gaultheria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gau_t;
typedef struct{int n_gaup,n_gaue,n_gau2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gau_state_t;
static gau_t gaups[N],gauss[N-2],gavss[N-4],gaas[N-6],gamks[N-6]; static gau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gau_init(void){if(init)return -1;st.n_gaup=0;st.n_gaue=0;st.n_gau2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gaups[i].active=0;for(int i=0;i<N-2;i++)gauss[i].active=0;for(int i=0;i<N-4;i++)gavss[i].active=0;for(int i=0;i<N-6;i++)gaas[i].active=0;for(int i=0;i<N-6;i++)gamks[i].active=0;init=1;ps("[GAU] Gaultheria initialized\n");return 0;}
int gau_planning(int t,int c,int a,int b,int d,int e,int y){return add(gaups,&st.n_gaup,&st.t_f1,N,t,c,a,b,d,e,y);}
int gau_execution(int t,int c,int a,int b,int d,int e,int y){return add(gauss,&st.n_gaue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gavss,&st.n_gau2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gau_market(int t,int c,int a,int b,int d,int e,int y){return add(gamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gau_report(void){ps("[GAU] Gaup: ");pi(st.n_gaup);ps(" PCS=");pi(st.t_f1);ps("\nGaue: ");pi(st.n_gaue);ps(" PCS=");pi(st.t_f2);ps("\nGauv: ");pi(st.n_gau2);ps(" PCS=");pi(st.t_f3);ps("\nGaoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gau_state(void){ps("[GAU] Gaup=");pi(st.n_gaup);ps(" Gaue=");pi(st.n_gaue);ps(" Gauv=");pi(st.n_gau2);ps(" Gaoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gaultheria Admin Demo ===\n\n");gau_init();
ps("Gaultheria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gau_planning(t,c,925+(i*17),914+(i*14),894+(i*10),876+(i*6),2020+(i%5));}
ps("\nGaultheria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gau_execution(t,c,914+(i*15),903+(i*12),885+(i*8),872+(i*5),2021+(i%4));}
ps("\nGaultheria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gau_evaluation(t,c,906+(i*13),895+(i*10),879+(i*7),868+(i*4),2022+(i%3));}
ps("\nGaultheria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gau_accessory(t,c,898+(i*11),889+(i*9),875+(i*6),865+(i*3),2023+(i%2));}
ps("\nGaultheria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gau_market(t,c,892+(i*9),883+(i*7),870+(i*5),862+(i*3),2024);}
ps("\n");gau_report();gau_state();ps("\n=== Demo Complete ===\n");return 0;}
