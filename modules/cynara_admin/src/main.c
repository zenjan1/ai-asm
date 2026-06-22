/* cynara_admin: Cynara management technology administration (v1.0)
 * Cynara planning, cynara execution, cynara evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nar_t;
typedef struct{int n_narp,n_nare,n_nar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nar_state_t;
static nar_t narps[N],narss[N-2],narvss[N-4],naras[N-6],narmks[N-6]; static nar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nar_init(void){if(init)return -1;st.n_narp=0;st.n_nare=0;st.n_nar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)narps[i].active=0;for(int i=0;i<N-2;i++)narss[i].active=0;for(int i=0;i<N-4;i++)narvss[i].active=0;for(int i=0;i<N-6;i++)naras[i].active=0;for(int i=0;i<N-6;i++)narmks[i].active=0;init=1;ps("[NAR] Cynara initialized\n");return 0;}
int nar_planning(int t,int c,int a,int b,int d,int e,int y){return add(narps,&st.n_narp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nar_execution(int t,int c,int a,int b,int d,int e,int y){return add(narss,&st.n_nare,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(narvss,&st.n_nar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(naras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nar_market(int t,int c,int a,int b,int d,int e,int y){return add(narmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nar_report(void){ps("[NAR] Cynpp: ");pi(st.n_narp);ps(" PCS=");pi(st.t_f1);ps("\nCynpe: ");pi(st.n_nare);ps(" PCS=");pi(st.t_f2);ps("\nCynv: ");pi(st.n_nar2);ps(" PCS=");pi(st.t_f3);ps("\nCync: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nar_state(void){ps("[NAR] Cynpp=");pi(st.n_narp);ps(" Cynpe=");pi(st.n_nare);ps(" Cynv=");pi(st.n_nar2);ps(" Cync=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cynara Admin Demo ===\n\n");nar_init();
ps("Cynara planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nar_planning(t,c,1251+(i*17),1240+(i*14),1220+(i*10),1202+(i*6),2020+(i%5));}
ps("\nCynara execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nar_execution(t,c,1240+(i*15),1229+(i*12),1211+(i*8),1198+(i*5),2021+(i%4));}
ps("\nCynara evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nar_evaluation(t,c,1232+(i*13),1221+(i*10),1205+(i*7),1194+(i*4),2022+(i%3));}
ps("\nCynara accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nar_accessory(t,c,1224+(i*11),1215+(i*9),1201+(i*6),1191+(i*3),2023+(i%2));}
ps("\nCynara marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nar_market(t,c,1218+(i*9),1209+(i*7),1196+(i*5),1188+(i*3),2024);}
ps("\n");nar_report();nar_state();ps("\n=== Demo Complete ===\n");return 0;}
