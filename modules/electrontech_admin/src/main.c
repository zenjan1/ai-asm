/* electrontech_admin: Electronic technology administration (v1.0)
 * Electronic components, IC, electronic equipment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} elc_t;
typedef struct{int n_cp,n_ic,n_eq,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} elc_state_t;
static elc_t cps[N],ics[N-2],eqs[N-4],acs[N-6],mks[N-6]; static elc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;elc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int elc_init(void){if(init)return -1;st.n_cp=0;st.n_ic=0;st.n_eq=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cps[i].active=0;for(int i=0;i<N-2;i++)ics[i].active=0;for(int i=0;i<N-4;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ELC] Electrontech initialized\n");return 0;}
int elc_component(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f1,N,t,c,a,b,d,e,y);}
int elc_ic(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int elc_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int elc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int elc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void elc_report(void){ps("[ELC] Cp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f1);ps("\nIc: ");pi(st.n_ic);ps(" PCS=");pi(st.t_f2);ps("\nEq: ");pi(st.n_eq);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void elc_state(void){ps("[ELC] Cp=");pi(st.n_cp);ps(" Ic=");pi(st.n_ic);ps(" Eq=");pi(st.n_eq);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Electronic Tech Admin Demo ===\n\n");elc_init();
ps("Electronic components...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;elc_component(t,c,313+(i*17),298+(i*14),278+(i*10),260+(i*6),2020+(i%5));}
ps("\nIntegrated circuits...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;elc_ic(t,c,302+(i*15),288+(i*12),270+(i*8),257+(i*5),2021+(i%4));}
ps("\nElectronic equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;elc_equipment(t,c,294+(i*13),280+(i*10),264+(i*7),253+(i*4),2022+(i%3));}
ps("\nElectronic accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elc_accessory(t,c,286+(i*11),274+(i*9),260+(i*6),250+(i*3),2023+(i%2));}
ps("\nElectronic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elc_market(t,c,280+(i*9),269+(i*7),256+(i*5),248+(i*3),2024);}
ps("\n");elc_report();elc_state();ps("\n=== Demo Complete ===\n");return 0;}
