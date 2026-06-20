/* fisherytech_admin: Fishery technology administration (v1.0)
 * Aquaculture, marine fishing, fishery processing, fishery equipment, fishery resources
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fyt_t;
typedef struct{int n_ac,n_mf,n_fp,n_eq,n_rs,t_f1,t_f2,t_f3,t_f4,t_f5;} fyt_state_t;
static fyt_t acs[N],mfs[N-2],fps[N-4],eqs[N-6],rss[N-6]; static fyt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fyt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fyt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FYT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fyt_init(void){if(init)return -1;st.n_ac=0;st.n_mf=0;st.n_fp=0;st.n_eq=0;st.n_rs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acs[i].active=0;for(int i=0;i<N-2;i++)mfs[i].active=0;for(int i=0;i<N-4;i++)fps[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)rss[i].active=0;init=1;ps("[FYT] Fisherytech initialized\n");return 0;}
int fyt_aqua(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f1,N,t,c,a,b,d,e,y);}
int fyt_fishing(int t,int c,int a,int b,int d,int e,int y){return add(mfs,&st.n_mf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fyt_process(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fyt_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fyt_resource(int t,int c,int a,int b,int d,int e,int y){return add(rss,&st.n_rs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fyt_report(void){ps("[FYT] Aqua: ");pi(st.n_ac);ps(" Ton=");pi(st.t_f1);ps("\nFish: ");pi(st.n_mf);ps(" Catch=");pi(st.t_f2);ps("\nProc: ");pi(st.n_fp);ps(" Freeze=");pi(st.t_f3);ps("\nEquip: ");pi(st.n_eq);ps(" Vessel=");pi(st.t_f4);ps("\nRes: ");pi(st.n_rs);ps(" Stock=");pi(st.t_f5);ps("\n");}
void fyt_state(void){ps("[FYT] Ac=");pi(st.n_ac);ps(" Mf=");pi(st.n_mf);ps(" Fp=");pi(st.n_fp);ps(" Eq=");pi(st.n_eq);ps(" Rs=");pi(st.n_rs);ps("\n");}
int main(void){
ps("=== Fishery Tech Admin Demo ===\n\n");fyt_init();
ps("Aquaculture...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fyt_aqua(t,c,168+(i*17),153+(i*14),133+(i*10),116+(i*6),2020+(i%5));}
ps("\nMarine fishing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fyt_fishing(t,c,157+(i*15),143+(i*12),125+(i*8),112+(i*5),2021+(i%4));}
ps("\nFishery processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fyt_process(t,c,149+(i*13),135+(i*10),119+(i*7),108+(i*4),2022+(i%3));}
ps("\nFishery equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fyt_equipment(t,c,141+(i*11),129+(i*9),115+(i*6),105+(i*3),2023+(i%2));}
ps("\nFishery resources...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fyt_resource(t,c,135+(i*9),124+(i*7),111+(i*5),103+(i*3),2024);}
ps("\n");fyt_report();fyt_state();ps("\n=== Demo Complete ===\n");return 0;}
