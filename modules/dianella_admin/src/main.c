/* dianella_admin: Dianella management technology administration (v1.0)
 * Dianella planning, dianella execution, dianella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dla_t;
typedef struct{int n_dlap,n_dlae,n_dla2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dla_state_t;
static dla_t dlaps[N],dlass[N-2],dlavss[N-4],dlaas[N-6],dlamks[N-6]; static dla_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dla_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dla_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DLA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dla_init(void){if(init)return -1;st.n_dlap=0;st.n_dlae=0;st.n_dla2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dlaps[i].active=0;for(int i=0;i<N-2;i++)dlass[i].active=0;for(int i=0;i<N-4;i++)dlavss[i].active=0;for(int i=0;i<N-6;i++)dlaas[i].active=0;for(int i=0;i<N-6;i++)dlamks[i].active=0;init=1;ps("[DLA] Dianella initialized\n");return 0;}
int dla_planning(int t,int c,int a,int b,int d,int e,int y){return add(dlaps,&st.n_dlap,&st.t_f1,N,t,c,a,b,d,e,y);}
int dla_execution(int t,int c,int a,int b,int d,int e,int y){return add(dlass,&st.n_dlae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dla_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dlavss,&st.n_dla2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dla_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dlaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dla_market(int t,int c,int a,int b,int d,int e,int y){return add(dlamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dla_report(void){ps("[DLA] Diap: ");pi(st.n_dlap);ps(" PCS=");pi(st.t_f1);ps("\nDiae: ");pi(st.n_dlae);ps(" PCS=");pi(st.t_f2);ps("\nDiav: ");pi(st.n_dla2);ps(" PCS=");pi(st.t_f3);ps("\nDiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dla_state(void){ps("[DLA] Diap=");pi(st.n_dlap);ps(" Diae=");pi(st.n_dlae);ps(" Diav=");pi(st.n_dla2);ps(" Diac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dianella Admin Demo ===\n\n");dla_init();
ps("Dianella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dla_planning(t,c,1262+(i*17),1251+(i*14),1231+(i*10),1213+(i*6),2020+(i%5));}
ps("\nDianella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dla_execution(t,c,1251+(i*15),1240+(i*12),1222+(i*8),1209+(i*5),2021+(i%4));}
ps("\nDianella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dla_evaluation(t,c,1243+(i*13),1232+(i*10),1216+(i*7),1205+(i*4),2022+(i%3));}
ps("\nDianella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dla_accessory(t,c,1235+(i*11),1226+(i*9),1212+(i*6),1202+(i*3),2023+(i%2));}
ps("\nDianella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dla_market(t,c,1229+(i*9),1220+(i*7),1207+(i*5),1199+(i*3),2024);}
ps("\n");dla_report();dla_state();ps("\n=== Demo Complete ===\n");return 0;}
