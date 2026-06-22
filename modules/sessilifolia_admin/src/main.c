/* sessilifolia_admin: Sessilifolia management technology administration (v1.0)
 * Sessilifolia planning, sessilifolia execution, sessilifolia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ses_t;
typedef struct{int n_sesp,n_sese,n_ses2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ses_state_t;
static ses_t sesps[N],sesss[N-2],sesvss[N-4],sesas[N-6],sesmks[N-6]; static ses_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ses_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ses_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ses_init(void){if(init)return -1;st.n_sesp=0;st.n_sese=0;st.n_ses2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sesps[i].active=0;for(int i=0;i<N-2;i++)sesss[i].active=0;for(int i=0;i<N-4;i++)sesvss[i].active=0;for(int i=0;i<N-6;i++)sesas[i].active=0;for(int i=0;i<N-6;i++)sesmks[i].active=0;init=1;ps("[SES] Sessilifolia initialized\n");return 0;}
int ses_planning(int t,int c,int a,int b,int d,int e,int y){return add(sesps,&st.n_sesp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ses_execution(int t,int c,int a,int b,int d,int e,int y){return add(sesss,&st.n_sese,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ses_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sesvss,&st.n_ses2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ses_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sesas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ses_market(int t,int c,int a,int b,int d,int e,int y){return add(sesmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ses_report(void){ps("[SES] Sespp: ");pi(st.n_sesp);ps(" PCS=");pi(st.t_f1);ps("\nSese: ");pi(st.n_sese);ps(" PCS=");pi(st.t_f2);ps("\nSesv: ");pi(st.n_ses2);ps(" PCS=");pi(st.t_f3);ps("\nSesc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ses_state(void){ps("[SES] Sespp=");pi(st.n_sesp);ps(" Sese=");pi(st.n_sese);ps(" Sesv=");pi(st.n_ses2);ps(" Sesc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sessilifolia Admin Demo ===\n\n");ses_init();
ps("Sessilifolia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ses_planning(t,c,1190+(i*17),1179+(i*14),1159+(i*10),1141+(i*6),2020+(i%5));}
ps("\nSessilifolia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ses_execution(t,c,1179+(i*15),1168+(i*12),1150+(i*8),1137+(i*5),2021+(i%4));}
ps("\nSessilifolia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ses_evaluation(t,c,1171+(i*13),1160+(i*10),1144+(i*7),1133+(i*4),2022+(i%3));}
ps("\nSessilifolia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ses_accessory(t,c,1163+(i*11),1154+(i*9),1140+(i*6),1130+(i*3),2023+(i%2));}
ps("\nSessilifolia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ses_market(t,c,1157+(i*9),1148+(i*7),1135+(i*5),1127+(i*3),2024);}
ps("\n");ses_report();ses_state();ps("\n=== Demo Complete ===\n");return 0;}
