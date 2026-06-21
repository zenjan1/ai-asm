/* pistacia_admin: Pistacia management technology administration (v1.0)
 * Pistacia planning, pistacia execution, pistacia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pis_t;
typedef struct{int n_pisp,n_pise,n_pis2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pis_state_t;
static pis_t pisps[N],pisss[N-2],pisvss[N-4],pisas[N-6],pismks[N-6]; static pis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pis_init(void){if(init)return -1;st.n_pisp=0;st.n_pise=0;st.n_pis2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pisps[i].active=0;for(int i=0;i<N-2;i++)pisss[i].active=0;for(int i=0;i<N-4;i++)pisvss[i].active=0;for(int i=0;i<N-6;i++)pisas[i].active=0;for(int i=0;i<N-6;i++)pismks[i].active=0;init=1;ps("[PIS] Pistacia initialized\n");return 0;}
int pis_planning(int t,int c,int a,int b,int d,int e,int y){return add(pisps,&st.n_pisp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pis_execution(int t,int c,int a,int b,int d,int e,int y){return add(pisss,&st.n_pise,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pisvss,&st.n_pis2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pisas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pis_market(int t,int c,int a,int b,int d,int e,int y){return add(pismks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pis_report(void){ps("[PIS] pisp: ");pi(st.n_pisp);ps(" PCS=");pi(st.t_f1);ps("\npise: ");pi(st.n_pise);ps(" PCS=");pi(st.t_f2);ps("\npisv: ");pi(st.n_pis2);ps(" PCS=");pi(st.t_f3);ps("\npisc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pis_state(void){ps("[PIS] pisp=");pi(st.n_pisp);ps(" pise=");pi(st.n_pise);ps(" pisv=");pi(st.n_pis2);ps(" pisc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pistacia Admin Demo ===\n\n");pis_init();
ps("Pistacia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pis_planning(t,c,1085+(i*17),1074+(i*14),1054+(i*10),1036+(i*6),2020+(i%5));}
ps("\nPistacia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pis_execution(t,c,1074+(i*15),1063+(i*12),1045+(i*8),1032+(i*5),2021+(i%4));}
ps("\nPistacia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pis_evaluation(t,c,1066+(i*13),1055+(i*10),1039+(i*7),1028+(i*4),2022+(i%3));}
ps("\nPistacia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pis_accessory(t,c,1058+(i*11),1049+(i*9),1035+(i*6),1025+(i*3),2023+(i%2));}
ps("\nPistacia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pis_market(t,c,1052+(i*9),1043+(i*7),1030+(i*5),1022+(i*3),2024);}
ps("\n");pis_report();pis_state();ps("\n=== Demo Complete ===\n");return 0;}
