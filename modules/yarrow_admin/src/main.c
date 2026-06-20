/* yarrow_admin: Yarrow management technology administration (v1.0)
 * Yarrow planning, yarrow execution, yarrow evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} yar_t;
typedef struct{int n_yrp,n_yre,n_yrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} yar_state_t;
static yar_t yrps[N],yres[N-2],yrvs[N-4],acs[N-6],mks[N-6]; static yar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;yar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[YAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int yar_init(void){if(init)return -1;st.n_yrp=0;st.n_yre=0;st.n_yrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)yrps[i].active=0;for(int i=0;i<N-2;i++)yres[i].active=0;for(int i=0;i<N-4;i++)yrvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[YAR] Yarrow initialized\n");return 0;}
int yar_planning(int t,int c,int a,int b,int d,int e,int y){return add(yrps,&st.n_yrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int yar_execution(int t,int c,int a,int b,int d,int e,int y){return add(yres,&st.n_yre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int yar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(yrvs,&st.n_yrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int yar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int yar_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void yar_report(void){ps("[YAR] Yrp: ");pi(st.n_yrp);ps(" PCS=");pi(st.t_f1);ps("\nYre: ");pi(st.n_yre);ps(" PCS=");pi(st.t_f2);ps("\nYrv: ");pi(st.n_yrv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void yar_state(void){ps("[YAR] Yrp=");pi(st.n_yrp);ps(" Yre=");pi(st.n_yre);ps(" Yrv=");pi(st.n_yrv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Yarrow Admin Demo ===\n\n");yar_init();
ps("Yarrow planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;yar_planning(t,c,580+(i*17),569+(i*14),549+(i*10),531+(i*6),2020+(i%5));}
ps("\nYarrow execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;yar_execution(t,c,569+(i*15),558+(i*12),540+(i*8),527+(i*5),2021+(i%4));}
ps("\nYarrow evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;yar_evaluation(t,c,561+(i*13),550+(i*10),534+(i*7),523+(i*4),2022+(i%3));}
ps("\nYarrow accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yar_accessory(t,c,553+(i*11),544+(i*9),530+(i*6),520+(i*3),2023+(i%2));}
ps("\nYarrow marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yar_market(t,c,547+(i*9),538+(i*7),525+(i*5),517+(i*3),2024);}
ps("\n");yar_report();yar_state();ps("\n=== Demo Complete ===\n");return 0;}
