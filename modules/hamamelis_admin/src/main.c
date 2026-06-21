/* hamamelis_admin: Hamamelis management technology administration (v1.0)
 * Hamamelis planning, hamamelis execution, hamamelis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ham_t;
typedef struct{int n_hamp,n_hame,n_ham2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ham_state_t;
static ham_t hamps[N],hamss[N-2],hamvss[N-4],hamas[N-6],hammks[N-6]; static ham_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ham_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ham_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ham_init(void){if(init)return -1;st.n_hamp=0;st.n_hame=0;st.n_ham2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hamps[i].active=0;for(int i=0;i<N-2;i++)hamss[i].active=0;for(int i=0;i<N-4;i++)hamvss[i].active=0;for(int i=0;i<N-6;i++)hamas[i].active=0;for(int i=0;i<N-6;i++)hammks[i].active=0;init=1;ps("[HAM] Hamamelis initialized\n");return 0;}
int ham_planning(int t,int c,int a,int b,int d,int e,int y){return add(hamps,&st.n_hamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ham_execution(int t,int c,int a,int b,int d,int e,int y){return add(hamss,&st.n_hame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ham_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hamvss,&st.n_ham2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ham_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hamas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ham_market(int t,int c,int a,int b,int d,int e,int y){return add(hammks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ham_report(void){ps("[HAM] Hamp: ");pi(st.n_hamp);ps(" PCS=");pi(st.t_f1);ps("\nHame: ");pi(st.n_hame);ps(" PCS=");pi(st.t_f2);ps("\nHamv: ");pi(st.n_ham2);ps(" PCS=");pi(st.t_f3);ps("\nHamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ham_state(void){ps("[HAM] Hamp=");pi(st.n_hamp);ps(" Hame=");pi(st.n_hame);ps(" Hamv=");pi(st.n_ham2);ps(" Hamc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hamamelis Admin Demo ===\n\n");ham_init();
ps("Hamamelis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ham_planning(t,c,926+(i*17),915+(i*14),895+(i*10),877+(i*6),2020+(i%5));}
ps("\nHamamelis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ham_execution(t,c,915+(i*15),904+(i*12),886+(i*8),873+(i*5),2021+(i%4));}
ps("\nHamamelis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ham_evaluation(t,c,907+(i*13),896+(i*10),880+(i*7),869+(i*4),2022+(i%3));}
ps("\nHamamelis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ham_accessory(t,c,899+(i*11),890+(i*9),876+(i*6),866+(i*3),2023+(i%2));}
ps("\nHamamelis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ham_market(t,c,893+(i*9),884+(i*7),871+(i*5),863+(i*3),2024);}
ps("\n");ham_report();ham_state();ps("\n=== Demo Complete ===\n");return 0;}
