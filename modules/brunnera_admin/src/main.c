/* brunnera_admin: Brunnera management technology administration (v1.0)
 * Brunnera planning, brunnera execution, brunnera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bru_t;
typedef struct{int n_brup,n_brue,n_bru2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bru_state_t;
static bru_t brups[N],bruss[N-2],bruvss[N-4],bruas[N-6],brumks[N-6]; static bru_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bru_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bru_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bru_init(void){if(init)return -1;st.n_brup=0;st.n_brue=0;st.n_bru2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brups[i].active=0;for(int i=0;i<N-2;i++)bruss[i].active=0;for(int i=0;i<N-4;i++)bruvss[i].active=0;for(int i=0;i<N-6;i++)bruas[i].active=0;for(int i=0;i<N-6;i++)brumks[i].active=0;init=1;ps("[BRU] Brunnera initialized\n");return 0;}
int bru_planning(int t,int c,int a,int b,int d,int e,int y){return add(brups,&st.n_brup,&st.t_f1,N,t,c,a,b,d,e,y);}
int bru_execution(int t,int c,int a,int b,int d,int e,int y){return add(bruss,&st.n_brue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bru_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bruvss,&st.n_bru2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bru_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bruas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bru_market(int t,int c,int a,int b,int d,int e,int y){return add(brumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bru_report(void){ps("[BRU] Brup: ");pi(st.n_brup);ps(" PCS=");pi(st.t_f1);ps("\nBrue: ");pi(st.n_brue);ps(" PCS=");pi(st.t_f2);ps("\nBruv: ");pi(st.n_bru2);ps(" PCS=");pi(st.t_f3);ps("\nBruc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bru_state(void){ps("[BRU] Brup=");pi(st.n_brup);ps(" Brue=");pi(st.n_brue);ps(" Bruv=");pi(st.n_bru2);ps(" Bruc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Brunnera Admin Demo ===\n\n");bru_init();
ps("Brunnera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bru_planning(t,c,902+(i*17),891+(i*14),871+(i*10),853+(i*6),2020+(i%5));}
ps("\nBrunnera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bru_execution(t,c,891+(i*15),880+(i*12),862+(i*8),849+(i*5),2021+(i%4));}
ps("\nBrunnera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bru_evaluation(t,c,883+(i*13),872+(i*10),856+(i*7),845+(i*4),2022+(i%3));}
ps("\nBrunnera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bru_accessory(t,c,875+(i*11),866+(i*9),852+(i*6),842+(i*3),2023+(i%2));}
ps("\nBrunnera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bru_market(t,c,869+(i*9),860+(i*7),847+(i*5),839+(i*3),2024);}
ps("\n");bru_report();bru_state();ps("\n=== Demo Complete ===\n");return 0;}
