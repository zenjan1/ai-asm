/* geranium_admin: Geranium management technology administration (v1.0)
 * Geranium planning, geranium execution, geranium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gr_t;
typedef struct{int n_grp,n_gre,n_grv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gr_state_t;
static gr_t grps[N],grss[N-2],grvs[N-4],gras[N-6],grmks[N-6]; static gr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GRU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gr_init(void){if(init)return -1;st.n_grp=0;st.n_gre=0;st.n_grv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)grps[i].active=0;for(int i=0;i<N-2;i++)grss[i].active=0;for(int i=0;i<N-4;i++)grvs[i].active=0;for(int i=0;i<N-6;i++)gras[i].active=0;for(int i=0;i<N-6;i++)grmks[i].active=0;init=1;ps("[GRU] Geranium initialized\n");return 0;}
int gr_planning(int t,int c,int a,int b,int d,int e,int y){return add(grps,&st.n_grp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gr_execution(int t,int c,int a,int b,int d,int e,int y){return add(grss,&st.n_gre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(grvs,&st.n_grv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gr_market(int t,int c,int a,int b,int d,int e,int y){return add(grmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gr_report(void){ps("[GRU] Grp: ");pi(st.n_grp);ps(" PCS=");pi(st.t_f1);ps("\nGre: ");pi(st.n_gre);ps(" PCS=");pi(st.t_f2);ps("\nGrv: ");pi(st.n_grv);ps(" PCS=");pi(st.t_f3);ps("\nGrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gr_state(void){ps("[GRU] Grp=");pi(st.n_grp);ps(" Gre=");pi(st.n_gre);ps(" Grv=");pi(st.n_grv);ps(" Grc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Geranium Admin Demo ===\n\n");gr_init();
ps("Geranium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gr_planning(t,c,649+(i*17),638+(i*14),618+(i*10),600+(i*6),2020+(i%5));}
ps("\nGeranium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gr_execution(t,c,638+(i*15),627+(i*12),609+(i*8),596+(i*5),2021+(i%4));}
ps("\nGeranium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gr_evaluation(t,c,630+(i*13),619+(i*10),603+(i*7),592+(i*4),2022+(i%3));}
ps("\nGeranium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gr_accessory(t,c,622+(i*11),613+(i*9),599+(i*6),589+(i*3),2023+(i%2));}
ps("\nGeranium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gr_market(t,c,616+(i*9),607+(i*7),594+(i*5),586+(i*3),2024);}
ps("\n");gr_report();gr_state();ps("\n=== Demo Complete ===\n");return 0;}
