/* yucca_admin: Yucca management technology administration (v1.0)
 * Yucca planning, yucca execution, yucca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} yuc_t;
typedef struct{int n_yucp,n_yuce,n_yuc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} yuc_state_t;
static yuc_t yucps[N],yucss[N-2],yucvss[N-4],yucas[N-6],yucmks[N-6]; static yuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yuc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;yuc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[YUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int yuc_init(void){if(init)return -1;st.n_yucp=0;st.n_yuce=0;st.n_yuc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)yucps[i].active=0;for(int i=0;i<N-2;i++)yucss[i].active=0;for(int i=0;i<N-4;i++)yucvss[i].active=0;for(int i=0;i<N-6;i++)yucas[i].active=0;for(int i=0;i<N-6;i++)yucmks[i].active=0;init=1;ps("[YUC] Yucca initialized\n");return 0;}
int yuc_planning(int t,int c,int a,int b,int d,int e,int y){return add(yucps,&st.n_yucp,&st.t_f1,N,t,c,a,b,d,e,y);}
int yuc_execution(int t,int c,int a,int b,int d,int e,int y){return add(yucss,&st.n_yuce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int yuc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(yucvss,&st.n_yuc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int yuc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(yucas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int yuc_market(int t,int c,int a,int b,int d,int e,int y){return add(yucmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void yuc_report(void){ps("[YUC] Yucp: ");pi(st.n_yucp);ps(" PCS=");pi(st.t_f1);ps("\nYuce: ");pi(st.n_yuce);ps(" PCS=");pi(st.t_f2);ps("\nYucv: ");pi(st.n_yuc2);ps(" PCS=");pi(st.t_f3);ps("\nYucc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void yuc_state(void){ps("[YUC] Yucp=");pi(st.n_yucp);ps(" Yuce=");pi(st.n_yuce);ps(" Yucv=");pi(st.n_yuc2);ps(" Yucc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Yucca Admin Demo ===\n\n");yuc_init();
ps("Yucca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;yuc_planning(t,c,962+(i*17),951+(i*14),931+(i*10),913+(i*6),2020+(i%5));}
ps("\nYucca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;yuc_execution(t,c,951+(i*15),940+(i*12),922+(i*8),909+(i*5),2021+(i%4));}
ps("\nYucca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;yuc_evaluation(t,c,943+(i*13),932+(i*10),916+(i*7),905+(i*4),2022+(i%3));}
ps("\nYucca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yuc_accessory(t,c,935+(i*11),926+(i*9),912+(i*6),902+(i*3),2023+(i%2));}
ps("\nYucca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;yuc_market(t,c,929+(i*9),920+(i*7),907+(i*5),899+(i*3),2024);}
ps("\n");yuc_report();yuc_state();ps("\n=== Demo Complete ===\n");return 0;}
