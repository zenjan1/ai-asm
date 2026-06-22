/* cunila_admin: Cunila management technology administration (v1.0)
 * Cunila planning, cunila execution, cunila evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nil_t;
typedef struct{int n_nilp,n_nile,n_nil2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nil_state_t;
static nil_t nilps[N],nilss[N-2],nilvss[N-4],nilas[N-6],nilmks[N-6]; static nil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nil_init(void){if(init)return -1;st.n_nilp=0;st.n_nile=0;st.n_nil2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nilps[i].active=0;for(int i=0;i<N-2;i++)nilss[i].active=0;for(int i=0;i<N-4;i++)nilvss[i].active=0;for(int i=0;i<N-6;i++)nilas[i].active=0;for(int i=0;i<N-6;i++)nilmks[i].active=0;init=1;ps("[NIL] Cunila initialized\n");return 0;}
int nil_planning(int t,int c,int a,int b,int d,int e,int y){return add(nilps,&st.n_nilp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nil_execution(int t,int c,int a,int b,int d,int e,int y){return add(nilss,&st.n_nile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nilvss,&st.n_nil2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nilas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nil_market(int t,int c,int a,int b,int d,int e,int y){return add(nilmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nil_report(void){ps("[NIL] Cunpp: ");pi(st.n_nilp);ps(" PCS=");pi(st.t_f1);ps("\nCunpe: ");pi(st.n_nile);ps(" PCS=");pi(st.t_f2);ps("\nCunv: ");pi(st.n_nil2);ps(" PCS=");pi(st.t_f3);ps("\nCunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nil_state(void){ps("[NIL] Cunpp=");pi(st.n_nilp);ps(" Cunpe=");pi(st.n_nile);ps(" Cunv=");pi(st.n_nil2);ps(" Cunc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cunila Admin Demo ===\n\n");nil_init();
ps("Cunila planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nil_planning(t,c,1249+(i*17),1238+(i*14),1218+(i*10),1200+(i*6),2020+(i%5));}
ps("\nCunila execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nil_execution(t,c,1238+(i*15),1227+(i*12),1209+(i*8),1196+(i*5),2021+(i%4));}
ps("\nCunila evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nil_evaluation(t,c,1230+(i*13),1219+(i*10),1203+(i*7),1192+(i*4),2022+(i%3));}
ps("\nCunila accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nil_accessory(t,c,1222+(i*11),1213+(i*9),1199+(i*6),1189+(i*3),2023+(i%2));}
ps("\nCunila marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nil_market(t,c,1216+(i*9),1207+(i*7),1194+(i*5),1186+(i*3),2024);}
ps("\n");nil_report();nil_state();ps("\n=== Demo Complete ===\n");return 0;}
