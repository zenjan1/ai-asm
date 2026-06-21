/* impatiens_admin: Impatiens management technology administration (v1.0)
 * Impatiens planning, impatiens execution, impatiens evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} im_t;
typedef struct{int n_imp,n_ime,n_imv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} im_state_t;
static im_t imps[N],imss[N-2],imvss[N-4],imas[N-6],immks[N-6]; static im_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(im_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;im_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int imp_init(void){if(init)return -1;st.n_imp=0;st.n_ime=0;st.n_imv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)imps[i].active=0;for(int i=0;i<N-2;i++)imss[i].active=0;for(int i=0;i<N-4;i++)imvss[i].active=0;for(int i=0;i<N-6;i++)imas[i].active=0;for(int i=0;i<N-6;i++)immks[i].active=0;init=1;ps("[IMP] Impatiens initialized\n");return 0;}
int imp_planning(int t,int c,int a,int b,int d,int e,int y){return add(imps,&st.n_imp,&st.t_f1,N,t,c,a,b,d,e,y);}
int imp_execution(int t,int c,int a,int b,int d,int e,int y){return add(imss,&st.n_ime,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int imp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(imvss,&st.n_imv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int imp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(imas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int imp_market(int t,int c,int a,int b,int d,int e,int y){return add(immks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void imp_report(void){ps("[IMP] Imp: ");pi(st.n_imp);ps(" PCS=");pi(st.t_f1);ps("\nIme: ");pi(st.n_ime);ps(" PCS=");pi(st.t_f2);ps("\nImv: ");pi(st.n_imv);ps(" PCS=");pi(st.t_f3);ps("\nImc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void imp_state(void){ps("[IMP] Imp=");pi(st.n_imp);ps(" Ime=");pi(st.n_ime);ps(" Imv=");pi(st.n_imv);ps(" Imc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Impatiens Admin Demo ===\n\n");imp_init();
ps("Impatiens planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;imp_planning(t,c,789+(i*17),778+(i*14),758+(i*10),740+(i*6),2020+(i%5));}
ps("\nImpatiens execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;imp_execution(t,c,778+(i*15),767+(i*12),749+(i*8),736+(i*5),2021+(i%4));}
ps("\nImpatiens evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;imp_evaluation(t,c,770+(i*13),759+(i*10),745+(i*7),734+(i*4),2022+(i%3));}
ps("\nImpatiens accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;imp_accessory(t,c,762+(i*11),753+(i*9),739+(i*6),729+(i*3),2023+(i%2));}
ps("\nImpatiens marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;imp_market(t,c,756+(i*9),747+(i*7),734+(i*5),726+(i*3),2024);}
ps("\n");imp_report();imp_state();ps("\n=== Demo Complete ===\n");return 0;}
