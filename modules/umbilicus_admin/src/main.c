/* umbilicus_admin: Umbilicus management technology administration (v1.0)
 * Umbilicus planning, umbilicus execution, umbilicus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} umbi_t;
typedef struct{int n_umbip,n_umbie,n_umbi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} umbi_state_t;
static umbi_t umbis[N],umbiess[N-2],umbivss[N-4],umbias[N-6],umbimks[N-6]; static umbi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(umbi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;umbi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UMBI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int umbi_init(void){if(init)return -1;st.n_umbip=0;st.n_umbie=0;st.n_umbi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)umbis[i].active=0;for(int i=0;i<N-2;i++)umbiess[i].active=0;for(int i=0;i<N-4;i++)umbivss[i].active=0;for(int i=0;i<N-6;i++)umbias[i].active=0;for(int i=0;i<N-6;i++)umbimks[i].active=0;init=1;ps("[UMBI] Umbilicus initialized\n");return 0;}
int umbi_planning(int t,int c,int a,int b,int d,int e,int y){return add(umbis,&st.n_umbip,&st.t_f1,N,t,c,a,b,d,e,y);}
int umbi_execution(int t,int c,int a,int b,int d,int e,int y){return add(umbiess,&st.n_umbie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int umbi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(umbivss,&st.n_umbi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int umbi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(umbias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int umbi_market(int t,int c,int a,int b,int d,int e,int y){return add(umbimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void umbi_report(void){ps("[UMBI] Umbip: ");pi(st.n_umbip);ps(" PCS=");pi(st.t_f1);ps("\nUmbie: ");pi(st.n_umbie);ps(" PCS=");pi(st.t_f2);ps("\nUmbi2: ");pi(st.n_umbi2);ps(" PCS=");pi(st.t_f3);ps("\nUmbic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void umbi_state(void){ps("[UMBI] Umbip=");pi(st.n_umbip);ps(" Umbie=");pi(st.n_umbie);ps(" Umbi2=");pi(st.n_umbi2);ps(" Umbic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Umbilicus Admin Demo ===\n\n");umbi_init();
ps("Umbilicus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;umbi_planning(t,c,1352+(i*17),1341+(i*14),1321+(i*10),1303+(i*6),2020+(i%5));}
ps("\nUmbilicus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;umbi_execution(t,c,1341+(i*15),1330+(i*12),1312+(i*8),1299+(i*5),2021+(i%4));}
ps("\nUmbilicus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;umbi_evaluation(t,c,1333+(i*13),1322+(i*10),1306+(i*7),1295+(i*4),2022+(i%3));}
ps("\nUmbilicus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;umbi_accessory(t,c,1325+(i*11),1316+(i*9),1302+(i*6),1292+(i*3),2023+(i%2));}
ps("\nUmbilicus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;umbi_market(t,c,1319+(i*9),1310+(i*7),1297+(i*5),1289+(i*3),2024);}
ps("\n");umbi_report();umbi_state();ps("\n=== Demo Complete ===\n");return 0;}
