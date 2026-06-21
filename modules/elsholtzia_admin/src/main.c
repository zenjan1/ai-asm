/* elsholtzia_admin: Elsholtzia management technology administration (v1.0)
 * Elsholtzia planning, elsholtzia execution, elsholtzia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} els_t;
typedef struct{int n_elsp,n_else,n_els2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} els_state_t;
static els_t elsps[N],elsss[N-2],elsvss[N-4],elsas[N-6],elsmks[N-6]; static els_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(els_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;els_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int els_init(void){if(init)return -1;st.n_elsp=0;st.n_else=0;st.n_els2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)elsps[i].active=0;for(int i=0;i<N-2;i++)elsss[i].active=0;for(int i=0;i<N-4;i++)elsvss[i].active=0;for(int i=0;i<N-6;i++)elsas[i].active=0;for(int i=0;i<N-6;i++)elsmks[i].active=0;init=1;ps("[ELS] Elsholtzia initialized\n");return 0;}
int els_planning(int t,int c,int a,int b,int d,int e,int y){return add(elsps,&st.n_elsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int els_execution(int t,int c,int a,int b,int d,int e,int y){return add(elsss,&st.n_else,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int els_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(elsvss,&st.n_els2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int els_accessory(int t,int c,int a,int b,int d,int e,int y){return add(elsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int els_market(int t,int c,int a,int b,int d,int e,int y){return add(elsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void els_report(void){ps("[ELS] Elsp: ");pi(st.n_elsp);ps(" PCS=");pi(st.t_f1);ps("\nElse: ");pi(st.n_else);ps(" PCS=");pi(st.t_f2);ps("\nElsv: ");pi(st.n_els2);ps(" PCS=");pi(st.t_f3);ps("\nElsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void els_state(void){ps("[ELS] Elsp=");pi(st.n_elsp);ps(" Else=");pi(st.n_else);ps(" Elsv=");pi(st.n_els2);ps(" Elsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Elsholtzia Admin Demo ===\n\n");els_init();
ps("Elsholtzia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;els_planning(t,c,920+(i*17),909+(i*14),889+(i*10),871+(i*6),2020+(i%5));}
ps("\nElsholtzia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;els_execution(t,c,909+(i*15),898+(i*12),880+(i*8),867+(i*5),2021+(i%4));}
ps("\nElsholtzia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;els_evaluation(t,c,901+(i*13),890+(i*10),874+(i*7),863+(i*4),2022+(i%3));}
ps("\nElsholtzia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;els_accessory(t,c,893+(i*11),884+(i*9),870+(i*6),860+(i*3),2023+(i%2));}
ps("\nElsholtzia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;els_market(t,c,887+(i*9),878+(i*7),865+(i*5),857+(i*3),2024);}
ps("\n");els_report();els_state();ps("\n=== Demo Complete ===\n");return 0;}
