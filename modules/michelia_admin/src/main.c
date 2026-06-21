/* michelia_admin: Michelia management technology administration (v1.0)
 * Michelia planning, michelia execution, michelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mci_t;
typedef struct{int n_mip,n_mie,n_miv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mci_state_t;
static mci_t mcips[N],mciss[N-2],mcivss[N-4],mcias[N-6],mcimks[N-6]; static mci_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mci_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mci_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mci_init(void){if(init)return -1;st.n_mip=0;st.n_mie=0;st.n_miv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcips[i].active=0;for(int i=0;i<N-2;i++)mciss[i].active=0;for(int i=0;i<N-4;i++)mcivss[i].active=0;for(int i=0;i<N-6;i++)mcias[i].active=0;for(int i=0;i<N-6;i++)mcimks[i].active=0;init=1;ps("[MCI] Michelia initialized\n");return 0;}
int mci_planning(int t,int c,int a,int b,int d,int e,int y){return add(mcips,&st.n_mip,&st.t_f1,N,t,c,a,b,d,e,y);}
int mci_execution(int t,int c,int a,int b,int d,int e,int y){return add(mciss,&st.n_mie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mci_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mcivss,&st.n_miv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mci_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mcias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mci_market(int t,int c,int a,int b,int d,int e,int y){return add(mcimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mci_report(void){ps("[MCI] Mip: ");pi(st.n_mip);ps(" PCS=");pi(st.t_f1);ps("\nMie: ");pi(st.n_mie);ps(" PCS=");pi(st.t_f2);ps("\nMiv: ");pi(st.n_miv);ps(" PCS=");pi(st.t_f3);ps("\nMic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mci_state(void){ps("[MCI] Mip=");pi(st.n_mip);ps(" Mie=");pi(st.n_mie);ps(" Miv=");pi(st.n_miv);ps(" Mic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Michelia Admin Demo ===\n\n");mci_init();
ps("Michelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mci_planning(t,c,839+(i*17),828+(i*14),808+(i*10),790+(i*6),2020+(i%5));}
ps("\nMichelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mci_execution(t,c,828+(i*15),817+(i*12),799+(i*8),786+(i*5),2021+(i%4));}
ps("\nMichelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mci_evaluation(t,c,820+(i*13),809+(i*10),793+(i*7),782+(i*4),2022+(i%3));}
ps("\nMichelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mci_accessory(t,c,812+(i*11),803+(i*9),789+(i*6),779+(i*3),2023+(i%2));}
ps("\nMichelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mci_market(t,c,806+(i*9),797+(i*7),784+(i*5),776+(i*3),2024);}
ps("\n");mci_report();mci_state();ps("\n=== Demo Complete ===\n");return 0;}
