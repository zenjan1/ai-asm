/* nepeta_admin: Nepeta management technology administration (v1.0)
 * Nepeta planning, nepeta execution, nepeta evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} npt_t;
typedef struct{int n_nep,n_nee,n_nev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} npt_state_t;
static npt_t nptps[N],nptss[N-2],nptvss[N-4],nptas[N-6],nptmks[N-6]; static npt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(npt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;npt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int npt_init(void){if(init)return -1;st.n_nep=0;st.n_nee=0;st.n_nev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nptps[i].active=0;for(int i=0;i<N-2;i++)nptss[i].active=0;for(int i=0;i<N-4;i++)nptvss[i].active=0;for(int i=0;i<N-6;i++)nptas[i].active=0;for(int i=0;i<N-6;i++)nptmks[i].active=0;init=1;ps("[NPT] Nepeta initialized\n");return 0;}
int npt_planning(int t,int c,int a,int b,int d,int e,int y){return add(nptps,&st.n_nep,&st.t_f1,N,t,c,a,b,d,e,y);}
int npt_execution(int t,int c,int a,int b,int d,int e,int y){return add(nptss,&st.n_nee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int npt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nptvss,&st.n_nev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int npt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nptas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int npt_market(int t,int c,int a,int b,int d,int e,int y){return add(nptmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void npt_report(void){ps("[NPT] Nep: ");pi(st.n_nep);ps(" PCS=");pi(st.t_f1);ps("\nNee: ");pi(st.n_nee);ps(" PCS=");pi(st.t_f2);ps("\nNev: ");pi(st.n_nev);ps(" PCS=");pi(st.t_f3);ps("\nNec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void npt_state(void){ps("[NPT] Nep=");pi(st.n_nep);ps(" Nee=");pi(st.n_nee);ps(" Nev=");pi(st.n_nev);ps(" Nec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nepeta Admin Demo ===\n\n");npt_init();
ps("Nepeta planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;npt_planning(t,c,841+(i*17),830+(i*14),810+(i*10),792+(i*6),2020+(i%5));}
ps("\nNepeta execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;npt_execution(t,c,830+(i*15),819+(i*12),801+(i*8),788+(i*5),2021+(i%4));}
ps("\nNepeta evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;npt_evaluation(t,c,822+(i*13),811+(i*10),795+(i*7),784+(i*4),2022+(i%3));}
ps("\nNepeta accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;npt_accessory(t,c,814+(i*11),805+(i*9),791+(i*6),781+(i*3),2023+(i%2));}
ps("\nNepeta marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;npt_market(t,c,808+(i*9),799+(i*7),786+(i*5),778+(i*3),2024);}
ps("\n");npt_report();npt_state();ps("\n=== Demo Complete ===\n");return 0;}
