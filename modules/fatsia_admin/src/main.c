/* fatsia_admin: Fatsia management technology administration (v1.0)
 * Fatsia planning, fatsia execution, fatsia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fa_t;
typedef struct{int n_fts,n_fte,n_ftv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fa_state_t;
static fa_t ftss[N],ftess[N-2],ftvss[N-4],ftas[N-6],ftmks[N-6]; static fa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FTS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fts_init(void){if(init)return -1;st.n_fts=0;st.n_fte=0;st.n_ftv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ftss[i].active=0;for(int i=0;i<N-2;i++)ftess[i].active=0;for(int i=0;i<N-4;i++)ftvss[i].active=0;for(int i=0;i<N-6;i++)ftas[i].active=0;for(int i=0;i<N-6;i++)ftmks[i].active=0;init=1;ps("[FTS] Fatsia initialized\n");return 0;}
int fts_planning(int t,int c,int a,int b,int d,int e,int y){return add(ftss,&st.n_fts,&st.t_f1,N,t,c,a,b,d,e,y);}
int fts_execution(int t,int c,int a,int b,int d,int e,int y){return add(ftess,&st.n_fte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fts_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ftvss,&st.n_ftv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fts_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ftas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fts_market(int t,int c,int a,int b,int d,int e,int y){return add(ftmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fts_report(void){ps("[FTS] Fts: ");pi(st.n_fts);ps(" PCS=");pi(st.t_f1);ps("\nFte: ");pi(st.n_fte);ps(" PCS=");pi(st.t_f2);ps("\nFtv: ");pi(st.n_ftv);ps(" PCS=");pi(st.t_f3);ps("\nFtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fts_state(void){ps("[FTS] Fts=");pi(st.n_fts);ps(" Fte=");pi(st.n_fte);ps(" Ftv=");pi(st.n_ftv);ps(" Ftc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fatsia Admin Demo ===\n\n");fts_init();
ps("Fatsia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fts_planning(t,c,772+(i*17),761+(i*14),741+(i*10),723+(i*6),2020+(i%5));}
ps("\nFatsia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fts_execution(t,c,761+(i*15),750+(i*12),732+(i*8),719+(i*5),2021+(i%4));}
ps("\nFatsia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fts_evaluation(t,c,753+(i*13),742+(i*10),728+(i*7),717+(i*4),2022+(i%3));}
ps("\nFatsia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fts_accessory(t,c,745+(i*11),736+(i*9),722+(i*6),712+(i*3),2023+(i%2));}
ps("\nFatsia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fts_market(t,c,739+(i*9),730+(i*7),717+(i*5),709+(i*3),2024);}
ps("\n");fts_report();fts_state();ps("\n=== Demo Complete ===\n");return 0;}
