/* volkowia_admin: Volkowia management technology administration (v1.0)
 * Volkowia planning, volkowia execution, volkowia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} volk_t;
typedef struct{int n_volkp,n_volk,n_volk2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} volk_state_t;
static volk_t volks[N],volke[N-2],volkv[N-4],volac[N-6],volkm[N-6]; static volk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(volk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;volk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VOLK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int volk_init(void){if(init)return -1;st.n_volkp=0;st.n_volk=0;st.n_volk2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)volks[i].active=0;for(int i=0;i<N-2;i++)volke[i].active=0;for(int i=0;i<N-4;i++)volkv[i].active=0;for(int i=0;i<N-6;i++)volac[i].active=0;for(int i=0;i<N-6;i++)volkm[i].active=0;init=1;ps("[VOLK] Volkowia initialized\n");return 0;}
int volk_planning(int t,int c,int a,int b,int d,int e,int y){return add(volks,&st.n_volkp,&st.t_f1,N,t,c,a,b,d,e,y);}
int volk_execution(int t,int c,int a,int b,int d,int e,int y){return add(volke,&st.n_volk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int volk_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(volkv,&st.n_volk2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int volk_accessory(int t,int c,int a,int b,int d,int e,int y){return add(volac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int volk_market(int t,int c,int a,int b,int d,int e,int y){return add(volkm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void volk_report(void){ps("[VOLK] Volkp: ");pi(st.n_volkp);ps(" PCS=");pi(st.t_f1);ps("\nVolke: ");pi(st.n_volk);ps(" PCS=");pi(st.t_f2);ps("\nVolkv: ");pi(st.n_volk2);ps(" PCS=");pi(st.t_f3);ps("\nVolac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void volk_state(void){ps("[VOLK] Volkp=");pi(st.n_volkp);ps(" Volke=");pi(st.n_volk);ps(" Volkv=");pi(st.n_volk2);ps(" Volac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Volkowia Admin Demo ===\n\n");volk_init();
ps("Volkowia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;volk_planning(t,c,1371+(i*17),1360+(i*14),1340+(i*10),1322+(i*6),2020+(i%5));}
ps("\nVolkowia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;volk_execution(t,c,1360+(i*15),1349+(i*12),1331+(i*8),1318+(i*5),2021+(i%4));}
ps("\nVolkowia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;volk_evaluation(t,c,1352+(i*13),1341+(i*10),1325+(i*7),1314+(i*4),2022+(i%3));}
ps("\nVolkowia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;volk_accessory(t,c,1344+(i*11),1335+(i*9),1321+(i*6),1311+(i*3),2023+(i%2));}
ps("\nVolkowia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;volk_market(t,c,1338+(i*9),1329+(i*7),1316+(i*5),1308+(i*3),2024);}
ps("\n");volk_report();volk_state();ps("\n=== Demo Complete ===\n");return 0;}
