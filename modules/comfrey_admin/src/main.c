/* comfrey_admin: Comfrey management technology administration (v1.0)
 * Comfrey planning, comfrey execution, comfrey evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cf_t;
typedef struct{int n_cfp,n_cfe,n_cfv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cf_state_t;
static cf_t cfps[N],cfss[N-2],cfvss[N-4],cfas[N-6],cfmks[N-6]; static cf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CFR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cf_init(void){if(init)return -1;st.n_cfp=0;st.n_cfe=0;st.n_cfv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cfps[i].active=0;for(int i=0;i<N-2;i++)cfss[i].active=0;for(int i=0;i<N-4;i++)cfvss[i].active=0;for(int i=0;i<N-6;i++)cfas[i].active=0;for(int i=0;i<N-6;i++)cfmks[i].active=0;init=1;ps("[CFR] Comfrey initialized\n");return 0;}
int cf_planning(int t,int c,int a,int b,int d,int e,int y){return add(cfps,&st.n_cfp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cf_execution(int t,int c,int a,int b,int d,int e,int y){return add(cfss,&st.n_cfe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cfvss,&st.n_cfv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cfas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cf_market(int t,int c,int a,int b,int d,int e,int y){return add(cfmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cf_report(void){ps("[CFR] Cfp: ");pi(st.n_cfp);ps(" PCS=");pi(st.t_f1);ps("\nCfe: ");pi(st.n_cfe);ps(" PCS=");pi(st.t_f2);ps("\nCfv: ");pi(st.n_cfv);ps(" PCS=");pi(st.t_f3);ps("\nCfc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cf_state(void){ps("[CFR] Cfp=");pi(st.n_cfp);ps(" Cfe=");pi(st.n_cfe);ps(" Cfv=");pi(st.n_cfv);ps(" Cfc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Comfrey Admin Demo ===\n\n");cf_init();
ps("Comfrey planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cf_planning(t,c,693+(i*17),682+(i*14),662+(i*10),644+(i*6),2020+(i%5));}
ps("\nComfrey execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cf_execution(t,c,682+(i*15),671+(i*12),653+(i*8),640+(i*5),2021+(i%4));}
ps("\nComfrey evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cf_evaluation(t,c,674+(i*13),663+(i*10),647+(i*7),636+(i*4),2022+(i%3));}
ps("\nComfrey accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cf_accessory(t,c,666+(i*11),657+(i*9),643+(i*6),633+(i*3),2023+(i%2));}
ps("\nComfrey marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cf_market(t,c,660+(i*9),651+(i*7),638+(i*5),630+(i*3),2024);}
ps("\n");cf_report();cf_state();ps("\n=== Demo Complete ===\n");return 0;}
