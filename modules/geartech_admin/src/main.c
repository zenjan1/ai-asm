/* geartech_admin: Gear technology administration (v1.0)
 * Cylindrical gear, bevel gear, worm gear, gearbox, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} grt_t;
typedef struct{int n_cg,n_bg,n_wg,n_gb,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} grt_state_t;
static grt_t cgs[N],bgs[N-2],wgs[N-4],gbs[N-6],mks[N-6]; static grt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;grt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int grt_init(void){if(init)return -1;st.n_cg=0;st.n_bg=0;st.n_wg=0;st.n_gb=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cgs[i].active=0;for(int i=0;i<N-2;i++)bgs[i].active=0;for(int i=0;i<N-4;i++)wgs[i].active=0;for(int i=0;i<N-6;i++)gbs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GRT] Geartech initialized\n");return 0;}
int grt_cylindrical(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f1,N,t,c,a,b,d,e,y);}
int grt_bevel(int t,int c,int a,int b,int d,int e,int y){return add(bgs,&st.n_bg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int grt_worm(int t,int c,int a,int b,int d,int e,int y){return add(wgs,&st.n_wg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int grt_box(int t,int c,int a,int b,int d,int e,int y){return add(gbs,&st.n_gb,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int grt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void grt_report(void){ps("[GRT] Cg: ");pi(st.n_cg);ps(" PCS=");pi(st.t_f1);ps("\nBg: ");pi(st.n_bg);ps(" PCS=");pi(st.t_f2);ps("\nWg: ");pi(st.n_wg);ps(" PCS=");pi(st.t_f3);ps("\nGb: ");pi(st.n_gb);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void grt_state(void){ps("[GRT] Cg=");pi(st.n_cg);ps(" Bg=");pi(st.n_bg);ps(" Wg=");pi(st.n_wg);ps(" Gb=");pi(st.n_gb);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gear Tech Admin Demo ===\n\n");grt_init();
ps("Cylindrical gear...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;grt_cylindrical(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nBevel gear...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;grt_bevel(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nWorm gear...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;grt_worm(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nGearbox...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grt_box(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nGear marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grt_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");grt_report();grt_state();ps("\n=== Demo Complete ===\n");return 0;}
