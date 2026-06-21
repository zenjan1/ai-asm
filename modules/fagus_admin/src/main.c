/* fagus_admin: Fagus management technology administration (v1.0)
 * Fagus planning, fagus execution, fagus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fag_t;
typedef struct{int n_fagp,n_fage,n_fag2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fag_state_t;
static fag_t fagps[N],fagss[N-2],fagvss[N-4],fagas[N-6],fagmks[N-6]; static fag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fag_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fag_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FAG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fag_init(void){if(init)return -1;st.n_fagp=0;st.n_fage=0;st.n_fag2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fagps[i].active=0;for(int i=0;i<N-2;i++)fagss[i].active=0;for(int i=0;i<N-4;i++)fagvss[i].active=0;for(int i=0;i<N-6;i++)fagas[i].active=0;for(int i=0;i<N-6;i++)fagmks[i].active=0;init=1;ps("[FAG] Fagus initialized\n");return 0;}
int fag_planning(int t,int c,int a,int b,int d,int e,int y){return add(fagps,&st.n_fagp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fag_execution(int t,int c,int a,int b,int d,int e,int y){return add(fagss,&st.n_fage,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fag_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fagvss,&st.n_fag2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fag_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fagas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fag_market(int t,int c,int a,int b,int d,int e,int y){return add(fagmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fag_report(void){ps("[FAG] Fagp: ");pi(st.n_fagp);ps(" PCS=");pi(st.t_f1);ps("\nFage: ");pi(st.n_fage);ps(" PCS=");pi(st.t_f2);ps("\nFagv: ");pi(st.n_fag2);ps(" PCS=");pi(st.t_f3);ps("\nFagc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fag_state(void){ps("[FAG] Fagp=");pi(st.n_fagp);ps(" Fage=");pi(st.n_fage);ps(" Fagv=");pi(st.n_fag2);ps(" Fagc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fagus Admin Demo ===\n\n");fag_init();
ps("Fagus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fag_planning(t,c,1007+(i*17),996+(i*14),976+(i*10),958+(i*6),2020+(i%5));}
ps("\nFagus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fag_execution(t,c,996+(i*15),985+(i*12),967+(i*8),954+(i*5),2021+(i%4));}
ps("\nFagus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fag_evaluation(t,c,988+(i*13),977+(i*10),961+(i*7),950+(i*4),2022+(i%3));}
ps("\nFagus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fag_accessory(t,c,980+(i*11),971+(i*9),957+(i*6),947+(i*3),2023+(i%2));}
ps("\nFagus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fag_market(t,c,974+(i*9),965+(i*7),952+(i*5),944+(i*3),2024);}
ps("\n");fag_report();fag_state();ps("\n=== Demo Complete ===\n");return 0;}
