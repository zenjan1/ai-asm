/* nigella_admin: Nigella management technology administration (v1.0)
 * Nigella planning, nigella execution, nigella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nig_t;
typedef struct{int n_nip,n_nie,n_niv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nig_state_t;
static nig_t nips[N],nies[N-2],nivs[N-4],acs[N-6],mks[N-6]; static nig_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nig_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nig_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NIG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nig_init(void){if(init)return -1;st.n_nip=0;st.n_nie=0;st.n_niv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nips[i].active=0;for(int i=0;i<N-2;i++)nies[i].active=0;for(int i=0;i<N-4;i++)nivs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NIG] Nigella initialized\n");return 0;}
int nig_planning(int t,int c,int a,int b,int d,int e,int y){return add(nips,&st.n_nip,&st.t_f1,N,t,c,a,b,d,e,y);}
int nig_execution(int t,int c,int a,int b,int d,int e,int y){return add(nies,&st.n_nie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nig_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nivs,&st.n_niv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nig_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nig_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nig_report(void){ps("[NIG] Nip: ");pi(st.n_nip);ps(" PCS=");pi(st.t_f1);ps("\nNie: ");pi(st.n_nie);ps(" PCS=");pi(st.t_f2);ps("\nNiv: ");pi(st.n_niv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nig_state(void){ps("[NIG] Nip=");pi(st.n_nip);ps(" Nie=");pi(st.n_nie);ps(" Niv=");pi(st.n_niv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nigella Admin Demo ===\n\n");nig_init();
ps("Nigella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nig_planning(t,c,597+(i*17),586+(i*14),566+(i*10),548+(i*6),2020+(i%5));}
ps("\nNigella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nig_execution(t,c,586+(i*15),575+(i*12),557+(i*8),544+(i*5),2021+(i%4));}
ps("\nNigella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nig_evaluation(t,c,578+(i*13),567+(i*10),551+(i*7),540+(i*4),2022+(i%3));}
ps("\nNigella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nig_accessory(t,c,570+(i*11),561+(i*9),547+(i*6),537+(i*3),2023+(i%2));}
ps("\nNigella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nig_market(t,c,564+(i*9),555+(i*7),542+(i*5),534+(i*3),2024);}
ps("\n");nig_report();nig_state();ps("\n=== Demo Complete ===\n");return 0;}
