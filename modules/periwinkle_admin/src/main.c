/* periwinkle_admin: Periwinkle management technology administration (v1.0)
 * Periwinkle planning, periwinkle execution, periwinkle evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} per_t;
typedef struct{int n_perp,n_pere,n_perv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} per_state_t;
static per_t perp[N],peres[N-2],pervs[N-4],acs[N-6],mks[N-6]; static per_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(per_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;per_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int per_init(void){if(init)return -1;st.n_perp=0;st.n_pere=0;st.n_perv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)perp[i].active=0;for(int i=0;i<N-2;i++)peres[i].active=0;for(int i=0;i<N-4;i++)pervs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PER] Periwinkle initialized\n");return 0;}
int per_planning(int t,int c,int a,int b,int d,int e,int y){return add(perp,&st.n_perp,&st.t_f1,N,t,c,a,b,d,e,y);}
int per_execution(int t,int c,int a,int b,int d,int e,int y){return add(peres,&st.n_pere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int per_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pervs,&st.n_perv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int per_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int per_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void per_report(void){ps("[PER] Perp: ");pi(st.n_perp);ps(" PCS=");pi(st.t_f1);ps("\nPere: ");pi(st.n_pere);ps(" PCS=");pi(st.t_f2);ps("\nPerv: ");pi(st.n_perv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void per_state(void){ps("[PER] Perp=");pi(st.n_perp);ps(" Pere=");pi(st.n_pere);ps(" Perv=");pi(st.n_perv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Periwinkle Admin Demo ===\n\n");per_init();
ps("Periwinkle planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;per_planning(t,c,590+(i*17),579+(i*14),559+(i*10),541+(i*6),2020+(i%5));}
ps("\nPeriwinkle execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;per_execution(t,c,579+(i*15),568+(i*12),550+(i*8),537+(i*5),2021+(i%4));}
ps("\nPeriwinkle evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;per_evaluation(t,c,571+(i*13),560+(i*10),544+(i*7),533+(i*4),2022+(i%3));}
ps("\nPeriwinkle accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;per_accessory(t,c,563+(i*11),554+(i*9),540+(i*6),530+(i*3),2023+(i%2));}
ps("\nPeriwinkle marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;per_market(t,c,557+(i*9),548+(i*7),535+(i*5),527+(i*3),2024);}
ps("\n");per_report();per_state();ps("\n=== Demo Complete ===\n");return 0;}
