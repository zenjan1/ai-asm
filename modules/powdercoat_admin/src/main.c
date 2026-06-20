/* powdercoat_admin: Powder coating administration (v1.0)
 * Epoxy powder, polyester powder, acrylic powder, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pwc_t;
typedef struct{int n_ep,n_pe,n_ac,n_ax,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pwc_state_t;
static pwc_t eps[N],pes[N-2],acs[N-4],axs[N-6],mks[N-6]; static pwc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pwc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pwc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PWC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pwc_init(void){if(init)return -1;st.n_ep=0;st.n_pe=0;st.n_ac=0;st.n_ax=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eps[i].active=0;for(int i=0;i<N-2;i++)pes[i].active=0;for(int i=0;i<N-4;i++)acs[i].active=0;for(int i=0;i<N-6;i++)axs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PWC] Powdercoat initialized\n");return 0;}
int pwc_epoxy(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f1,N,t,c,a,b,d,e,y);}
int pwc_polyester(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pwc_acrylic(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pwc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(axs,&st.n_ax,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pwc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pwc_report(void){ps("[PWC] Ep: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f1);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f2);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f3);ps("\nAx: ");pi(st.n_ax);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pwc_state(void){ps("[PWC] Ep=");pi(st.n_ep);ps(" Pe=");pi(st.n_pe);ps(" Ac=");pi(st.n_ac);ps(" Ax=");pi(st.n_ax);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Powder Coat Admin Demo ===\n\n");pwc_init();
ps("Epoxy powder...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pwc_epoxy(t,c,338+(i*17),323+(i*14),303+(i*10),285+(i*6),2020+(i%5));}
ps("\nPolyester powder...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pwc_polyester(t,c,327+(i*15),313+(i*12),295+(i*8),282+(i*5),2021+(i%4));}
ps("\nAcrylic powder...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pwc_acrylic(t,c,319+(i*13),305+(i*10),289+(i*7),278+(i*4),2022+(i%3));}
ps("\nPowder coating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pwc_accessory(t,c,311+(i*11),299+(i*9),285+(i*6),275+(i*3),2023+(i%2));}
ps("\nPowder coating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pwc_market(t,c,305+(i*9),294+(i*7),281+(i*5),273+(i*3),2024);}
ps("\n");pwc_report();pwc_state();ps("\n=== Demo Complete ===\n");return 0;}
