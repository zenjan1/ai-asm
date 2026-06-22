/* zierias_admin: Zierias management technology administration (v1.0)
 * Zierias planning, zierias execution, zierias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zier_t;
typedef struct{int n_zierp,n_zier,n_zier2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zier_state_t;
static zier_t ziers[N],ziere[N-2],zierv[N-4],zierac[N-6],zierm[N-6]; static zier_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zier_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zier_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZIER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zier_init(void){if(init)return -1;st.n_zierp=0;st.n_zier=0;st.n_zier2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ziers[i].active=0;for(int i=0;i<N-2;i++)ziere[i].active=0;for(int i=0;i<N-4;i++)zierv[i].active=0;for(int i=0;i<N-6;i++)zierac[i].active=0;for(int i=0;i<N-6;i++)zierm[i].active=0;init=1;ps("[ZIER] Zierias initialized\n");return 0;}
int zier_planning(int t,int c,int a,int b,int d,int e,int y){return add(ziers,&st.n_zierp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zier_execution(int t,int c,int a,int b,int d,int e,int y){return add(ziere,&st.n_zier,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zier_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zierv,&st.n_zier2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zier_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zierac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zier_market(int t,int c,int a,int b,int d,int e,int y){return add(zierm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zier_report(void){ps("[ZIER] Zierp: ");pi(st.n_zierp);ps(" PCS=");pi(st.t_f1);ps("\nZiere: ");pi(st.n_zier);ps(" PCS=");pi(st.t_f2);ps("\nZierv: ");pi(st.n_zier2);ps(" PCS=");pi(st.t_f3);ps("\nZierac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zier_state(void){ps("[ZIER] Zierp=");pi(st.n_zierp);ps(" Ziere=");pi(st.n_zier);ps(" Zierv=");pi(st.n_zier2);ps(" Zierac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zierias Admin Demo ===\n\n");zier_init();
ps("Zierias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zier_planning(t,c,1379+(i*17),1368+(i*14),1348+(i*10),1330+(i*6),2020+(i%5));}
ps("\nZierias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zier_execution(t,c,1368+(i*15),1357+(i*12),1339+(i*8),1326+(i*5),2021+(i%4));}
ps("\nZierias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zier_evaluation(t,c,1360+(i*13),1349+(i*10),1333+(i*7),1322+(i*4),2022+(i%3));}
ps("\nZierias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zier_accessory(t,c,1352+(i*11),1343+(i*9),1329+(i*6),1319+(i*3),2023+(i%2));}
ps("\nZierias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zier_market(t,c,1346+(i*9),1337+(i*7),1324+(i*5),1316+(i*3),2024);}
ps("\n");zier_report();zier_state();ps("\n=== Demo Complete ===\n");return 0;}
