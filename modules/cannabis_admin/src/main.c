/* cannabis_admin: Cannabis management technology administration (v1.0)
 * Cannabis planning, cannabis execution, cannabis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nab_t;
typedef struct{int n_nabp,n_nabe,n_nab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nab_state_t;
static nab_t nabps[N],nabss[N-2],nabvss[N-4],nabas[N-6],nabmks[N-6]; static nab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nab_init(void){if(init)return -1;st.n_nabp=0;st.n_nabe=0;st.n_nab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nabps[i].active=0;for(int i=0;i<N-2;i++)nabss[i].active=0;for(int i=0;i<N-4;i++)nabvss[i].active=0;for(int i=0;i<N-6;i++)nabas[i].active=0;for(int i=0;i<N-6;i++)nabmks[i].active=0;init=1;ps("[NAB] Cannabis initialized\n");return 0;}
int nab_planning(int t,int c,int a,int b,int d,int e,int y){return add(nabps,&st.n_nabp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nab_execution(int t,int c,int a,int b,int d,int e,int y){return add(nabss,&st.n_nabe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nabvss,&st.n_nab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nabas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nab_market(int t,int c,int a,int b,int d,int e,int y){return add(nabmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nab_report(void){ps("[NAB] Canpp: ");pi(st.n_nabp);ps(" PCS=");pi(st.t_f1);ps("\nCanpe: ");pi(st.n_nabe);ps(" PCS=");pi(st.t_f2);ps("\nCanv: ");pi(st.n_nab2);ps(" PCS=");pi(st.t_f3);ps("\nCanc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nab_state(void){ps("[NAB] Canpp=");pi(st.n_nabp);ps(" Canpe=");pi(st.n_nabe);ps(" Canv=");pi(st.n_nab2);ps(" Canc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cannabis Admin Demo ===\n\n");nab_init();
ps("Cannabis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nab_planning(t,c,1230+(i*17),1219+(i*14),1199+(i*10),1181+(i*6),2020+(i%5));}
ps("\nCannabis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nab_execution(t,c,1219+(i*15),1208+(i*12),1190+(i*8),1177+(i*5),2021+(i%4));}
ps("\nCannabis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nab_evaluation(t,c,1211+(i*13),1200+(i*10),1184+(i*7),1173+(i*4),2022+(i%3));}
ps("\nCannabis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nab_accessory(t,c,1203+(i*11),1194+(i*9),1180+(i*6),1170+(i*3),2023+(i%2));}
ps("\nCannabis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nab_market(t,c,1197+(i*9),1188+(i*7),1175+(i*5),1167+(i*3),2024);}
ps("\n");nab_report();nab_state();ps("\n=== Demo Complete ===\n");return 0;}
