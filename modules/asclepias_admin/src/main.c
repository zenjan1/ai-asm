/* asclepias_admin: Asclepias management technology administration (v1.0)
 * Asclepias planning, asclepias execution, asclepias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asc_t;
typedef struct{int n_ascp,n_asce,n_asc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asc_state_t;
static asc_t ascps[N],ascss[N-2],ascvss[N-4],ascas[N-6],ascmks[N-6]; static asc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asc_init(void){if(init)return -1;st.n_ascp=0;st.n_asce=0;st.n_asc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ascps[i].active=0;for(int i=0;i<N-2;i++)ascss[i].active=0;for(int i=0;i<N-4;i++)ascvss[i].active=0;for(int i=0;i<N-6;i++)ascas[i].active=0;for(int i=0;i<N-6;i++)ascmks[i].active=0;init=1;ps("[ASC] Asclepias initialized\n");return 0;}
int asc_planning(int t,int c,int a,int b,int d,int e,int y){return add(ascps,&st.n_ascp,&st.t_f1,N,t,c,a,b,d,e,y);}
int asc_execution(int t,int c,int a,int b,int d,int e,int y){return add(ascss,&st.n_asce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ascvss,&st.n_asc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ascas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asc_market(int t,int c,int a,int b,int d,int e,int y){return add(ascmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asc_report(void){ps("[ASC] Ascp: ");pi(st.n_ascp);ps(" PCS=");pi(st.t_f1);ps("\nAsce: ");pi(st.n_asce);ps(" PCS=");pi(st.t_f2);ps("\nAscv: ");pi(st.n_asc2);ps(" PCS=");pi(st.t_f3);ps("\nAscc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asc_state(void){ps("[ASC] Ascp=");pi(st.n_ascp);ps(" Asce=");pi(st.n_asce);ps(" Ascv=");pi(st.n_asc2);ps(" Ascc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Asclepias Admin Demo ===\n\n");asc_init();
ps("Asclepias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asc_planning(t,c,974+(i*17),963+(i*14),943+(i*10),925+(i*6),2020+(i%5));}
ps("\nAsclepias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asc_execution(t,c,963+(i*15),952+(i*12),934+(i*8),921+(i*5),2021+(i%4));}
ps("\nAsclepias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asc_evaluation(t,c,955+(i*13),944+(i*10),928+(i*7),917+(i*4),2022+(i%3));}
ps("\nAsclepias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asc_accessory(t,c,947+(i*11),938+(i*9),924+(i*6),914+(i*3),2023+(i%2));}
ps("\nAsclepias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asc_market(t,c,941+(i*9),932+(i*7),919+(i*5),911+(i*3),2024);}
ps("\n");asc_report();asc_state();ps("\n=== Demo Complete ===\n");return 0;}
