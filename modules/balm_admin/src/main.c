/* balm_admin: Balm management technology administration (v1.0)
 * Balm planning, balm execution, balm evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bl_t;
typedef struct{int n_blp,n_ble,n_blv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bl_state_t;
static bl_t blps[N],blss[N-2],blvss[N-4],blas[N-6],blmks[N-6]; static bl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bl_init(void){if(init)return -1;st.n_blp=0;st.n_ble=0;st.n_blv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)blps[i].active=0;for(int i=0;i<N-2;i++)blss[i].active=0;for(int i=0;i<N-4;i++)blvss[i].active=0;for(int i=0;i<N-6;i++)blas[i].active=0;for(int i=0;i<N-6;i++)blmks[i].active=0;init=1;ps("[BLM] Balm initialized\n");return 0;}
int bl_planning(int t,int c,int a,int b,int d,int e,int y){return add(blps,&st.n_blp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bl_execution(int t,int c,int a,int b,int d,int e,int y){return add(blss,&st.n_ble,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(blvss,&st.n_blv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(blas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bl_market(int t,int c,int a,int b,int d,int e,int y){return add(blmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bl_report(void){ps("[BLM] Blp: ");pi(st.n_blp);ps(" PCS=");pi(st.t_f1);ps("\nBle: ");pi(st.n_ble);ps(" PCS=");pi(st.t_f2);ps("\nBlv: ");pi(st.n_blv);ps(" PCS=");pi(st.t_f3);ps("\nBlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bl_state(void){ps("[BLM] Blp=");pi(st.n_blp);ps(" Ble=");pi(st.n_ble);ps(" Blv=");pi(st.n_blv);ps(" Blc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Balm Admin Demo ===\n\n");bl_init();
ps("Balm planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bl_planning(t,c,663+(i*17),652+(i*14),632+(i*10),614+(i*6),2020+(i%5));}
ps("\nBalm execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bl_execution(t,c,652+(i*15),641+(i*12),623+(i*8),610+(i*5),2021+(i%4));}
ps("\nBalm evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bl_evaluation(t,c,644+(i*13),633+(i*10),617+(i*7),606+(i*4),2022+(i%3));}
ps("\nBalm accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bl_accessory(t,c,636+(i*11),627+(i*9),613+(i*6),603+(i*3),2023+(i%2));}
ps("\nBalm marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bl_market(t,c,630+(i*9),621+(i*7),608+(i*5),600+(i*3),2024);}
ps("\n");bl_report();bl_state();ps("\n=== Demo Complete ===\n");return 0;}
