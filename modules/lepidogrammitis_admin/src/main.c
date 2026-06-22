/* lepidogrammitis_admin: Lepidogrammitis management technology administration (v1.0)
 * Lepidogrammitis planning, lepidogrammitis execution, lepidogrammitis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lepd_t;
typedef struct{int n_lepdop,n_lepd,n_lepd2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lepd_state_t;
static lepd_t lepdos[N],lepde[N-2],lepd2[N-4],lepdac[N-6],lepdam[N-6]; static lepd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lepd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lepd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEPD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lepd_init(void){if(init)return -1;st.n_lepdop=0;st.n_lepd=0;st.n_lepd2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lepdos[i].active=0;for(int i=0;i<N-2;i++)lepde[i].active=0;for(int i=0;i<N-4;i++)lepd2[i].active=0;for(int i=0;i<N-6;i++)lepdac[i].active=0;for(int i=0;i<N-6;i++)lepdam[i].active=0;init=1;ps("[LEPD] Lepidogrammitis initialized\n");return 0;}
int lepd_planning(int t,int c,int a,int b,int d,int e,int y){return add(lepdos,&st.n_lepdop,&st.t_f1,N,t,c,a,b,d,e,y);}
int lepd_execution(int t,int c,int a,int b,int d,int e,int y){return add(lepde,&st.n_lepd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lepd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lepd2,&st.n_lepd2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lepd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lepdac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lepd_market(int t,int c,int a,int b,int d,int e,int y){return add(lepdam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lepd_report(void){ps("[LEPD] Lepdp: ");pi(st.n_lepdop);ps(" PCS=");pi(st.t_f1);ps("\nLepde: ");pi(st.n_lepd);ps(" PCS=");pi(st.t_f2);ps("\nLepd2: ");pi(st.n_lepd2);ps(" PCS=");pi(st.t_f3);ps("\nLdp: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lepd_state(void){ps("[LEPD] Lepdp=");pi(st.n_lepdop);ps(" Lepd=");pi(st.n_lepd);ps(" Ldp2=");pi(st.n_lepd2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lepidogrammitis Admin Demo ===\n\n");lepd_init();
ps("Lepidogrammitis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lepd_planning(t,c,1554-(i*17),1543-(i*14),1523-(i*10),1505-(i*6),2020+(i%5));}
ps("\nLepidogrammitis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lepd_execution(t,c,1543-(i*15),1532-(i*12),1514-(i*8),1501-(i*5),2021+(i%4));}
ps("\nLepidogrammitis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lepd_evaluation(t,c,1535-(i*13),1524-(i*10),1508-(i*7),1497-(i*4),2022+(i%3));}
ps("\nLepidogrammitis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lepd_accessory(t,c,1527-(i*11),1518-(i*9),1504-(i*6),1494-(i*3),2023+(i%2));}
ps("\nLepidogrammitis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lepd_market(t,c,1521-(i*9),1512-(i*7),1499-(i*5),1491-(i*3),2024);}
ps("\n");lepd_report();lepd_state();ps("\n=== Demo Complete ===\n");return 0;}
