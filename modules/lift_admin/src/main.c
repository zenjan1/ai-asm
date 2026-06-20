/* lift_admin: Lifting technology administration (v1.0)
 * Elevators, hoists, aerial work platforms, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lfx_t;
typedef struct{int n_el,n_hs,n_aw,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lfx_state_t;
static lfx_t els[N],hss[N-2],aws[N-4],acs[N-6],mks[N-6]; static lfx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lfx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lfx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LFX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lfx_init(void){if(init)return -1;st.n_el=0;st.n_hs=0;st.n_aw=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)els[i].active=0;for(int i=0;i<N-2;i++)hss[i].active=0;for(int i=0;i<N-4;i++)aws[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LFX] Lift initialized\n");return 0;}
int lfx_elevator(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f1,N,t,c,a,b,d,e,y);}
int lfx_hoist(int t,int c,int a,int b,int d,int e,int y){return add(hss,&st.n_hs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lfx_aerial(int t,int c,int a,int b,int d,int e,int y){return add(aws,&st.n_aw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lfx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lfx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lfx_report(void){ps("[LFX] El: ");pi(st.n_el);ps(" PCS=");pi(st.t_f1);ps("\nHs: ");pi(st.n_hs);ps(" PCS=");pi(st.t_f2);ps("\nAw: ");pi(st.n_aw);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lfx_state(void){ps("[LFX] El=");pi(st.n_el);ps(" Hs=");pi(st.n_hs);ps(" Aw=");pi(st.n_aw);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lift Admin Demo ===\n\n");lfx_init();
ps("Elevators...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lfx_elevator(t,c,435+(i*17),424+(i*14),404+(i*10),386+(i*6),2020+(i%5));}
ps("\nHoists...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lfx_hoist(t,c,424+(i*15),413+(i*12),395+(i*8),382+(i*5),2021+(i%4));}
ps("\nAerial work platforms...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lfx_aerial(t,c,416+(i*13),405+(i*10),389+(i*7),378+(i*4),2022+(i%3));}
ps("\nLifting accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lfx_accessory(t,c,408+(i*11),399+(i*9),385+(i*6),375+(i*3),2023+(i%2));}
ps("\nLifting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lfx_market(t,c,402+(i*9),393+(i*7),380+(i*5),372+(i*3),2024);}
ps("\n");lfx_report();lfx_state();ps("\n=== Demo Complete ===\n");return 0;}
