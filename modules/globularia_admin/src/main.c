/* globularia_admin: Globularia management technology administration (v1.0)
 * Globularia planning, globularia execution, globularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} glo_t;
typedef struct{int n_glop,n_gloe,n_glo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} glo_state_t;
static glo_t glops[N],gloss[N-2],glovss[N-4],gloas[N-6],glomks[N-6]; static glo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(glo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;glo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int glo_init(void){if(init)return -1;st.n_glop=0;st.n_gloe=0;st.n_glo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)glops[i].active=0;for(int i=0;i<N-2;i++)gloss[i].active=0;for(int i=0;i<N-4;i++)glovss[i].active=0;for(int i=0;i<N-6;i++)gloas[i].active=0;for(int i=0;i<N-6;i++)glomks[i].active=0;init=1;ps("[GLO] Globularia initialized\n");return 0;}
int glo_planning(int t,int c,int a,int b,int d,int e,int y){return add(glops,&st.n_glop,&st.t_f1,N,t,c,a,b,d,e,y);}
int glo_execution(int t,int c,int a,int b,int d,int e,int y){return add(gloss,&st.n_gloe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int glo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(glovss,&st.n_glo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int glo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gloas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int glo_market(int t,int c,int a,int b,int d,int e,int y){return add(glomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void glo_report(void){ps("[GLO] Glopp: ");pi(st.n_glop);ps(" PCS=");pi(st.t_f1);ps("\nGloe: ");pi(st.n_gloe);ps(" PCS=");pi(st.t_f2);ps("\nGlov: ");pi(st.n_glo2);ps(" PCS=");pi(st.t_f3);ps("\nGloc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void glo_state(void){ps("[GLO] Glopp=");pi(st.n_glop);ps(" Gloe=");pi(st.n_gloe);ps(" Glov=");pi(st.n_glo2);ps(" Gloc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Globularia Admin Demo ===\n\n");glo_init();
ps("Globularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;glo_planning(t,c,1146+(i*17),1135+(i*14),1115+(i*10),1097+(i*6),2020+(i%5));}
ps("\nGlobularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;glo_execution(t,c,1135+(i*15),1124+(i*12),1106+(i*8),1093+(i*5),2021+(i%4));}
ps("\nGlobularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;glo_evaluation(t,c,1127+(i*13),1116+(i*10),1100+(i*7),1089+(i*4),2022+(i%3));}
ps("\nGlobularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glo_accessory(t,c,1119+(i*11),1110+(i*9),1096+(i*6),1086+(i*3),2023+(i%2));}
ps("\nGlobularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glo_market(t,c,1113+(i*9),1104+(i*7),1091+(i*5),1083+(i*3),2024);}
ps("\n");glo_report();glo_state();ps("\n=== Demo Complete ===\n");return 0;}
