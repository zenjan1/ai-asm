/* anagyris_admin: Anagyris management technology administration (v1.0)
 * Anagyris planning, anagyris execution, anagyris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anag_t;
typedef struct{int n_anagp,n_anag,n_anag2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anag_state_t;
static anag_t anags[N],anage[N-2],anag2[N-4],anagac[N-6],anagam[N-6]; static anag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(anag_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anag_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANAG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anag_init(void){if(init)return -1;st.n_anagp=0;st.n_anag=0;st.n_anag2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anags[i].active=0;for(int i=0;i<N-2;i++)anage[i].active=0;for(int i=0;i<N-4;i++)anag2[i].active=0;for(int i=0;i<N-6;i++)anagac[i].active=0;for(int i=0;i<N-6;i++)anagam[i].active=0;init=1;ps("[ANAG] Anagyris initialized\n");return 0;}
int anag_planning(int t,int c,int a,int b,int d,int e,int y){return add(anags,&st.n_anagp,&st.t_f1,N,t,c,a,b,d,e,y);}
int anag_execution(int t,int c,int a,int b,int d,int e,int y){return add(anage,&st.n_anag,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anag_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anag2,&st.n_anag2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anag_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anagac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anag_market(int t,int c,int a,int b,int d,int e,int y){return add(anagam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anag_report(void){ps("[ANAG] Nagp: ");pi(st.n_anagp);ps(" PCS=");pi(st.t_f1);ps("\nNage: ");pi(st.n_anag);ps(" PCS=");pi(st.t_f2);ps("\nNag2: ");pi(st.n_anag2);ps(" PCS=");pi(st.t_f3);ps("\nNagac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anag_state(void){ps("[ANAG] Nagp=");pi(st.n_anagp);ps(" Nage=");pi(st.n_anag);ps(" Nag2=");pi(st.n_anag2);ps(" Nagac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anagyris Admin Demo ===\n\n");anag_init();
ps("Anagyris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anag_planning(t,c,1404+(i*17),1393+(i*14),1373+(i*10),1355+(i*6),2020+(i%5));}
ps("\nAnagyris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anag_execution(t,c,1393+(i*15),1382+(i*12),1364+(i*8),1351+(i*5),2021+(i%4));}
ps("\nAnagyris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anag_evaluation(t,c,1385+(i*13),1374+(i*10),1358+(i*7),1347+(i*4),2022+(i%3));}
ps("\nAnagyris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anag_accessory(t,c,1377+(i*11),1368+(i*9),1354+(i*6),1344+(i*3),2023+(i%2));}
ps("\nAnagyris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anag_market(t,c,1371+(i*9),1362+(i*7),1349+(i*5),1341+(i*3),2024);}
ps("\n");anag_report();anag_state();ps("\n=== Demo Complete ===\n");return 0;}
