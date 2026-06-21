/* arum_admin: Arum management technology administration (v1.0)
 * Arum planning, arum execution, arum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aru_t;
typedef struct{int n_arup,n_arue,n_aru2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aru_state_t;
static aru_t arups[N],aruss[N-2],aruvss[N-4],ar_uas[N-6],ar_umks[N-6]; static aru_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aru_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aru_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aru_init(void){if(init)return -1;st.n_arup=0;st.n_arue=0;st.n_aru2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arups[i].active=0;for(int i=0;i<N-2;i++)aruss[i].active=0;for(int i=0;i<N-4;i++)aruvss[i].active=0;for(int i=0;i<N-6;i++)ar_uas[i].active=0;for(int i=0;i<N-6;i++)ar_umks[i].active=0;init=1;ps("[ARU] Arum initialized\n");return 0;}
int aru_planning(int t,int c,int a,int b,int d,int e,int y){return add(arups,&st.n_arup,&st.t_f1,N,t,c,a,b,d,e,y);}
int aru_execution(int t,int c,int a,int b,int d,int e,int y){return add(aruss,&st.n_arue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aru_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aruvss,&st.n_aru2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aru_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ar_uas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aru_market(int t,int c,int a,int b,int d,int e,int y){return add(ar_umks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aru_report(void){ps("[ARU] Arup: ");pi(st.n_arup);ps(" PCS=");pi(st.t_f1);ps("\nArue: ");pi(st.n_arue);ps(" PCS=");pi(st.t_f2);ps("\nAruv: ");pi(st.n_aru2);ps(" PCS=");pi(st.t_f3);ps("\nAruc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aru_state(void){ps("[ARU] Arup=");pi(st.n_arup);ps(" Arue=");pi(st.n_arue);ps(" Aruv=");pi(st.n_aru2);ps(" Aruc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arum Admin Demo ===\n\n");aru_init();
ps("Arum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aru_planning(t,c,973+(i*17),962+(i*14),942+(i*10),924+(i*6),2020+(i%5));}
ps("\nArum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aru_execution(t,c,962+(i*15),951+(i*12),933+(i*8),920+(i*5),2021+(i%4));}
ps("\nArum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aru_evaluation(t,c,954+(i*13),943+(i*10),927+(i*7),916+(i*4),2022+(i%3));}
ps("\nArum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aru_accessory(t,c,946+(i*11),937+(i*9),923+(i*6),913+(i*3),2023+(i%2));}
ps("\nArum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aru_market(t,c,940+(i*9),931+(i*7),918+(i*5),910+(i*3),2024);}
ps("\n");aru_report();aru_state();ps("\n=== Demo Complete ===\n");return 0;}
