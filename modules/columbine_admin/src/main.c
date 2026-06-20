/* columbine_admin: Columbine management technology administration (v1.0)
 * Columbine planning, columbine execution, columbine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} col_t;
typedef struct{int n_cmp,n_cme,n_cmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} col_state_t;
static col_t cmps[N],cmes[N-2],cmvs[N-4],acs[N-6],mks[N-6]; static col_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(col_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;col_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int col_init(void){if(init)return -1;st.n_cmp=0;st.n_cme=0;st.n_cmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cmps[i].active=0;for(int i=0;i<N-2;i++)cmes[i].active=0;for(int i=0;i<N-4;i++)cmvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[COL] Columbine initialized\n");return 0;}
int col_planning(int t,int c,int a,int b,int d,int e,int y){return add(cmps,&st.n_cmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int col_execution(int t,int c,int a,int b,int d,int e,int y){return add(cmes,&st.n_cme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int col_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cmvs,&st.n_cmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int col_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int col_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void col_report(void){ps("[COL] Cmp: ");pi(st.n_cmp);ps(" PCS=");pi(st.t_f1);ps("\nCme: ");pi(st.n_cme);ps(" PCS=");pi(st.t_f2);ps("\nCmv: ");pi(st.n_cmv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void col_state(void){ps("[COL] Cmp=");pi(st.n_cmp);ps(" Cme=");pi(st.n_cme);ps(" Cmv=");pi(st.n_cmv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Columbine Admin Demo ===\n\n");col_init();
ps("Columbine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;col_planning(t,c,581+(i*17),570+(i*14),550+(i*10),532+(i*6),2020+(i%5));}
ps("\nColumbine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;col_execution(t,c,570+(i*15),559+(i*12),541+(i*8),528+(i*5),2021+(i%4));}
ps("\nColumbine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;col_evaluation(t,c,562+(i*13),551+(i*10),535+(i*7),524+(i*4),2022+(i%3));}
ps("\nColumbine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;col_accessory(t,c,554+(i*11),545+(i*9),531+(i*6),521+(i*3),2023+(i%2));}
ps("\nColumbine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;col_market(t,c,548+(i*9),539+(i*7),526+(i*5),518+(i*3),2024);}
ps("\n");col_report();col_state();ps("\n=== Demo Complete ===\n");return 0;}
