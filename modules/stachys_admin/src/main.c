/* stachys_admin: Stachys management technology administration (v1.0)
 * Stachys planning, stachys execution, stachys evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sta_t;
typedef struct{int n_stap,n_stae,n_sta2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sta_state_t;
static sta_t staps[N],stass[N-2],stavss[N-4],stas[N-6],stamks[N-6]; static sta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sta_init(void){if(init)return -1;st.n_stap=0;st.n_stae=0;st.n_sta2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)staps[i].active=0;for(int i=0;i<N-2;i++)stass[i].active=0;for(int i=0;i<N-4;i++)stavss[i].active=0;for(int i=0;i<N-6;i++)stas[i].active=0;for(int i=0;i<N-6;i++)stamks[i].active=0;init=1;ps("[STA] Stachys initialized\n");return 0;}
int sta_planning(int t,int c,int a,int b,int d,int e,int y){return add(staps,&st.n_stap,&st.t_f1,N,t,c,a,b,d,e,y);}
int sta_execution(int t,int c,int a,int b,int d,int e,int y){return add(stass,&st.n_stae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sta_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(stavss,&st.n_sta2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sta_accessory(int t,int c,int a,int b,int d,int e,int y){return add(stas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sta_market(int t,int c,int a,int b,int d,int e,int y){return add(stamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sta_report(void){ps("[STA] Stap: ");pi(st.n_stap);ps(" PCS=");pi(st.t_f1);ps("\nStae: ");pi(st.n_stae);ps(" PCS=");pi(st.t_f2);ps("\nStav: ");pi(st.n_sta2);ps(" PCS=");pi(st.t_f3);ps("\nStac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sta_state(void){ps("[STA] Stap=");pi(st.n_stap);ps(" Stae=");pi(st.n_stae);ps(" Stav=");pi(st.n_sta2);ps(" Stac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Stachys Admin Demo ===\n\n");sta_init();
ps("Stachys planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sta_planning(t,c,949+(i*17),938+(i*14),918+(i*10),900+(i*6),2020+(i%5));}
ps("\nStachys execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sta_execution(t,c,938+(i*15),927+(i*12),909+(i*8),896+(i*5),2021+(i%4));}
ps("\nStachys evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sta_evaluation(t,c,930+(i*13),919+(i*10),903+(i*7),892+(i*4),2022+(i%3));}
ps("\nStachys accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sta_accessory(t,c,922+(i*11),913+(i*9),899+(i*6),889+(i*3),2023+(i%2));}
ps("\nStachys marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sta_market(t,c,916+(i*9),907+(i*7),894+(i*5),886+(i*3),2024);}
ps("\n");sta_report();sta_state();ps("\n=== Demo Complete ===\n");return 0;}
