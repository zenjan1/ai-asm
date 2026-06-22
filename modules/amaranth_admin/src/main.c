/* amaranth_admin: Amaranth management technology administration (v1.0)
 * Amaranth planning, amaranth execution, amaranth evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} amar_t;
typedef struct{int n_amarpp,n_amar,n_amar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} amar_state_t;
static amar_t amars[N],amare[N-2],amarv[N-4],amarac[N-6],amarm[N-6]; static amar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(amar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;amar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int amar_init(void){if(init)return -1;st.n_amarpp=0;st.n_amar=0;st.n_amar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)amars[i].active=0;for(int i=0;i<N-2;i++)amare[i].active=0;for(int i=0;i<N-4;i++)amarv[i].active=0;for(int i=0;i<N-6;i++)amarac[i].active=0;for(int i=0;i<N-6;i++)amarm[i].active=0;init=1;ps("[AMAR] Amaranth initialized\n");return 0;}
int amar_planning(int t,int c,int a,int b,int d,int e,int y){return add(amars,&st.n_amarpp,&st.t_f1,N,t,c,a,b,d,e,y);}
int amar_execution(int t,int c,int a,int b,int d,int e,int y){return add(amare,&st.n_amar,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int amar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(amarv,&st.n_amar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int amar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(amarac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int amar_market(int t,int c,int a,int b,int d,int e,int y){return add(amarm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void amar_report(void){ps("[AMAR] Marpp: ");pi(st.n_amarpp);ps(" PCS=");pi(st.t_f1);ps("\nMare: ");pi(st.n_amar);ps(" PCS=");pi(st.t_f2);ps("\nMarv: ");pi(st.n_amar2);ps(" PCS=");pi(st.t_f3);ps("\nMarac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void amar_state(void){ps("[AMAR] Marpp=");pi(st.n_amarpp);ps(" Mare=");pi(st.n_amar);ps(" Marv=");pi(st.n_amar2);ps(" Marac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Amaranth Admin Demo ===\n\n");amar_init();
ps("Amaranth planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;amar_planning(t,c,1400+(i*17),1389+(i*14),1369+(i*10),1351+(i*6),2020+(i%5));}
ps("\nAmaranth execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;amar_execution(t,c,1389+(i*15),1378+(i*12),1360+(i*8),1347+(i*5),2021+(i%4));}
ps("\nAmaranth evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;amar_evaluation(t,c,1381+(i*13),1370+(i*10),1354+(i*7),1343+(i*4),2022+(i%3));}
ps("\nAmaranth accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amar_accessory(t,c,1373+(i*11),1364+(i*9),1350+(i*6),1340+(i*3),2023+(i%2));}
ps("\nAmaranth marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amar_market(t,c,1367+(i*9),1358+(i*7),1345+(i*5),1337+(i*3),2024);}
ps("\n");amar_report();amar_state();ps("\n=== Demo Complete ===\n");return 0;}
