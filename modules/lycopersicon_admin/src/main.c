/* lycopersicon_admin: Lycopersicon management technology administration (v1.0)
 * Lycopersicon planning, lycopersicon execution, lycopersicon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ly_t;
typedef struct{int n_lyp,n_lye,n_lyv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ly_state_t;
static ly_t lyps[N],lyss[N-2],lyvss[N-4],lyas[N-6],lymks[N-6]; static ly_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ly_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ly_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lyc_init(void){if(init)return -1;st.n_lyp=0;st.n_lye=0;st.n_lyv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lyps[i].active=0;for(int i=0;i<N-2;i++)lyss[i].active=0;for(int i=0;i<N-4;i++)lyvss[i].active=0;for(int i=0;i<N-6;i++)lyas[i].active=0;for(int i=0;i<N-6;i++)lymks[i].active=0;init=1;ps("[LYC] Lycopersicon initialized\n");return 0;}
int lyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(lyps,&st.n_lyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(lyss,&st.n_lye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lyvss,&st.n_lyv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lyc_market(int t,int c,int a,int b,int d,int e,int y){return add(lymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lyc_report(void){ps("[LYC] Lyp: ");pi(st.n_lyp);ps(" PCS=");pi(st.t_f1);ps("\nLye: ");pi(st.n_lye);ps(" PCS=");pi(st.t_f2);ps("\nLyv: ");pi(st.n_lyv);ps(" PCS=");pi(st.t_f3);ps("\nLyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lyc_state(void){ps("[LYC] Lyp=");pi(st.n_lyp);ps(" Lye=");pi(st.n_lye);ps(" Lyv=");pi(st.n_lyv);ps(" Lyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lycopersicon Admin Demo ===\n\n");lyc_init();
ps("Lycopersicon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lyc_planning(t,c,730+(i*17),719+(i*14),699+(i*10),681+(i*6),2020+(i%5));}
ps("\nLycopersicon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lyc_execution(t,c,719+(i*15),708+(i*12),690+(i*8),677+(i*5),2021+(i%4));}
ps("\nLycopersicon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lyc_evaluation(t,c,711+(i*13),700+(i*10),684+(i*7),673+(i*4),2022+(i%3));}
ps("\nLycopersicon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyc_accessory(t,c,703+(i*11),694+(i*9),680+(i*6),670+(i*3),2023+(i%2));}
ps("\nLycopersicon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyc_market(t,c,697+(i*9),688+(i*7),675+(i*5),667+(i*3),2024);}
ps("\n");lyc_report();lyc_state();ps("\n=== Demo Complete ===\n");return 0;}
