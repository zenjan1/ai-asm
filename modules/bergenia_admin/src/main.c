/* bergenia_admin: Bergenia management technology administration (v1.0)
 * Bergenia planning, bergenia execution, bergenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ber_t;
typedef struct{int n_berp,n_bere,n_ber2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ber_state_t;
static ber_t berps[N],berss[N-2],bervss[N-4],beras[N-6],bermks[N-6]; static ber_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ber_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ber_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ber_init(void){if(init)return -1;st.n_berp=0;st.n_bere=0;st.n_ber2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)berps[i].active=0;for(int i=0;i<N-2;i++)berss[i].active=0;for(int i=0;i<N-4;i++)bervss[i].active=0;for(int i=0;i<N-6;i++)beras[i].active=0;for(int i=0;i<N-6;i++)bermks[i].active=0;init=1;ps("[BER] Bergenia initialized\n");return 0;}
int ber_planning(int t,int c,int a,int b,int d,int e,int y){return add(berps,&st.n_berp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ber_execution(int t,int c,int a,int b,int d,int e,int y){return add(berss,&st.n_bere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ber_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bervss,&st.n_ber2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ber_accessory(int t,int c,int a,int b,int d,int e,int y){return add(beras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ber_market(int t,int c,int a,int b,int d,int e,int y){return add(bermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ber_report(void){ps("[BER] Berp: ");pi(st.n_berp);ps(" PCS=");pi(st.t_f1);ps("\nBere: ");pi(st.n_bere);ps(" PCS=");pi(st.t_f2);ps("\nBerv: ");pi(st.n_ber2);ps(" PCS=");pi(st.t_f3);ps("\nBerc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ber_state(void){ps("[BER] Berp=");pi(st.n_berp);ps(" Bere=");pi(st.n_bere);ps(" Berv=");pi(st.n_ber2);ps(" Berc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bergenia Admin Demo ===\n\n");ber_init();
ps("Bergenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ber_planning(t,c,901+(i*17),890+(i*14),870+(i*10),852+(i*6),2020+(i%5));}
ps("\nBergenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ber_execution(t,c,890+(i*15),879+(i*12),861+(i*8),848+(i*5),2021+(i%4));}
ps("\nBergenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ber_evaluation(t,c,882+(i*13),871+(i*10),855+(i*7),844+(i*4),2022+(i%3));}
ps("\nBergenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ber_accessory(t,c,874+(i*11),865+(i*9),851+(i*6),841+(i*3),2023+(i%2));}
ps("\nBergenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ber_market(t,c,868+(i*9),859+(i*7),846+(i*5),838+(i*3),2024);}
ps("\n");ber_report();ber_state();ps("\n=== Demo Complete ===\n");return 0;}
