/* lychnis_admin: Lychnis management technology administration (v1.0)
 * Lychnis planning, lychnis execution, lychnis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lyc_t;
typedef struct{int n_lycp,n_lyce,n_lyc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lyc_state_t;
static lyc_t lycps[N],lycss[N-2],lycvss[N-4],lycas[N-6],lycmks[N-6]; static lyc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lyc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lyc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lyc_init(void){if(init)return -1;st.n_lycp=0;st.n_lyce=0;st.n_lyc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lycps[i].active=0;for(int i=0;i<N-2;i++)lycss[i].active=0;for(int i=0;i<N-4;i++)lycvss[i].active=0;for(int i=0;i<N-6;i++)lycas[i].active=0;for(int i=0;i<N-6;i++)lycmks[i].active=0;init=1;ps("[LYC] Lychnis initialized\n");return 0;}
int lyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(lycps,&st.n_lycp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(lycss,&st.n_lyce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lycvss,&st.n_lyc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lycas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lyc_market(int t,int c,int a,int b,int d,int e,int y){return add(lycmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lyc_report(void){ps("[LYC] Lycop: ");pi(st.n_lycp);ps(" PCS=");pi(st.t_f1);ps("\nLyce: ");pi(st.n_lyce);ps(" PCS=");pi(st.t_f2);ps("\nLycv: ");pi(st.n_lyc2);ps(" PCS=");pi(st.t_f3);ps("\nLycc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lyc_state(void){ps("[LYC] Lycop=");pi(st.n_lycp);ps(" Lyce=");pi(st.n_lyce);ps(" Lycv=");pi(st.n_lyc2);ps(" Lycc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lychnis Admin Demo ===\n\n");lyc_init();
ps("Lychnis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lyc_planning(t,c,938+(i*17),927+(i*14),907+(i*10),889+(i*6),2020+(i%5));}
ps("\nLychnis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lyc_execution(t,c,927+(i*15),916+(i*12),898+(i*8),885+(i*5),2021+(i%4));}
ps("\nLychnis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lyc_evaluation(t,c,919+(i*13),908+(i*10),892+(i*7),881+(i*4),2022+(i%3));}
ps("\nLychnis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyc_accessory(t,c,911+(i*11),902+(i*9),888+(i*6),878+(i*3),2023+(i%2));}
ps("\nLychnis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyc_market(t,c,905+(i*9),896+(i*7),883+(i*5),875+(i*3),2024);}
ps("\n");lyc_report();lyc_state();ps("\n=== Demo Complete ===\n");return 0;}
