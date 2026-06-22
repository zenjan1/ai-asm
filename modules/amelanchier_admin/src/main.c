/* amelanchier_admin: Amelanchier management technology administration (v1.0)
 * Amelanchier planning, amelanchier execution, amelanchier evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} amel_t;
typedef struct{int n_amelp,n_amel,n_amel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} amel_state_t;
static amel_t amels[N],amele[N-2],amel2[N-4],amelac[N-6],amelam[N-6]; static amel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(amel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;amel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int amel_init(void){if(init)return -1;st.n_amelp=0;st.n_amel=0;st.n_amel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)amels[i].active=0;for(int i=0;i<N-2;i++)amele[i].active=0;for(int i=0;i<N-4;i++)amel2[i].active=0;for(int i=0;i<N-6;i++)amelac[i].active=0;for(int i=0;i<N-6;i++)amelam[i].active=0;init=1;ps("[AMEL] Amelanchier initialized\n");return 0;}
int amel_planning(int t,int c,int a,int b,int d,int e,int y){return add(amels,&st.n_amelp,&st.t_f1,N,t,c,a,b,d,e,y);}
int amel_execution(int t,int c,int a,int b,int d,int e,int y){return add(amele,&st.n_amel,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int amel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(amel2,&st.n_amel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int amel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(amelac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int amel_market(int t,int c,int a,int b,int d,int e,int y){return add(amelam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void amel_report(void){ps("[AMEL] Melp: ");pi(st.n_amelp);ps(" PCS=");pi(st.t_f1);ps("\nMele: ");pi(st.n_amel);ps(" PCS=");pi(st.t_f2);ps("\nMel2: ");pi(st.n_amel2);ps(" PCS=");pi(st.t_f3);ps("\nMelac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void amel_state(void){ps("[AMEL] Melp=");pi(st.n_amelp);ps(" Mele=");pi(st.n_amel);ps(" Mel2=");pi(st.n_amel2);ps(" Melac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Amelanchier Admin Demo ===\n\n");amel_init();
ps("Amelanchier planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;amel_planning(t,c,1402+(i*17),1391+(i*14),1371+(i*10),1353+(i*6),2020+(i%5));}
ps("\nAmelanchier execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;amel_execution(t,c,1391+(i*15),1380+(i*12),1362+(i*8),1349+(i*5),2021+(i%4));}
ps("\nAmelanchier evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;amel_evaluation(t,c,1383+(i*13),1372+(i*10),1356+(i*7),1345+(i*4),2022+(i%3));}
ps("\nAmelanchier accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amel_accessory(t,c,1375+(i*11),1366+(i*9),1352+(i*6),1342+(i*3),2023+(i%2));}
ps("\nAmelanchier marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amel_market(t,c,1369+(i*9),1360+(i*7),1347+(i*5),1339+(i*3),2024);}
ps("\n");amel_report();amel_state();ps("\n=== Demo Complete ===\n");return 0;}
