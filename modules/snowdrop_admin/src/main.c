/* snowdrop_admin: Snowdrop management technology administration (v1.0)
 * Snowdrop planning, snowdrop execution, snowdrop evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sno_t;
typedef struct{int n_snp,n_sne,n_snv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sno_state_t;
static sno_t snps[N],snes[N-2],snvs[N-4],acs[N-6],mks[N-6]; static sno_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sno_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sno_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SNO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sno_init(void){if(init)return -1;st.n_snp=0;st.n_sne=0;st.n_snv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)snps[i].active=0;for(int i=0;i<N-2;i++)snes[i].active=0;for(int i=0;i<N-4;i++)snvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SNO] Snowdrop initialized\n");return 0;}
int sno_planning(int t,int c,int a,int b,int d,int e,int y){return add(snps,&st.n_snp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sno_execution(int t,int c,int a,int b,int d,int e,int y){return add(snes,&st.n_sne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sno_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(snvs,&st.n_snv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sno_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sno_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sno_report(void){ps("[SNO] Snp: ");pi(st.n_snp);ps(" PCS=");pi(st.t_f1);ps("\nSne: ");pi(st.n_sne);ps(" PCS=");pi(st.t_f2);ps("\nSnv: ");pi(st.n_snv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sno_state(void){ps("[SNO] Snp=");pi(st.n_snp);ps(" Sne=");pi(st.n_sne);ps(" Snv=");pi(st.n_snv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Snowdrop Admin Demo ===\n\n");sno_init();
ps("Snowdrop planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sno_planning(t,c,589+(i*17),578+(i*14),558+(i*10),540+(i*6),2020+(i%5));}
ps("\nSnowdrop execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sno_execution(t,c,578+(i*15),567+(i*12),549+(i*8),536+(i*5),2021+(i%4));}
ps("\nSnowdrop evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sno_evaluation(t,c,570+(i*13),559+(i*10),543+(i*7),532+(i*4),2022+(i%3));}
ps("\nSnowdrop accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sno_accessory(t,c,562+(i*11),553+(i*9),539+(i*6),529+(i*3),2023+(i%2));}
ps("\nSnowdrop marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sno_market(t,c,556+(i*9),547+(i*7),534+(i*5),526+(i*3),2024);}
ps("\n");sno_report();sno_state();ps("\n=== Demo Complete ===\n");return 0;}
