/* hedera_admin: Hedera management technology administration (v1.0)
 * Hedera planning, hedera execution, hedera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hed_t;
typedef struct{int n_hedp,n_hede,n_hed2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hed_state_t;
static hed_t hedps[N],hedss[N-2],hedvss[N-4],hedas[N-6],hedmks[N-6]; static hed_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hed_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hed_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hed_init(void){if(init)return -1;st.n_hedp=0;st.n_hede=0;st.n_hed2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hedps[i].active=0;for(int i=0;i<N-2;i++)hedss[i].active=0;for(int i=0;i<N-4;i++)hedvss[i].active=0;for(int i=0;i<N-6;i++)hedas[i].active=0;for(int i=0;i<N-6;i++)hedmks[i].active=0;init=1;ps("[HED] Hedera initialized\n");return 0;}
int hed_planning(int t,int c,int a,int b,int d,int e,int y){return add(hedps,&st.n_hedp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hed_execution(int t,int c,int a,int b,int d,int e,int y){return add(hedss,&st.n_hede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hed_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hedvss,&st.n_hed2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hed_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hedas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hed_market(int t,int c,int a,int b,int d,int e,int y){return add(hedmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hed_report(void){ps("[HED] Hedp: ");pi(st.n_hedp);ps(" PCS=");pi(st.t_f1);ps("\nHede: ");pi(st.n_hede);ps(" PCS=");pi(st.t_f2);ps("\nHedv: ");pi(st.n_hed2);ps(" PCS=");pi(st.t_f3);ps("\nHedc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hed_state(void){ps("[HED] Hedp=");pi(st.n_hedp);ps(" Hede=");pi(st.n_hede);ps(" Hedv=");pi(st.n_hed2);ps(" Hedc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hedera Admin Demo ===\n\n");hed_init();
ps("Hedera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hed_planning(t,c,1019+(i*17),1008+(i*14),988+(i*10),970+(i*6),2020+(i%5));}
ps("\nHedera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hed_execution(t,c,1008+(i*15),997+(i*12),979+(i*8),966+(i*5),2021+(i%4));}
ps("\nHedera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hed_evaluation(t,c,1000+(i*13),989+(i*10),973+(i*7),962+(i*4),2022+(i%3));}
ps("\nHedera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hed_accessory(t,c,992+(i*11),983+(i*9),969+(i*6),959+(i*3),2023+(i%2));}
ps("\nHedera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hed_market(t,c,986+(i*9),977+(i*7),964+(i*5),956+(i*3),2024);}
ps("\n");hed_report();hed_state();ps("\n=== Demo Complete ===\n");return 0;}
