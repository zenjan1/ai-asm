/* tiger_lily_admin: Tiger lily management technology administration (v1.0)
 * Tiger lily planning, tiger lily execution, tiger lily evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ti_t;
typedef struct{int n_tgp,n_tge,n_tgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ti_state_t;
static ti_t tgps[N],tgss[N-2],tgvss[N-4],tgas[N-6],tgmks[N-6]; static ti_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ti_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ti_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TGR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tg_init(void){if(init)return -1;st.n_tgp=0;st.n_tge=0;st.n_tgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tgps[i].active=0;for(int i=0;i<N-2;i++)tgss[i].active=0;for(int i=0;i<N-4;i++)tgvss[i].active=0;for(int i=0;i<N-6;i++)tgas[i].active=0;for(int i=0;i<N-6;i++)tgmks[i].active=0;init=1;ps("[TGR] Tiger lily initialized\n");return 0;}
int tg_planning(int t,int c,int a,int b,int d,int e,int y){return add(tgps,&st.n_tgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tg_execution(int t,int c,int a,int b,int d,int e,int y){return add(tgss,&st.n_tge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tgvss,&st.n_tgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tg_market(int t,int c,int a,int b,int d,int e,int y){return add(tgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tg_report(void){ps("[TGR] Tgp: ");pi(st.n_tgp);ps(" PCS=");pi(st.t_f1);ps("\nTge: ");pi(st.n_tge);ps(" PCS=");pi(st.t_f2);ps("\nTgv: ");pi(st.n_tgv);ps(" PCS=");pi(st.t_f3);ps("\nTgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tg_state(void){ps("[TGR] Tgp=");pi(st.n_tgp);ps(" Tge=");pi(st.n_tge);ps(" Tgv=");pi(st.n_tgv);ps(" Tgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tiger Lily Admin Demo ===\n\n");tg_init();
ps("Tiger lily planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tg_planning(t,c,716+(i*17),705+(i*14),685+(i*10),667+(i*6),2020+(i%5));}
ps("\nTiger lily execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tg_execution(t,c,705+(i*15),694+(i*12),676+(i*8),663+(i*5),2021+(i%4));}
ps("\nTiger lily evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tg_evaluation(t,c,697+(i*13),686+(i*10),670+(i*7),659+(i*4),2022+(i%3));}
ps("\nTiger lily accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tg_accessory(t,c,689+(i*11),680+(i*9),666+(i*6),656+(i*3),2023+(i%2));}
ps("\nTiger lily marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tg_market(t,c,683+(i*9),674+(i*7),661+(i*5),653+(i*3),2024);}
ps("\n");tg_report();tg_state();ps("\n=== Demo Complete ===\n");return 0;}
