/* glasstech_admin: Glass technology administration (v1.0)
 * Flat glass, container glass, fiberglass, specialty glass, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gls_t;
typedef struct{int n_fg,n_cg,n_gfb,n_spg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gls_state_t;
static gls_t fgs[N],cgs[N-2],gfbs[N-4],spgs[N-6],mks[N-6]; static gls_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gls_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gls_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gls_init(void){if(init)return -1;st.n_fg=0;st.n_cg=0;st.n_gfb=0;st.n_spg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fgs[i].active=0;for(int i=0;i<N-2;i++)cgs[i].active=0;for(int i=0;i<N-4;i++)gfbs[i].active=0;for(int i=0;i<N-6;i++)spgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GLS] Glasstech initialized\n");return 0;}
int gls_flat(int t,int c,int a,int b,int d,int e,int y){return add(fgs,&st.n_fg,&st.t_f1,N,t,c,a,b,d,e,y);}
int gls_container(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gls_fiber(int t,int c,int a,int b,int d,int e,int y){return add(gfbs,&st.n_gfb,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gls_specialty(int t,int c,int a,int b,int d,int e,int y){return add(spgs,&st.n_spg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gls_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gls_report(void){ps("[GLS] Flat: ");pi(st.n_fg);ps(" m2=");pi(st.t_f1);ps("\nCont: ");pi(st.n_cg);ps(" PCS=");pi(st.t_f2);ps("\nFiber: ");pi(st.n_gfb);ps(" kg=");pi(st.t_f3);ps("\nSpec: ");pi(st.n_spg);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gls_state(void){ps("[GLS] Fg=");pi(st.n_fg);ps(" Cg=");pi(st.n_cg);ps(" Gfb=");pi(st.n_gfb);ps(" Spg=");pi(st.n_spg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Glass Tech Admin Demo ===\n\n");gls_init();
ps("Flat glass...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gls_flat(t,c,208+(i*17),193+(i*14),173+(i*10),155+(i*6),2020+(i%5));}
ps("\nContainer glass...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gls_container(t,c,197+(i*15),183+(i*12),165+(i*8),152+(i*5),2021+(i%4));}
ps("\nFiberglass...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gls_fiber(t,c,189+(i*13),175+(i*10),159+(i*7),148+(i*4),2022+(i%3));}
ps("\nSpecialty glass...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gls_specialty(t,c,181+(i*11),169+(i*9),155+(i*6),145+(i*3),2023+(i%2));}
ps("\nGlass marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gls_market(t,c,175+(i*9),164+(i*7),151+(i*5),143+(i*3),2024);}
ps("\n");gls_report();gls_state();ps("\n=== Demo Complete ===\n");return 0;}
