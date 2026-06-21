/* guarea_admin: Guarea management technology administration (v1.0)
 * Guarea planning, guarea execution, guarea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gua_t;
typedef struct{int n_guap,n_guae,n_gua2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gua_state_t;
static gua_t guaps[N],guass[N-2],guavss[N-4],guaas[N-6],guamks[N-6]; static gua_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gua_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gua_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gua_init(void){if(init)return -1;st.n_guap=0;st.n_guae=0;st.n_gua2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)guaps[i].active=0;for(int i=0;i<N-2;i++)guass[i].active=0;for(int i=0;i<N-4;i++)guavss[i].active=0;for(int i=0;i<N-6;i++)guaas[i].active=0;for(int i=0;i<N-6;i++)guamks[i].active=0;init=1;ps("[GUA] Guarea initialized\n");return 0;}
int gua_planning(int t,int c,int a,int b,int d,int e,int y){return add(guaps,&st.n_guap,&st.t_f1,N,t,c,a,b,d,e,y);}
int gua_execution(int t,int c,int a,int b,int d,int e,int y){return add(guass,&st.n_guae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gua_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(guavss,&st.n_gua2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gua_accessory(int t,int c,int a,int b,int d,int e,int y){return add(guaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gua_market(int t,int c,int a,int b,int d,int e,int y){return add(guamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gua_report(void){ps("[GUA] Guap: ");pi(st.n_guap);ps(" PCS=");pi(st.t_f1);ps("\nGuae: ");pi(st.n_guae);ps(" PCS=");pi(st.t_f2);ps("\nGuav: ");pi(st.n_gua2);ps(" PCS=");pi(st.t_f3);ps("\nGuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gua_state(void){ps("[GUA] Guap=");pi(st.n_guap);ps(" Guae=");pi(st.n_guae);ps(" Guav=");pi(st.n_gua2);ps(" Guac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Guarea Admin Demo ===\n\n");gua_init();
ps("Guarea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gua_planning(t,c,1015+(i*17),1004+(i*14),984+(i*10),966+(i*6),2020+(i%5));}
ps("\nGuarea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gua_execution(t,c,1004+(i*15),993+(i*12),975+(i*8),962+(i*5),2021+(i%4));}
ps("\nGuarea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gua_evaluation(t,c,996+(i*13),985+(i*10),969+(i*7),958+(i*4),2022+(i%3));}
ps("\nGuarea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gua_accessory(t,c,988+(i*11),979+(i*9),965+(i*6),955+(i*3),2023+(i%2));}
ps("\nGuarea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gua_market(t,c,982+(i*9),973+(i*7),960+(i*5),952+(i*3),2024);}
ps("\n");gua_report();gua_state();ps("\n=== Demo Complete ===\n");return 0;}
