/* foxglove_admin: Foxglove management technology administration (v1.0)
 * Foxglove planning, foxglove execution, foxglove evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fox_t;
typedef struct{int n_fop,n_foe,n_fov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fox_state_t;
static fox_t fops[N],foes[N-2],fovs[N-4],acs[N-6],mks[N-6]; static fox_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fox_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fox_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FOX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fox_init(void){if(init)return -1;st.n_fop=0;st.n_foe=0;st.n_fov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fops[i].active=0;for(int i=0;i<N-2;i++)foes[i].active=0;for(int i=0;i<N-4;i++)fovs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FOX] Foxglove initialized\n");return 0;}
int fox_planning(int t,int c,int a,int b,int d,int e,int y){return add(fops,&st.n_fop,&st.t_f1,N,t,c,a,b,d,e,y);}
int fox_execution(int t,int c,int a,int b,int d,int e,int y){return add(foes,&st.n_foe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fox_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fovs,&st.n_fov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fox_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fox_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fox_report(void){ps("[FOX] Fop: ");pi(st.n_fop);ps(" PCS=");pi(st.t_f1);ps("\nFoe: ");pi(st.n_foe);ps(" PCS=");pi(st.t_f2);ps("\nFov: ");pi(st.n_fov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fox_state(void){ps("[FOX] Fop=");pi(st.n_fop);ps(" Foe=");pi(st.n_foe);ps(" Fov=");pi(st.n_fov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Foxglove Admin Demo ===\n\n");fox_init();
ps("Foxglove planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fox_planning(t,c,575+(i*17),564+(i*14),544+(i*10),526+(i*6),2020+(i%5));}
ps("\nFoxglove execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fox_execution(t,c,564+(i*15),553+(i*12),535+(i*8),522+(i*5),2021+(i%4));}
ps("\nFoxglove evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fox_evaluation(t,c,556+(i*13),545+(i*10),529+(i*7),518+(i*4),2022+(i%3));}
ps("\nFoxglove accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fox_accessory(t,c,548+(i*11),539+(i*9),525+(i*6),515+(i*3),2023+(i%2));}
ps("\nFoxglove marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fox_market(t,c,542+(i*9),533+(i*7),520+(i*5),512+(i*3),2024);}
ps("\n");fox_report();fox_state();ps("\n=== Demo Complete ===\n");return 0;}
