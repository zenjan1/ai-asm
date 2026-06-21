/* yellowwood_admin: Yellowwood management technology administration (v1.0)
 * Yellowwood planning, yellowwood execution, yellowwood evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} yel_t;
typedef struct{int n_yelp,n_yele,n_yel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} yel_state_t;
static yel_t yelps[N],yelss[N-2],yelvss[N-4],yelas[N-6],yelmks[N-6]; static yel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;yel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[YEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int yel_init(void){if(init)return -1;st.n_yelp=0;st.n_yele=0;st.n_yel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)yelps[i].active=0;for(int i=0;i<N-2;i++)yelss[i].active=0;for(int i=0;i<N-4;i++)yelvss[i].active=0;for(int i=0;i<N-6;i++)yelas[i].active=0;for(int i=0;i<N-6;i++)yelmks[i].active=0;init=1;ps("[YEL] Yellowwood initialized\n");return 0;}
int yel_planning(int t,int c,int a,int b,int d,int e,int y){return add(yelps,&st.n_yelp,&st.t_f1,N,t,c,a,b,d,e,y);}
int yel_execution(int t,int c,int a,int b,int d,int e,int y){return add(yelss,&st.n_yele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int yel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(yelvss,&st.n_yel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int yel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(yelas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int yel_market(int t,int c,int a,int b,int d,int e,int y){return add(yelmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void yel_report(void){ps("[YEL] Yelps: ");pi(st.n_yelp);ps(" PCS=");pi(st.t_f1);ps("\nYele: ");pi(st.n_yele);ps(" PCS=");pi(st.t_f2);ps("\nYelv: ");pi(st.n_yel2);ps(" PCS=");pi(st.t_f3);ps("\nYelc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void yel_state(void){ps("[YEL] Yelps=");pi(st.n_yelp);ps(" Yele=");pi(st.n_yele);ps(" Yelv=");pi(st.n_yel2);ps(" Yelc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Yellowwood Admin Demo ===\n\n");yel_init();
ps("Yellowwood planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;yel_planning(t,c,959+(i*17),948+(i*14),928+(i*10),910+(i*6),2020+(i%5));}
ps("\nYellowwood execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;yel_execution(t,c,948+(i*15),937+(i*12),919+(i*8),906+(i*5),2021+(i%4));}
ps("\nYellowwood evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;yel_evaluation(t,c,940+(i*13),929+(i*10),913+(i*7),902+(i*4),2022+(i%3));}
ps("\nYellowwood accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yel_accessory(t,c,932+(i*11),923+(i*9),909+(i*6),899+(i*3),2023+(i%2));}
ps("\nYellowwood marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yel_market(t,c,926+(i*9),917+(i*7),904+(i*5),896+(i*3),2024);}
ps("\n");yel_report();yel_state();ps("\n=== Demo Complete ===\n");return 0;}
