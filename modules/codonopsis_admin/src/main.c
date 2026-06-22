/* codonopsis_admin: Codonopsis management technology administration (v1.0)
 * Codonopsis planning, codonopsis execution, codonopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cod_t;
typedef struct{int n_codp,n_code,n_cod2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cod_state_t;
static cod_t codps[N],codss[N-2],codvss[N-4],codas[N-6],codmks[N-6]; static cod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cod_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cod_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cod_init(void){if(init)return -1;st.n_codp=0;st.n_code=0;st.n_cod2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)codps[i].active=0;for(int i=0;i<N-2;i++)codss[i].active=0;for(int i=0;i<N-4;i++)codvss[i].active=0;for(int i=0;i<N-6;i++)codas[i].active=0;for(int i=0;i<N-6;i++)codmks[i].active=0;init=1;ps("[COD] Codonopsis initialized\n");return 0;}
int cod_planning(int t,int c,int a,int b,int d,int e,int y){return add(codps,&st.n_codp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cod_execution(int t,int c,int a,int b,int d,int e,int y){return add(codss,&st.n_code,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cod_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(codvss,&st.n_cod2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cod_accessory(int t,int c,int a,int b,int d,int e,int y){return add(codas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cod_market(int t,int c,int a,int b,int d,int e,int y){return add(codmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cod_report(void){ps("[COD] Codpp: ");pi(st.n_codp);ps(" PCS=");pi(st.t_f1);ps("\nCode: ");pi(st.n_code);ps(" PCS=");pi(st.t_f2);ps("\nCodv: ");pi(st.n_cod2);ps(" PCS=");pi(st.t_f3);ps("\nCodc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cod_state(void){ps("[COD] Codpp=");pi(st.n_codp);ps(" Code=");pi(st.n_code);ps(" Codv=");pi(st.n_cod2);ps(" Codc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Codonopsis Admin Demo ===\n\n");cod_init();
ps("Codonopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cod_planning(t,c,1131+(i*17),1120+(i*14),1100+(i*10),1082+(i*6),2020+(i%5));}
ps("\nCodonopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cod_execution(t,c,1120+(i*15),1109+(i*12),1091+(i*8),1078+(i*5),2021+(i%4));}
ps("\nCodonopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cod_evaluation(t,c,1112+(i*13),1101+(i*10),1085+(i*7),1074+(i*4),2022+(i%3));}
ps("\nCodonopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cod_accessory(t,c,1104+(i*11),1095+(i*9),1081+(i*6),1071+(i*3),2023+(i%2));}
ps("\nCodonopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cod_market(t,c,1098+(i*9),1089+(i*7),1076+(i*5),1068+(i*3),2024);}
ps("\n");cod_report();cod_state();ps("\n=== Demo Complete ===\n");return 0;}
