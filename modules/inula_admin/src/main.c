/* inula_admin: Inula management technology administration (v1.0)
 * Inula planning, inula execution, inula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} inu_t;
typedef struct{int n_inup,n_inue,n_inu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} inu_state_t;
static inu_t inups[N],inuss[N-2],inuvss[N-4],inuas[N-6],inumks[N-6]; static inu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(inu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;inu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[INU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int inu_init(void){if(init)return -1;st.n_inup=0;st.n_inue=0;st.n_inu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)inups[i].active=0;for(int i=0;i<N-2;i++)inuss[i].active=0;for(int i=0;i<N-4;i++)inuvss[i].active=0;for(int i=0;i<N-6;i++)inuas[i].active=0;for(int i=0;i<N-6;i++)inumks[i].active=0;init=1;ps("[INU] Inula initialized\n");return 0;}
int inu_planning(int t,int c,int a,int b,int d,int e,int y){return add(inups,&st.n_inup,&st.t_f1,N,t,c,a,b,d,e,y);}
int inu_execution(int t,int c,int a,int b,int d,int e,int y){return add(inuss,&st.n_inue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int inu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(inuvss,&st.n_inu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int inu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(inuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int inu_market(int t,int c,int a,int b,int d,int e,int y){return add(inumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void inu_report(void){ps("[INU] Inupp: ");pi(st.n_inup);ps(" PCS=");pi(st.t_f1);ps("\nInue: ");pi(st.n_inue);ps(" PCS=");pi(st.t_f2);ps("\nInuv: ");pi(st.n_inu2);ps(" PCS=");pi(st.t_f3);ps("\nInuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void inu_state(void){ps("[INU] Inupp=");pi(st.n_inup);ps(" Inue=");pi(st.n_inue);ps(" Inuv=");pi(st.n_inu2);ps(" Inuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Inula Admin Demo ===\n\n");inu_init();
ps("Inula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;inu_planning(t,c,1150+(i*17),1139+(i*14),1119+(i*10),1101+(i*6),2020+(i%5));}
ps("\nInula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;inu_execution(t,c,1139+(i*15),1128+(i*12),1110+(i*8),1097+(i*5),2021+(i%4));}
ps("\nInula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;inu_evaluation(t,c,1131+(i*13),1120+(i*10),1104+(i*7),1093+(i*4),2022+(i%3));}
ps("\nInula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;inu_accessory(t,c,1123+(i*11),1114+(i*9),1100+(i*6),1090+(i*3),2023+(i%2));}
ps("\nInula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;inu_market(t,c,1117+(i*9),1108+(i*7),1095+(i*5),1087+(i*3),2024);}
ps("\n");inu_report();inu_state();ps("\n=== Demo Complete ===\n");return 0;}
