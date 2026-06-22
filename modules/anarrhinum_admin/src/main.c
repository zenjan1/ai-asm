/* anarrhinum_admin: Anarrhinum management technology administration (v1.0)
 * Anarrhinum planning, anarrhinum execution, anarrhinum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anar_t;
typedef struct{int n_anarp,n_anar,n_anar2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anar_state_t;
static anar_t anars[N],anare[N-2],anar2[N-4],anarac[N-6],anaram[N-6]; static anar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(anar_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anar_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anar_init(void){if(init)return -1;st.n_anarp=0;st.n_anar=0;st.n_anar2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anars[i].active=0;for(int i=0;i<N-2;i++)anare[i].active=0;for(int i=0;i<N-4;i++)anar2[i].active=0;for(int i=0;i<N-6;i++)anarac[i].active=0;for(int i=0;i<N-6;i++)anaram[i].active=0;init=1;ps("[ANAR] Anarrhinum initialized\n");return 0;}
int anar_planning(int t,int c,int a,int b,int d,int e,int y){return add(anars,&st.n_anarp,&st.t_f1,N,t,c,a,b,d,e,y);}
int anar_execution(int t,int c,int a,int b,int d,int e,int y){return add(anare,&st.n_anar,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anar_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anar2,&st.n_anar2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anar_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anarac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anar_market(int t,int c,int a,int b,int d,int e,int y){return add(anaram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anar_report(void){ps("[ANAR] Narp: ");pi(st.n_anarp);ps(" PCS=");pi(st.t_f1);ps("\nNare: ");pi(st.n_anar);ps(" PCS=");pi(st.t_f2);ps("\nNar2: ");pi(st.n_anar2);ps(" PCS=");pi(st.t_f3);ps("\nNarac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anar_state(void){ps("[ANAR] Narp=");pi(st.n_anarp);ps(" Nare=");pi(st.n_anar);ps(" Nar2=");pi(st.n_anar2);ps(" Narac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anarrhinum Admin Demo ===\n\n");anar_init();
ps("Anarrhinum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anar_planning(t,c,1407+(i*17),1396+(i*14),1376+(i*10),1358+(i*6),2020+(i%5));}
ps("\nAnarrhinum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anar_execution(t,c,1396+(i*15),1385+(i*12),1367+(i*8),1354+(i*5),2021+(i%4));}
ps("\nAnarrhinum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anar_evaluation(t,c,1388+(i*13),1377+(i*10),1361+(i*7),1350+(i*4),2022+(i%3));}
ps("\nAnarrhinum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anar_accessory(t,c,1380+(i*11),1371+(i*9),1357+(i*6),1347+(i*3),2023+(i%2));}
ps("\nAnarrhinum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anar_market(t,c,1374+(i*9),1365+(i*7),1352+(i*5),1344+(i*3),2024);}
ps("\n");anar_report();anar_state();ps("\n=== Demo Complete ===\n");return 0;}
