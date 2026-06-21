/* ochna_admin: Ochna management technology administration (v1.0)
 * Ochna planning, ochna execution, ochna evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} och_t;
typedef struct{int n_ochp,n_och2,n_ochv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} och_state_t;
static och_t ochps[N],ochss[N-2],ochvss[N-4],ochas[N-6],ochmks[N-6]; static och_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(och_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;och_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int och_init(void){if(init)return -1;st.n_ochp=0;st.n_och2=0;st.n_ochv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ochps[i].active=0;for(int i=0;i<N-2;i++)ochss[i].active=0;for(int i=0;i<N-4;i++)ochvss[i].active=0;for(int i=0;i<N-6;i++)ochas[i].active=0;for(int i=0;i<N-6;i++)ochmks[i].active=0;init=1;ps("[OCH] Ochna initialized\n");return 0;}
int och_planning(int t,int c,int a,int b,int d,int e,int y){return add(ochps,&st.n_ochp,&st.t_f1,N,t,c,a,b,d,e,y);}
int och_execution(int t,int c,int a,int b,int d,int e,int y){return add(ochss,&st.n_och2,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int och_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ochvss,&st.n_ochv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int och_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ochas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int och_market(int t,int c,int a,int b,int d,int e,int y){return add(ochmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void och_report(void){ps("[OCH] Ochp: ");pi(st.n_ochp);ps(" PCS=");pi(st.t_f1);ps("\nOch2: ");pi(st.n_och2);ps(" PCS=");pi(st.t_f2);ps("\nOchv: ");pi(st.n_ochv);ps(" PCS=");pi(st.t_f3);ps("\nOchc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void och_state(void){ps("[OCH] Ochp=");pi(st.n_ochp);ps(" Och2=");pi(st.n_och2);ps(" Ochv=");pi(st.n_ochv);ps(" Ochc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ochna Admin Demo ===\n\n");och_init();
ps("Ochna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;och_planning(t,c,1072+(i*17),1061+(i*14),1041+(i*10),1023+(i*6),2020+(i%5));}
ps("\nOchna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;och_execution(t,c,1061+(i*15),1050+(i*12),1032+(i*8),1019+(i*5),2021+(i%4));}
ps("\nOchna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;och_evaluation(t,c,1053+(i*13),1042+(i*10),1026+(i*7),1015+(i*4),2022+(i%3));}
ps("\nOchna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;och_accessory(t,c,1045+(i*11),1036+(i*9),1022+(i*6),1012+(i*3),2023+(i%2));}
ps("\nOchna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;och_market(t,c,1039+(i*9),1030+(i*7),1017+(i*5),1009+(i*3),2024);}
ps("\n");och_report();och_state();ps("\n=== Demo Complete ===\n");return 0;}
