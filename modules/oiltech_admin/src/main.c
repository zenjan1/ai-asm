/* oiltech_admin: Oil technology administration (v1.0)
 * Oil crop planting, oil extraction, oil refining, oil products, oil marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} olt_t;
typedef struct{int n_oc,n_oe,n_or,n_op,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} olt_state_t;
static olt_t ocs[N],oes[N-2],ors[N-4],ops[N-6],mks[N-6]; static olt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(olt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;olt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int olt_init(void){if(init)return -1;st.n_oc=0;st.n_oe=0;st.n_or=0;st.n_op=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ocs[i].active=0;for(int i=0;i<N-2;i++)oes[i].active=0;for(int i=0;i<N-4;i++)ors[i].active=0;for(int i=0;i<N-6;i++)ops[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OLT] Oiltech initialized\n");return 0;}
int olt_crop(int t,int c,int a,int b,int d,int e,int y){return add(ocs,&st.n_oc,&st.t_f1,N,t,c,a,b,d,e,y);}
int olt_extract(int t,int c,int a,int b,int d,int e,int y){return add(oes,&st.n_oe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int olt_refine(int t,int c,int a,int b,int d,int e,int y){return add(ors,&st.n_or,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int olt_product(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int olt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void olt_report(void){ps("[OLT] Crop: ");pi(st.n_oc);ps(" ha=");pi(st.t_f1);ps("\nExtract: ");pi(st.n_oe);ps(" L=");pi(st.t_f2);ps("\nRefine: ");pi(st.n_or);ps(" L=");pi(st.t_f3);ps("\nProd: ");pi(st.n_op);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void olt_state(void){ps("[OLT] Oc=");pi(st.n_oc);ps(" Oe=");pi(st.n_oe);ps(" Or=");pi(st.n_or);ps(" Op=");pi(st.n_op);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oil Tech Admin Demo ===\n\n");olt_init();
ps("Oil crop planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;olt_crop(t,c,180+(i*17),165+(i*14),145+(i*10),127+(i*6),2020+(i%5));}
ps("\nOil extraction...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;olt_extract(t,c,169+(i*15),155+(i*12),137+(i*8),124+(i*5),2021+(i%4));}
ps("\nOil refining...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;olt_refine(t,c,161+(i*13),147+(i*10),131+(i*7),120+(i*4),2022+(i%3));}
ps("\nOil products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;olt_product(t,c,153+(i*11),141+(i*9),127+(i*6),117+(i*3),2023+(i%2));}
ps("\nOil marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;olt_market(t,c,147+(i*9),136+(i*7),123+(i*5),115+(i*3),2024);}
ps("\n");olt_report();olt_state();ps("\n=== Demo Complete ===\n");return 0;}
