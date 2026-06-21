/* brunfelsia_admin: Brunfelsia management technology administration (v1.0)
 * Brunfelsia planning, brunfelsia execution, brunfelsia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bf_t;
typedef struct{int n_brp,n_bre,n_brv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bf_state_t;
static bf_t brps[N],brss[N-2],brvss[N-4],bras[N-6],brmks[N-6]; static bf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brn_init(void){if(init)return -1;st.n_brp=0;st.n_bre=0;st.n_brv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brps[i].active=0;for(int i=0;i<N-2;i++)brss[i].active=0;for(int i=0;i<N-4;i++)brvss[i].active=0;for(int i=0;i<N-6;i++)bras[i].active=0;for(int i=0;i<N-6;i++)brmks[i].active=0;init=1;ps("[BRN] Brunfelsia initialized\n");return 0;}
int brn_planning(int t,int c,int a,int b,int d,int e,int y){return add(brps,&st.n_brp,&st.t_f1,N,t,c,a,b,d,e,y);}
int brn_execution(int t,int c,int a,int b,int d,int e,int y){return add(brss,&st.n_bre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(brvss,&st.n_brv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brn_market(int t,int c,int a,int b,int d,int e,int y){return add(brmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brn_report(void){ps("[BRN] Brp: ");pi(st.n_brp);ps(" PCS=");pi(st.t_f1);ps("\nBre: ");pi(st.n_bre);ps(" PCS=");pi(st.t_f2);ps("\nBrv: ");pi(st.n_brv);ps(" PCS=");pi(st.t_f3);ps("\nBrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brn_state(void){ps("[BRN] Brp=");pi(st.n_brp);ps(" Bre=");pi(st.n_bre);ps(" Brv=");pi(st.n_brv);ps(" Brc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Brunfelsia Admin Demo ===\n\n");brn_init();
ps("Brunfelsia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brn_planning(t,c,761+(i*17),750+(i*14),730+(i*10),712+(i*6),2020+(i%5));}
ps("\nBrunfelsia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brn_execution(t,c,750+(i*15),739+(i*12),721+(i*8),708+(i*5),2021+(i%4));}
ps("\nBrunfelsia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brn_evaluation(t,c,742+(i*13),731+(i*10),717+(i*7),706+(i*4),2022+(i%3));}
ps("\nBrunfelsia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brn_accessory(t,c,734+(i*11),725+(i*9),711+(i*6),701+(i*3),2023+(i%2));}
ps("\nBrunfelsia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brn_market(t,c,728+(i*9),719+(i*7),706+(i*5),698+(i*3),2024);}
ps("\n");brn_report();brn_state();ps("\n=== Demo Complete ===\n");return 0;}
