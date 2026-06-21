/* chelone_admin: Chelone management technology administration (v1.0)
 * Chelone planning, chelone execution, chelone evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} che_t;
typedef struct{int n_chep,n_chee,n_che2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} che_state_t;
static che_t cheps[N],chess[N-2],chevss[N-4],cheas[N-6],chemks[N-6]; static che_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(che_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;che_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int che_init(void){if(init)return -1;st.n_chep=0;st.n_chee=0;st.n_che2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cheps[i].active=0;for(int i=0;i<N-2;i++)chess[i].active=0;for(int i=0;i<N-4;i++)chevss[i].active=0;for(int i=0;i<N-6;i++)cheas[i].active=0;for(int i=0;i<N-6;i++)chemks[i].active=0;init=1;ps("[CHE] Chelone initialized\n");return 0;}
int che_planning(int t,int c,int a,int b,int d,int e,int y){return add(cheps,&st.n_chep,&st.t_f1,N,t,c,a,b,d,e,y);}
int che_execution(int t,int c,int a,int b,int d,int e,int y){return add(chess,&st.n_chee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int che_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chevss,&st.n_che2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int che_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cheas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int che_market(int t,int c,int a,int b,int d,int e,int y){return add(chemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void che_report(void){ps("[CHE] Chep: ");pi(st.n_chep);ps(" PCS=");pi(st.t_f1);ps("\nChee: ");pi(st.n_chee);ps(" PCS=");pi(st.t_f2);ps("\nChev: ");pi(st.n_che2);ps(" PCS=");pi(st.t_f3);ps("\nChec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void che_state(void){ps("[CHE] Chep=");pi(st.n_chep);ps(" Chee=");pi(st.n_chee);ps(" Chev=");pi(st.n_che2);ps(" Chec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chelone Admin Demo ===\n\n");che_init();
ps("Chelone planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;che_planning(t,c,908+(i*17),897+(i*14),877+(i*10),859+(i*6),2020+(i%5));}
ps("\nChelone execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;che_execution(t,c,897+(i*15),886+(i*12),868+(i*8),855+(i*5),2021+(i%4));}
ps("\nChelone evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;che_evaluation(t,c,889+(i*13),878+(i*10),862+(i*7),851+(i*4),2022+(i%3));}
ps("\nChelone accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;che_accessory(t,c,881+(i*11),872+(i*9),858+(i*6),848+(i*3),2023+(i%2));}
ps("\nChelone marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;che_market(t,c,875+(i*9),866+(i*7),853+(i*5),845+(i*3),2024);}
ps("\n");che_report();che_state();ps("\n=== Demo Complete ===\n");return 0;}
