/* virgilia_admin: Virgilia management technology administration (v1.0)
 * Virgilia planning, virgilia execution, virgilia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} virg_t;
typedef struct{int n_virgp,n_virg,n_virg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} virg_state_t;
static virg_t virgs[N],virgess[N-2],virgvss[N-4],virgas[N-6],virgmks[N-6]; static virg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(virg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;virg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VIRG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int virg_init(void){if(init)return -1;st.n_virgp=0;st.n_virg=0;st.n_virg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)virgs[i].active=0;for(int i=0;i<N-2;i++)virgess[i].active=0;for(int i=0;i<N-4;i++)virgvss[i].active=0;for(int i=0;i<N-6;i++)virgas[i].active=0;for(int i=0;i<N-6;i++)virgmks[i].active=0;init=1;ps("[VIRG] Virgilia initialized\n");return 0;}
int virg_planning(int t,int c,int a,int b,int d,int e,int y){return add(virgs,&st.n_virgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int virg_execution(int t,int c,int a,int b,int d,int e,int y){return add(virgess,&st.n_virg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int virg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(virgvss,&st.n_virg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int virg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(virgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int virg_market(int t,int c,int a,int b,int d,int e,int y){return add(virgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void virg_report(void){ps("[VIRG] Virgp: ");pi(st.n_virgp);ps(" PCS=");pi(st.t_f1);ps("\nVirge: ");pi(st.n_virg);ps(" PCS=");pi(st.t_f2);ps("\nVirgv: ");pi(st.n_virg2);ps(" PCS=");pi(st.t_f3);ps("\nVirgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void virg_state(void){ps("[VIRG] Virgp=");pi(st.n_virgp);ps(" Virge=");pi(st.n_virg);ps(" Virgv=");pi(st.n_virg2);ps(" Virgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Virgilia Admin Demo ===\n\n");virg_init();
ps("Virgilia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;virg_planning(t,c,1363+(i*17),1352+(i*14),1332+(i*10),1314+(i*6),2020+(i%5));}
ps("\nVirgilia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;virg_execution(t,c,1352+(i*15),1341+(i*12),1323+(i*8),1310+(i*5),2021+(i%4));}
ps("\nVirgilia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;virg_evaluation(t,c,1344+(i*13),1333+(i*10),1317+(i*7),1306+(i*4),2022+(i%3));}
ps("\nVirgilia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;virg_accessory(t,c,1336+(i*11),1327+(i*9),1313+(i*6),1303+(i*3),2023+(i%2));}
ps("\nVirgilia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;virg_market(t,c,1330+(i*9),1321+(i*7),1308+(i*5),1300+(i*3),2024);}
ps("\n");virg_report();virg_state();ps("\n=== Demo Complete ===\n");return 0;}
