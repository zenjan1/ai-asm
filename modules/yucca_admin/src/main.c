/* yucca_admin: Yucca management technology administration (v1.0)
 * Yucca planning, yucca execution, yucca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} yc_t;
typedef struct{int n_ycp,n_yce,n_ycv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} yc_state_t;
static yc_t ycps[N],ycss[N-2],ycvs[N-4],ycas[N-6],ycmks[N-6]; static yc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;yc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[YUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int yc_init(void){if(init)return -1;st.n_ycp=0;st.n_yce=0;st.n_ycv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ycps[i].active=0;for(int i=0;i<N-2;i++)ycss[i].active=0;for(int i=0;i<N-4;i++)ycvs[i].active=0;for(int i=0;i<N-6;i++)ycas[i].active=0;for(int i=0;i<N-6;i++)ycmks[i].active=0;init=1;ps("[YUC] Yucca initialized\n");return 0;}
int yc_planning(int t,int c,int a,int b,int d,int e,int y){return add(ycps,&st.n_ycp,&st.t_f1,N,t,c,a,b,d,e,y);}
int yc_execution(int t,int c,int a,int b,int d,int e,int y){return add(ycss,&st.n_yce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int yc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ycvs,&st.n_ycv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int yc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ycas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int yc_market(int t,int c,int a,int b,int d,int e,int y){return add(ycmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void yc_report(void){ps("[YUC] Ycp: ");pi(st.n_ycp);ps(" PCS=");pi(st.t_f1);ps("\nYce: ");pi(st.n_yce);ps(" PCS=");pi(st.t_f2);ps("\nYcv: ");pi(st.n_ycv);ps(" PCS=");pi(st.t_f3);ps("\nYcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void yc_state(void){ps("[YUC] Ycp=");pi(st.n_ycp);ps(" Yce=");pi(st.n_yce);ps(" Ycv=");pi(st.n_ycv);ps(" Ycc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Yucca Admin Demo ===\n\n");yc_init();
ps("Yucca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;yc_planning(t,c,641+(i*17),630+(i*14),610+(i*10),592+(i*6),2020+(i%5));}
ps("\nYucca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;yc_execution(t,c,630+(i*15),619+(i*12),601+(i*8),588+(i*5),2021+(i%4));}
ps("\nYucca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;yc_evaluation(t,c,622+(i*13),611+(i*10),595+(i*7),584+(i*4),2022+(i%3));}
ps("\nYucca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yc_accessory(t,c,614+(i*11),605+(i*9),591+(i*6),581+(i*3),2023+(i%2));}
ps("\nYucca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yc_market(t,c,608+(i*9),599+(i*7),586+(i*5),578+(i*3),2024);}
ps("\n");yc_report();yc_state();ps("\n=== Demo Complete ===\n");return 0;}
