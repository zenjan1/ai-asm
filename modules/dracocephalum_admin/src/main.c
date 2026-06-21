/* dracocephalum_admin: Dracocephalum management technology administration (v1.0)
 * Dracocephalum planning, dracocephalum execution, dracocephalum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dra_t;
typedef struct{int n_drap,n_drae,n_dra2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dra_state_t;
static dra_t draps[N],drass[N-2],dravss[N-4],dras[N-6],dramks[N-6]; static dra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dra_init(void){if(init)return -1;st.n_drap=0;st.n_drae=0;st.n_dra2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)draps[i].active=0;for(int i=0;i<N-2;i++)drass[i].active=0;for(int i=0;i<N-4;i++)dravss[i].active=0;for(int i=0;i<N-6;i++)dras[i].active=0;for(int i=0;i<N-6;i++)dramks[i].active=0;init=1;ps("[DRA] Dracocephalum initialized\n");return 0;}
int dra_planning(int t,int c,int a,int b,int d,int e,int y){return add(draps,&st.n_drap,&st.t_f1,N,t,c,a,b,d,e,y);}
int dra_execution(int t,int c,int a,int b,int d,int e,int y){return add(drass,&st.n_drae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dra_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dravss,&st.n_dra2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dra_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dra_market(int t,int c,int a,int b,int d,int e,int y){return add(dramks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dra_report(void){ps("[DRA] Drap: ");pi(st.n_drap);ps(" PCS=");pi(st.t_f1);ps("\nDrae: ");pi(st.n_drae);ps(" PCS=");pi(st.t_f2);ps("\nDrav: ");pi(st.n_dra2);ps(" PCS=");pi(st.t_f3);ps("\nDrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dra_state(void){ps("[DRA] Drap=");pi(st.n_drap);ps(" Drae=");pi(st.n_drae);ps(" Drav=");pi(st.n_dra2);ps(" Drac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dracocephalum Admin Demo ===\n\n");dra_init();
ps("Dracocephalum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dra_planning(t,c,917+(i*17),906+(i*14),886+(i*10),868+(i*6),2020+(i%5));}
ps("\nDracocephalum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dra_execution(t,c,906+(i*15),895+(i*12),877+(i*8),864+(i*5),2021+(i%4));}
ps("\nDracocephalum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dra_evaluation(t,c,898+(i*13),887+(i*10),871+(i*7),860+(i*4),2022+(i%3));}
ps("\nDracocephalum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dra_accessory(t,c,890+(i*11),881+(i*9),867+(i*6),857+(i*3),2023+(i%2));}
ps("\nDracocephalum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dra_market(t,c,884+(i*9),875+(i*7),862+(i*5),854+(i*3),2024);}
ps("\n");dra_report();dra_state();ps("\n=== Demo Complete ===\n");return 0;}
