/* peony_admin: Peony management technology administration (v1.0)
 * Peony planning, peony execution, peony evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} peo_t;
typedef struct{int n_pp,n_pe,n_pv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} peo_state_t;
static peo_t pps[N],pes[N-2],pvs[N-4],acs[N-6],mks[N-6]; static peo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(peo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;peo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PEO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int peo_init(void){if(init)return -1;st.n_pp=0;st.n_pe=0;st.n_pv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)pes[i].active=0;for(int i=0;i<N-4;i++)pvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PEO] Peony initialized\n");return 0;}
int peo_planning(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int peo_execution(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int peo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int peo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int peo_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void peo_report(void){ps("[PEO] Pp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f1);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f2);ps("\nPv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void peo_state(void){ps("[PEO] Pp=");pi(st.n_pp);ps(" Pe=");pi(st.n_pe);ps(" Pv=");pi(st.n_pv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Peony Admin Demo ===\n\n");peo_init();
ps("Peony planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;peo_planning(t,c,544+(i*17),533+(i*14),513+(i*10),495+(i*6),2020+(i%5));}
ps("\nPeony execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;peo_execution(t,c,533+(i*15),522+(i*12),504+(i*8),491+(i*5),2021+(i%4));}
ps("\nPeony evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;peo_evaluation(t,c,525+(i*13),514+(i*10),498+(i*7),487+(i*4),2022+(i%3));}
ps("\nPeony accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;peo_accessory(t,c,517+(i*11),508+(i*9),494+(i*6),484+(i*3),2023+(i%2));}
ps("\nPeony marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;peo_market(t,c,511+(i*9),502+(i*7),489+(i*5),481+(i*3),2024);}
ps("\n");peo_report();peo_state();ps("\n=== Demo Complete ===\n");return 0;}
