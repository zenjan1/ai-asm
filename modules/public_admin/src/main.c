/* public_admin: Public management technology administration (v1.0)
 * Public relations, public affairs, public communication, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pbl_t;
typedef struct{int n_pr,n_pa,n_pcc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pbl_state_t;
static pbl_t prs[N],pas[N-2],pccs[N-4],acs[N-6],mks[N-6]; static pbl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pbl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pbl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PBL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pbl_init(void){if(init)return -1;st.n_pr=0;st.n_pa=0;st.n_pcc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prs[i].active=0;for(int i=0;i<N-2;i++)pas[i].active=0;for(int i=0;i<N-4;i++)pccs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PBL] Public initialized\n");return 0;}
int pbl_relations(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f1,N,t,c,a,b,d,e,y);}
int pbl_affairs(int t,int c,int a,int b,int d,int e,int y){return add(pas,&st.n_pa,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pbl_communication(int t,int c,int a,int b,int d,int e,int y){return add(pccs,&st.n_pcc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pbl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pbl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pbl_report(void){ps("[PBL] Pr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f1);ps("\nPa: ");pi(st.n_pa);ps(" PCS=");pi(st.t_f2);ps("\nPcc: ");pi(st.n_pcc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pbl_state(void){ps("[PBL] Pr=");pi(st.n_pr);ps(" Pa=");pi(st.n_pa);ps(" Pcc=");pi(st.n_pcc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Public Admin Demo ===\n\n");pbl_init();
ps("Public relations...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pbl_relations(t,c,464+(i*17),453+(i*14),433+(i*10),415+(i*6),2020+(i%5));}
ps("\nPublic affairs...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pbl_affairs(t,c,453+(i*15),442+(i*12),424+(i*8),411+(i*5),2021+(i%4));}
ps("\nPublic communication...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pbl_communication(t,c,445+(i*13),434+(i*10),418+(i*7),407+(i*4),2022+(i%3));}
ps("\nPublic accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pbl_accessory(t,c,437+(i*11),428+(i*9),414+(i*6),404+(i*3),2023+(i%2));}
ps("\nPublic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pbl_market(t,c,431+(i*9),422+(i*7),409+(i*5),401+(i*3),2024);}
ps("\n");pbl_report();pbl_state();ps("\n=== Demo Complete ===\n");return 0;}
