/* cassia_admin: Cassia management technology administration (v1.0)
 * Cassia planning, cassia execution, cassia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cas_t;
typedef struct{int n_casp,n_casse,n_cas2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cas_state_t;
static cas_t casps[N],casss[N-2],casvss[N-4],casas[N-6],casmks[N-6]; static cas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cas_init(void){if(init)return -1;st.n_casp=0;st.n_casse=0;st.n_cas2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)casps[i].active=0;for(int i=0;i<N-2;i++)casss[i].active=0;for(int i=0;i<N-4;i++)casvss[i].active=0;for(int i=0;i<N-6;i++)casas[i].active=0;for(int i=0;i<N-6;i++)casmks[i].active=0;init=1;ps("[CAS] Cassia initialized\n");return 0;}
int cas_planning(int t,int c,int a,int b,int d,int e,int y){return add(casps,&st.n_casp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cas_execution(int t,int c,int a,int b,int d,int e,int y){return add(casss,&st.n_casse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(casvss,&st.n_cas2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(casas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cas_market(int t,int c,int a,int b,int d,int e,int y){return add(casmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cas_report(void){ps("[CAS] Cassp: ");pi(st.n_casp);ps(" PCS=");pi(st.t_f1);ps("\nCasse: ");pi(st.n_casse);ps(" PCS=");pi(st.t_f2);ps("\nCasv: ");pi(st.n_cas2);ps(" PCS=");pi(st.t_f3);ps("\nCasc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cas_state(void){ps("[CAS] Cassp=");pi(st.n_casp);ps(" Casse=");pi(st.n_casse);ps(" Casv=");pi(st.n_cas2);ps(" Casc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cassia Admin Demo ===\n\n");cas_init();
ps("Cassia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cas_planning(t,c,982+(i*17),971+(i*14),951+(i*10),933+(i*6),2020+(i%5));}
ps("\nCassia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cas_execution(t,c,971+(i*15),960+(i*12),942+(i*8),929+(i*5),2021+(i%4));}
ps("\nCassia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cas_evaluation(t,c,963+(i*13),952+(i*10),936+(i*7),925+(i*4),2022+(i%3));}
ps("\nCassia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cas_accessory(t,c,955+(i*11),946+(i*9),932+(i*6),922+(i*3),2023+(i%2));}
ps("\nCassia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cas_market(t,c,949+(i*9),940+(i*7),927+(i*5),919+(i*3),2024);}
ps("\n");cas_report();cas_state();ps("\n=== Demo Complete ===\n");return 0;}
