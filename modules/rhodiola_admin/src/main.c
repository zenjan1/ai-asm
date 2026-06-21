/* rhodiola_admin: Rhodiola management technology administration (v1.0)
 * Rhodiola planning, rhodiola execution, rhodiola evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rhl_t;
typedef struct{int n_rlp,n_rle,n_rlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rhl_state_t;
static rhl_t rhlps[N],rhlss[N-2],rhlvss[N-4],rhlas[N-6],rhlms[N-6]; static rhl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rhl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rhl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rhl_init(void){if(init)return -1;st.n_rlp=0;st.n_rle=0;st.n_rlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhlps[i].active=0;for(int i=0;i<N-2;i++)rhlss[i].active=0;for(int i=0;i<N-4;i++)rhlvss[i].active=0;for(int i=0;i<N-6;i++)rhlas[i].active=0;for(int i=0;i<N-6;i++)rhlms[i].active=0;init=1;ps("[RHL] Rhodiola initialized\n");return 0;}
int rhl_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhlps,&st.n_rlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rhl_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhlss,&st.n_rle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rhl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhlvss,&st.n_rlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rhl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rhl_market(int t,int c,int a,int b,int d,int e,int y){return add(rhlms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rhl_report(void){ps("[RHL] Rlp: ");pi(st.n_rlp);ps(" PCS=");pi(st.t_f1);ps("\nRle: ");pi(st.n_rle);ps(" PCS=");pi(st.t_f2);ps("\nRlv: ");pi(st.n_rlv);ps(" PCS=");pi(st.t_f3);ps("\nRlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rhl_state(void){ps("[RHL] Rlp=");pi(st.n_rlp);ps(" Rle=");pi(st.n_rle);ps(" Rlv=");pi(st.n_rlv);ps(" Rlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhodiola Admin Demo ===\n\n");rhl_init();
ps("Rhodiola planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rhl_planning(t,c,857+(i*17),846+(i*14),826+(i*10),808+(i*6),2020+(i%5));}
ps("\nRhodiola execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rhl_execution(t,c,846+(i*15),835+(i*12),817+(i*8),804+(i*5),2021+(i%4));}
ps("\nRhodiola evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rhl_evaluation(t,c,838+(i*13),827+(i*10),811+(i*7),800+(i*4),2022+(i%3));}
ps("\nRhodiola accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhl_accessory(t,c,830+(i*11),821+(i*9),807+(i*6),797+(i*3),2023+(i%2));}
ps("\nRhodiola marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhl_market(t,c,824+(i*9),815+(i*7),802+(i*5),794+(i*3),2024);}
ps("\n");rhl_report();rhl_state();ps("\n=== Demo Complete ===\n");return 0;}
