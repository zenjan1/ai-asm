/* cardamom_admin: Cardamom management technology administration (v1.0)
 * Cardamom planning, cardamom execution, cardamom evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cd_t;
typedef struct{int n_cdp,n_cde,n_cdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cd_state_t;
static cd_t cdps[N],cdss[N-2],cdvss[N-4],cdas[N-6],cdmks[N-6]; static cd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CDM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cd_init(void){if(init)return -1;st.n_cdp=0;st.n_cde=0;st.n_cdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cdps[i].active=0;for(int i=0;i<N-2;i++)cdss[i].active=0;for(int i=0;i<N-4;i++)cdvss[i].active=0;for(int i=0;i<N-6;i++)cdas[i].active=0;for(int i=0;i<N-6;i++)cdmks[i].active=0;init=1;ps("[CDM] Cardamom initialized\n");return 0;}
int cd_planning(int t,int c,int a,int b,int d,int e,int y){return add(cdps,&st.n_cdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cd_execution(int t,int c,int a,int b,int d,int e,int y){return add(cdss,&st.n_cde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cdvss,&st.n_cdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cd_market(int t,int c,int a,int b,int d,int e,int y){return add(cdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cd_report(void){ps("[CDM] Cdp: ");pi(st.n_cdp);ps(" PCS=");pi(st.t_f1);ps("\nCde: ");pi(st.n_cde);ps(" PCS=");pi(st.t_f2);ps("\nCdv: ");pi(st.n_cdv);ps(" PCS=");pi(st.t_f3);ps("\nCdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cd_state(void){ps("[CDM] Cdp=");pi(st.n_cdp);ps(" Cde=");pi(st.n_cde);ps(" Cdv=");pi(st.n_cdv);ps(" Cdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cardamom Admin Demo ===\n\n");cd_init();
ps("Cardamom planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cd_planning(t,c,674+(i*17),663+(i*14),643+(i*10),625+(i*6),2020+(i%5));}
ps("\nCardamom execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cd_execution(t,c,663+(i*15),652+(i*12),634+(i*8),621+(i*5),2021+(i%4));}
ps("\nCardamom evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cd_evaluation(t,c,655+(i*13),644+(i*10),628+(i*7),617+(i*4),2022+(i%3));}
ps("\nCardamom accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cd_accessory(t,c,647+(i*11),638+(i*9),624+(i*6),614+(i*3),2023+(i%2));}
ps("\nCardamom marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cd_market(t,c,641+(i*9),632+(i*7),619+(i*5),611+(i*3),2024);}
ps("\n");cd_report();cd_state();ps("\n=== Demo Complete ===\n");return 0;}
