/* begonia_admin: Begonia management technology administration (v1.0)
 * Begonia planning, begonia execution, begonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} beg_t;
typedef struct{int n_bp,n_be,n_bv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} beg_state_t;
static beg_t bps[N],bes[N-2],bvs[N-4],acs[N-6],mks[N-6]; static beg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;beg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BEG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int beg_init(void){if(init)return -1;st.n_bp=0;st.n_be=0;st.n_bv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bps[i].active=0;for(int i=0;i<N-2;i++)bes[i].active=0;for(int i=0;i<N-4;i++)bvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BEG] Begonia initialized\n");return 0;}
int beg_planning(int t,int c,int a,int b,int d,int e,int y){return add(bps,&st.n_bp,&st.t_f1,N,t,c,a,b,d,e,y);}
int beg_execution(int t,int c,int a,int b,int d,int e,int y){return add(bes,&st.n_be,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int beg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bvs,&st.n_bv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int beg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int beg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void beg_report(void){ps("[BEG] Bp: ");pi(st.n_bp);ps(" PCS=");pi(st.t_f1);ps("\nBe: ");pi(st.n_be);ps(" PCS=");pi(st.t_f2);ps("\nBv: ");pi(st.n_bv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void beg_state(void){ps("[BEG] Bp=");pi(st.n_bp);ps(" Be=");pi(st.n_be);ps(" Bv=");pi(st.n_bv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Begonia Admin Demo ===\n\n");beg_init();
ps("Begonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;beg_planning(t,c,566+(i*17),555+(i*14),535+(i*10),517+(i*6),2020+(i%5));}
ps("\nBegonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;beg_execution(t,c,555+(i*15),544+(i*12),526+(i*8),513+(i*5),2021+(i%4));}
ps("\nBegonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;beg_evaluation(t,c,547+(i*13),536+(i*10),520+(i*7),509+(i*4),2022+(i%3));}
ps("\nBegonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beg_accessory(t,c,539+(i*11),530+(i*9),516+(i*6),506+(i*3),2023+(i%2));}
ps("\nBegonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beg_market(t,c,533+(i*9),524+(i*7),511+(i*5),503+(i*3),2024);}
ps("\n");beg_report();beg_state();ps("\n=== Demo Complete ===\n");return 0;}
