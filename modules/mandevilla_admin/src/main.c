/* mandevilla_admin: Mandevilla management technology administration (v1.0)
 * Mandevilla planning, mandevilla execution, mandevilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mdv_t;
typedef struct{int n_mdp,n_mde,n_mdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mdv_state_t;
static mdv_t mdps[N],mdes[N-2],mdvs[N-4],mcs[N-6],mmks[N-6]; static mdv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mdv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mdv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mdv_init(void){if(init)return -1;st.n_mdp=0;st.n_mde=0;st.n_mdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mdps[i].active=0;for(int i=0;i<N-2;i++)mdes[i].active=0;for(int i=0;i<N-4;i++)mdvs[i].active=0;for(int i=0;i<N-6;i++)mcs[i].active=0;for(int i=0;i<N-6;i++)mmks[i].active=0;init=1;ps("[MDV] Mandevilla initialized\n");return 0;}
int mdv_planning(int t,int c,int a,int b,int d,int e,int y){return add(mdps,&st.n_mdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mdv_execution(int t,int c,int a,int b,int d,int e,int y){return add(mdes,&st.n_mde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mdv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mdvs,&st.n_mdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mdv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mdv_market(int t,int c,int a,int b,int d,int e,int y){return add(mmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mdv_report(void){ps("[MDV] Mdp: ");pi(st.n_mdp);ps(" PCS=");pi(st.t_f1);ps("\nMde: ");pi(st.n_mde);ps(" PCS=");pi(st.t_f2);ps("\nMdv: ");pi(st.n_mdv);ps(" PCS=");pi(st.t_f3);ps("\nMc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mdv_state(void){ps("[MDV] Mdp=");pi(st.n_mdp);ps(" Mde=");pi(st.n_mde);ps(" Mdv=");pi(st.n_mdv);ps(" Mc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mandevilla Admin Demo ===\n\n");mdv_init();
ps("Mandevilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mdv_planning(t,c,632+(i*17),621+(i*14),601+(i*10),583+(i*6),2020+(i%5));}
ps("\nMandevilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mdv_execution(t,c,621+(i*15),610+(i*12),592+(i*8),579+(i*5),2021+(i%4));}
ps("\nMandevilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mdv_evaluation(t,c,613+(i*13),602+(i*10),586+(i*7),575+(i*4),2022+(i%3));}
ps("\nMandevilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdv_accessory(t,c,605+(i*11),596+(i*9),582+(i*6),572+(i*3),2023+(i%2));}
ps("\nMandevilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdv_market(t,c,599+(i*9),590+(i*7),577+(i*5),569+(i*3),2024);}
ps("\n");mdv_report();mdv_state();ps("\n=== Demo Complete ===\n");return 0;}
