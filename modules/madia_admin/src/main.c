/* madia_admin: Madia management technology administration (v1.0)
 * Madia planning, madia execution, madia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mad_t;
typedef struct{int n_madp,n_made,n_mad2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mad_state_t;
static mad_t madps[N],madss[N-2],madvss[N-4],madas[N-6],madmks[N-6]; static mad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mad_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mad_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mad_init(void){if(init)return -1;st.n_madp=0;st.n_made=0;st.n_mad2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)madps[i].active=0;for(int i=0;i<N-2;i++)madss[i].active=0;for(int i=0;i<N-4;i++)madvss[i].active=0;for(int i=0;i<N-6;i++)madas[i].active=0;for(int i=0;i<N-6;i++)madmks[i].active=0;init=1;ps("[MAD] Madia initialized\n");return 0;}
int mad_planning(int t,int c,int a,int b,int d,int e,int y){return add(madps,&st.n_madp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mad_execution(int t,int c,int a,int b,int d,int e,int y){return add(madss,&st.n_made,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mad_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(madvss,&st.n_mad2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mad_accessory(int t,int c,int a,int b,int d,int e,int y){return add(madas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mad_market(int t,int c,int a,int b,int d,int e,int y){return add(madmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mad_report(void){ps("[MAD] Madpp: ");pi(st.n_madp);ps(" PCS=");pi(st.t_f1);ps("\nMade: ");pi(st.n_made);ps(" PCS=");pi(st.t_f2);ps("\nMadv: ");pi(st.n_mad2);ps(" PCS=");pi(st.t_f3);ps("\nMadc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mad_state(void){ps("[MAD] Madpp=");pi(st.n_madp);ps(" Made=");pi(st.n_made);ps(" Madv=");pi(st.n_mad2);ps(" Madc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Madia Admin Demo ===\n\n");mad_init();
ps("Madia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mad_planning(t,c,1157+(i*17),1146+(i*14),1126+(i*10),1108+(i*6),2020+(i%5));}
ps("\nMadia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mad_execution(t,c,1146+(i*15),1135+(i*12),1117+(i*8),1104+(i*5),2021+(i%4));}
ps("\nMadia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mad_evaluation(t,c,1138+(i*13),1127+(i*10),1111+(i*7),1100+(i*4),2022+(i%3));}
ps("\nMadia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mad_accessory(t,c,1130+(i*11),1121+(i*9),1107+(i*6),1097+(i*3),2023+(i%2));}
ps("\nMadia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mad_market(t,c,1124+(i*9),1115+(i*7),1102+(i*5),1094+(i*3),2024);}
ps("\n");mad_report();mad_state();ps("\n=== Demo Complete ===\n");return 0;}
