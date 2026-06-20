/* gazania_admin: Gazania management technology administration (v1.0)
 * Gazania planning, gazania execution, gazania evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gaz_t;
typedef struct{int n_gp,n_ge,n_gv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gaz_state_t;
static gaz_t gps[N],ges[N-2],gvs[N-4],acs[N-6],mks[N-6]; static gaz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gaz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gaz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gaz_init(void){if(init)return -1;st.n_gp=0;st.n_ge=0;st.n_gv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gps[i].active=0;for(int i=0;i<N-2;i++)ges[i].active=0;for(int i=0;i<N-4;i++)gvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GAZ] Gazania initialized\n");return 0;}
int gaz_planning(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gaz_execution(int t,int c,int a,int b,int d,int e,int y){return add(ges,&st.n_ge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gaz_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gvs,&st.n_gv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gaz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gaz_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gaz_report(void){ps("[GAZ] Gp: ");pi(st.n_gp);ps(" PCS=");pi(st.t_f1);ps("\nGe: ");pi(st.n_ge);ps(" PCS=");pi(st.t_f2);ps("\nGv: ");pi(st.n_gv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gaz_state(void){ps("[GAZ] Gp=");pi(st.n_gp);ps(" Ge=");pi(st.n_ge);ps(" Gv=");pi(st.n_gv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gazania Admin Demo ===\n\n");gaz_init();
ps("Gazania planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gaz_planning(t,c,571+(i*17),560+(i*14),540+(i*10),522+(i*6),2020+(i%5));}
ps("\nGazania execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gaz_execution(t,c,560+(i*15),549+(i*12),531+(i*8),518+(i*5),2021+(i%4));}
ps("\nGazania evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gaz_evaluation(t,c,552+(i*13),541+(i*10),525+(i*7),514+(i*4),2022+(i%3));}
ps("\nGazania accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gaz_accessory(t,c,544+(i*11),535+(i*9),521+(i*6),511+(i*3),2023+(i%2));}
ps("\nGazania marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gaz_market(t,c,538+(i*9),529+(i*7),516+(i*5),508+(i*3),2024);}
ps("\n");gaz_report();gaz_state();ps("\n=== Demo Complete ===\n");return 0;}
