/* guapira_admin: Guapira management technology administration (v1.0)
 * Guapira planning, guapira execution, guapira evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gup_t;
typedef struct{int n_gupp,n_gupe,n_gup2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gup_state_t;
static gup_t gupps[N],gupss[N-2],gupvss[N-4],gupas[N-6],gupmks[N-6]; static gup_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gup_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gup_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gup_init(void){if(init)return -1;st.n_gupp=0;st.n_gupe=0;st.n_gup2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gupps[i].active=0;for(int i=0;i<N-2;i++)gupss[i].active=0;for(int i=0;i<N-4;i++)gupvss[i].active=0;for(int i=0;i<N-6;i++)gupas[i].active=0;for(int i=0;i<N-6;i++)gupmks[i].active=0;init=1;ps("[GUP] Guapira initialized\n");return 0;}
int gup_planning(int t,int c,int a,int b,int d,int e,int y){return add(gupps,&st.n_gupp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gup_execution(int t,int c,int a,int b,int d,int e,int y){return add(gupss,&st.n_gupe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gup_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gupvss,&st.n_gup2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gup_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gupas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gup_market(int t,int c,int a,int b,int d,int e,int y){return add(gupmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gup_report(void){ps("[GUP] Guap: ");pi(st.n_gupp);ps(" PCS=");pi(st.t_f1);ps("\nGuae: ");pi(st.n_gupe);ps(" PCS=");pi(st.t_f2);ps("\nGuav: ");pi(st.n_gup2);ps(" PCS=");pi(st.t_f3);ps("\nGuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gup_state(void){ps("[GUP] Guap=");pi(st.n_gupp);ps(" Guae=");pi(st.n_gupe);ps(" Guav=");pi(st.n_gup2);ps(" Guac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Guapira Admin Demo ===\n\n");gup_init();
ps("Guapira planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gup_planning(t,c,1290+(i*17),1279+(i*14),1259+(i*10),1241+(i*6),2020+(i%5));}
ps("\nGuapira execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gup_execution(t,c,1279+(i*15),1268+(i*12),1250+(i*8),1237+(i*5),2021+(i%4));}
ps("\nGuapira evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gup_evaluation(t,c,1271+(i*13),1260+(i*10),1244+(i*7),1233+(i*4),2022+(i%3));}
ps("\nGuapira accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gup_accessory(t,c,1263+(i*11),1254+(i*9),1240+(i*6),1230+(i*3),2023+(i%2));}
ps("\nGuapira marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gup_market(t,c,1257+(i*9),1248+(i*7),1235+(i*5),1227+(i*3),2024);}
ps("\n");gup_report();gup_state();ps("\n=== Demo Complete ===\n");return 0;}
