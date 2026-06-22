/* glycyrrhiza_admin: Glycyrrhiza management technology administration (v1.0)
 * Glycyrrhiza planning, glycyrrhiza execution, glycyrrhiza evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gly_t;
typedef struct{int n_glyp,n_glye,n_gly2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gly_state_t;
static gly_t glyps[N],glyss[N-2],glyvss[N-4],glyas[N-6],glymks[N-6]; static gly_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gly_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gly_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gly_init(void){if(init)return -1;st.n_glyp=0;st.n_glye=0;st.n_gly2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)glyps[i].active=0;for(int i=0;i<N-2;i++)glyss[i].active=0;for(int i=0;i<N-4;i++)glyvss[i].active=0;for(int i=0;i<N-6;i++)glyas[i].active=0;for(int i=0;i<N-6;i++)glymks[i].active=0;init=1;ps("[GLY] Glycyrrhiza initialized\n");return 0;}
int gly_planning(int t,int c,int a,int b,int d,int e,int y){return add(glyps,&st.n_glyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gly_execution(int t,int c,int a,int b,int d,int e,int y){return add(glyss,&st.n_glye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gly_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(glyvss,&st.n_gly2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gly_accessory(int t,int c,int a,int b,int d,int e,int y){return add(glyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gly_market(int t,int c,int a,int b,int d,int e,int y){return add(glymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gly_report(void){ps("[GLY] Glyp: ");pi(st.n_glyp);ps(" PCS=");pi(st.t_f1);ps("\nGlye: ");pi(st.n_glye);ps(" PCS=");pi(st.t_f2);ps("\nGlyv: ");pi(st.n_gly2);ps(" PCS=");pi(st.t_f3);ps("\nGlyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gly_state(void){ps("[GLY] Glyp=");pi(st.n_glyp);ps(" Glye=");pi(st.n_glye);ps(" Glyv=");pi(st.n_gly2);ps(" Glyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Glycyrrhiza Admin Demo ===\n\n");gly_init();
ps("Glycyrrhiza planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gly_planning(t,c,1287+(i*17),1276+(i*14),1256+(i*10),1238+(i*6),2020+(i%5));}
ps("\nGlycyrrhiza execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gly_execution(t,c,1276+(i*15),1265+(i*12),1247+(i*8),1234+(i*5),2021+(i%4));}
ps("\nGlycyrrhiza evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gly_evaluation(t,c,1268+(i*13),1257+(i*10),1241+(i*7),1230+(i*4),2022+(i%3));}
ps("\nGlycyrrhiza accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gly_accessory(t,c,1260+(i*11),1251+(i*9),1237+(i*6),1227+(i*3),2023+(i%2));}
ps("\nGlycyrrhiza marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gly_market(t,c,1254+(i*9),1245+(i*7),1232+(i*5),1224+(i*3),2024);}
ps("\n");gly_report();gly_state();ps("\n=== Demo Complete ===\n");return 0;}
