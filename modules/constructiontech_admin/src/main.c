/* constructiontech_admin: Construction technology administration (v1.0)
 * BIM design, smart construction, green building, building ops, prefab construction
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cst_t;
typedef struct{int n_bm,n_sc,n_gb,n_op,n_pf,t_f1,t_f2,t_f3,t_f4,t_f5;} cst_state_t;
static cst_t bms[N],scs[N-2],gbs[N-4],ops[N-6],pfs[N-6]; static cst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cst_init(void){if(init)return -1;st.n_bm=0;st.n_sc=0;st.n_gb=0;st.n_op=0;st.n_pf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bms[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)gbs[i].active=0;for(int i=0;i<N-6;i++)ops[i].active=0;for(int i=0;i<N-6;i++)pfs[i].active=0;init=1;ps("[CST] Constructiontech initialized\n");return 0;}
int cst_bim(int t,int c,int a,int b,int d,int e,int y){return add(bms,&st.n_bm,&st.t_f1,N,t,c,a,b,d,e,y);}
int cst_smart(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cst_green(int t,int c,int a,int b,int d,int e,int y){return add(gbs,&st.n_gb,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cst_ops(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cst_prefab(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cst_report(void){ps("[CST] BIM: ");pi(st.n_bm);ps(" 3D=");pi(st.t_f1);ps("\nSmart: ");pi(st.n_sc);ps(" IoT=");pi(st.t_f2);ps("\nGreen: ");pi(st.n_gb);ps(" LEED=");pi(st.t_f3);ps("\nOps: ");pi(st.n_op);ps(" FM=");pi(st.t_f4);ps("\nPrefab: ");pi(st.n_pf);ps(" MIC=");pi(st.t_f5);ps("\n");}
void cst_state(void){ps("[CST] Bm=");pi(st.n_bm);ps(" Sc=");pi(st.n_sc);ps(" Gb=");pi(st.n_gb);ps(" Op=");pi(st.n_op);ps(" Pf=");pi(st.n_pf);ps("\n");}
int main(void){
ps("=== Construction Tech Admin Demo ===\n\n");cst_init();
ps("BIM design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cst_bim(t,c,152+(i*17),137+(i*14),117+(i*10),100+(i*6),2020+(i%5));}
ps("\nSmart construction...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cst_smart(t,c,141+(i*15),127+(i*12),109+(i*8),96+(i*5),2021+(i%4));}
ps("\nGreen building...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cst_green(t,c,133+(i*13),119+(i*10),103+(i*7),92+(i*4),2022+(i%3));}
ps("\nBuilding ops...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_ops(t,c,125+(i*11),113+(i*9),99+(i*6),89+(i*3),2023+(i%2));}
ps("\nPrefab construction...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_prefab(t,c,119+(i*9),108+(i*7),95+(i*5),87+(i*3),2024);}
ps("\n");cst_report();cst_state();ps("\n=== Demo Complete ===\n");return 0;}
