/* saw_admin: Sawing machine technology administration (v1.0)
 * Circular saw, band saw, bow saw, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} saw_t;
typedef struct{int n_cr,n_bd,n_bw,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} saw_state_t;
static saw_t crs[N],bds[N-2],bws[N-4],acs[N-6],mks[N-6]; static saw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(saw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;saw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int saw_init(void){if(init)return -1;st.n_cr=0;st.n_bd=0;st.n_bw=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crs[i].active=0;for(int i=0;i<N-2;i++)bds[i].active=0;for(int i=0;i<N-4;i++)bws[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SAW] Saw initialized\n");return 0;}
int saw_circular(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f1,N,t,c,a,b,d,e,y);}
int saw_band(int t,int c,int a,int b,int d,int e,int y){return add(bds,&st.n_bd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int saw_bow(int t,int c,int a,int b,int d,int e,int y){return add(bws,&st.n_bw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int saw_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int saw_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void saw_report(void){ps("[SAW] Cr: ");pi(st.n_cr);ps(" PCS=");pi(st.t_f1);ps("\nBd: ");pi(st.n_bd);ps(" PCS=");pi(st.t_f2);ps("\nBw: ");pi(st.n_bw);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void saw_state(void){ps("[SAW] Cr=");pi(st.n_cr);ps(" Bd=");pi(st.n_bd);ps(" Bw=");pi(st.n_bw);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Saw Admin Demo ===\n\n");saw_init();
ps("Circular saws...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;saw_circular(t,c,400+(i*17),385+(i*14),365+(i*10),347+(i*6),2020+(i%5));}
ps("\nBand saws...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;saw_band(t,c,389+(i*15),375+(i*12),357+(i*8),344+(i*5),2021+(i%4));}
ps("\nBow saws...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;saw_bow(t,c,381+(i*13),367+(i*10),351+(i*7),340+(i*4),2022+(i%3));}
ps("\nSaw accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;saw_accessory(t,c,373+(i*11),361+(i*9),347+(i*6),337+(i*3),2023+(i%2));}
ps("\nSaw marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;saw_market(t,c,367+(i*9),356+(i*7),343+(i*5),335+(i*3),2024);}
ps("\n");saw_report();saw_state();ps("\n=== Demo Complete ===\n");return 0;}
