/* asarum_admin: Asarum management technology administration (v1.0)
 * Asarum planning, asarum execution, asarum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asr_t;
typedef struct{int n_asp,n_ase,n_asv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asr_state_t;
static asr_t asrps[N],asrss[N-2],asrvss[N-4],asras[N-6],asrmks[N-6]; static asr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asr_init(void){if(init)return -1;st.n_asp=0;st.n_ase=0;st.n_asv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)asrps[i].active=0;for(int i=0;i<N-2;i++)asrss[i].active=0;for(int i=0;i<N-4;i++)asrvss[i].active=0;for(int i=0;i<N-6;i++)asras[i].active=0;for(int i=0;i<N-6;i++)asrmks[i].active=0;init=1;ps("[ASR] Asarum initialized\n");return 0;}
int asr_planning(int t,int c,int a,int b,int d,int e,int y){return add(asrps,&st.n_asp,&st.t_f1,N,t,c,a,b,d,e,y);}
int asr_execution(int t,int c,int a,int b,int d,int e,int y){return add(asrss,&st.n_ase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(asrvss,&st.n_asv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(asras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asr_market(int t,int c,int a,int b,int d,int e,int y){return add(asrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asr_report(void){ps("[ASR] Asp: ");pi(st.n_asp);ps(" PCS=");pi(st.t_f1);ps("\nAse: ");pi(st.n_ase);ps(" PCS=");pi(st.t_f2);ps("\nAsv: ");pi(st.n_asv);ps(" PCS=");pi(st.t_f3);ps("\nAsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asr_state(void){ps("[ASR] Asp=");pi(st.n_asp);ps(" Ase=");pi(st.n_ase);ps(" Asv=");pi(st.n_asv);ps(" Asc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Asarum Admin Demo ===\n\n");asr_init();
ps("Asarum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asr_planning(t,c,886+(i*17),875+(i*14),855+(i*10),837+(i*6),2020+(i%5));}
ps("\nAsarum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asr_execution(t,c,875+(i*15),864+(i*12),846+(i*8),833+(i*5),2021+(i%4));}
ps("\nAsarum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asr_evaluation(t,c,867+(i*13),856+(i*10),840+(i*7),829+(i*4),2022+(i%3));}
ps("\nAsarum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asr_accessory(t,c,859+(i*11),850+(i*9),836+(i*6),826+(i*3),2023+(i%2));}
ps("\nAsarum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asr_market(t,c,853+(i*9),844+(i*7),831+(i*5),823+(i*3),2024);}
ps("\n");asr_report();asr_state();ps("\n=== Demo Complete ===\n");return 0;}
