/* lachenalia_admin: Lachenalia management technology administration (v1.0)
 * Lachenalia planning, lachenalia execution, lachenalia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lac_t;
typedef struct{int n_lacp,n_lace,n_lac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lac_state_t;
static lac_t lacps[N],lacss[N-2],lacvss[N-4],lacas[N-6],lacmks[N-6]; static lac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lac_init(void){if(init)return -1;st.n_lacp=0;st.n_lace=0;st.n_lac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lacps[i].active=0;for(int i=0;i<N-2;i++)lacss[i].active=0;for(int i=0;i<N-4;i++)lacvss[i].active=0;for(int i=0;i<N-6;i++)lacas[i].active=0;for(int i=0;i<N-6;i++)lacmks[i].active=0;init=1;ps("[LAC] Lachenalia initialized\n");return 0;}
int lac_planning(int t,int c,int a,int b,int d,int e,int y){return add(lacps,&st.n_lacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lac_execution(int t,int c,int a,int b,int d,int e,int y){return add(lacss,&st.n_lace,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lacvss,&st.n_lac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lacas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lac_market(int t,int c,int a,int b,int d,int e,int y){return add(lacmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lac_report(void){ps("[LAC] Lacp: ");pi(st.n_lacp);ps(" PCS=");pi(st.t_f1);ps("\nLace: ");pi(st.n_lace);ps(" PCS=");pi(st.t_f2);ps("\nLacv: ");pi(st.n_lac2);ps(" PCS=");pi(st.t_f3);ps("\nLacc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lac_state(void){ps("[LAC] Lacp=");pi(st.n_lacp);ps(" Lace=");pi(st.n_lace);ps(" Lacv=");pi(st.n_lac2);ps(" Lacc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lachenalia Admin Demo ===\n\n");lac_init();
ps("Lachenalia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lac_planning(t,c,1039+(i*17),1028+(i*14),1008+(i*10),990+(i*6),2020+(i%5));}
ps("\nLachenalia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lac_execution(t,c,1028+(i*15),1017+(i*12),999+(i*8),986+(i*5),2021+(i%4));}
ps("\nLachenalia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lac_evaluation(t,c,1020+(i*13),1009+(i*10),993+(i*7),982+(i*4),2022+(i%3));}
ps("\nLachenalia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lac_accessory(t,c,1012+(i*11),1003+(i*9),989+(i*6),979+(i*3),2023+(i%2));}
ps("\nLachenalia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lac_market(t,c,1006+(i*9),997+(i*7),984+(i*5),976+(i*3),2024);}
ps("\n");lac_report();lac_state();ps("\n=== Demo Complete ===\n");return 0;}
