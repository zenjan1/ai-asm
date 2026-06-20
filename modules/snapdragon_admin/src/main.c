/* snapdragon_admin: Snapdragon management technology administration (v1.0)
 * Snapdragon planning, snapdragon execution, snapdragon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sna_t;
typedef struct{int n_sp,n_se,n_sv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sna_state_t;
static sna_t sps[N],ses[N-2],svs[N-4],acs[N-6],mks[N-6]; static sna_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sna_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sna_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SNA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sna_init(void){if(init)return -1;st.n_sp=0;st.n_se=0;st.n_sv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sps[i].active=0;for(int i=0;i<N-2;i++)ses[i].active=0;for(int i=0;i<N-4;i++)svs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SNA] Snapdragon initialized\n");return 0;}
int sna_planning(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sna_execution(int t,int c,int a,int b,int d,int e,int y){return add(ses,&st.n_se,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sna_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(svs,&st.n_sv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sna_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sna_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sna_report(void){ps("[SNA] Sp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f1);ps("\nSe: ");pi(st.n_se);ps(" PCS=");pi(st.t_f2);ps("\nSv: ");pi(st.n_sv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sna_state(void){ps("[SNA] Sp=");pi(st.n_sp);ps(" Se=");pi(st.n_se);ps(" Sv=");pi(st.n_sv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Snapdragon Admin Demo ===\n\n");sna_init();
ps("Snapdragon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sna_planning(t,c,562+(i*17),551+(i*14),531+(i*10),513+(i*6),2020+(i%5));}
ps("\nSnapdragon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sna_execution(t,c,551+(i*15),540+(i*12),522+(i*8),509+(i*5),2021+(i%4));}
ps("\nSnapdragon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sna_evaluation(t,c,543+(i*13),532+(i*10),516+(i*7),505+(i*4),2022+(i%3));}
ps("\nSnapdragon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sna_accessory(t,c,535+(i*11),526+(i*9),512+(i*6),502+(i*3),2023+(i%2));}
ps("\nSnapdragon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sna_market(t,c,529+(i*9),520+(i*7),507+(i*5),499+(i*3),2024);}
ps("\n");sna_report();sna_state();ps("\n=== Demo Complete ===\n");return 0;}
