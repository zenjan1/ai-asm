/* fashiontech_admin: Fashion technology administration (v1.0)
 * Smart design, virtual fitting, fashion supply chain, sustainable fashion, digital fashion
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fst_t;
typedef struct{int n_ds,n_vf,n_sc,n_sf,n_df,t_f1,t_f2,t_f3,t_f4,t_f5;} fst_state_t;
static fst_t dss[N],vfs[N-2],scs[N-4],sfs[N-6],dfs[N-6]; static fst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fst_init(void){if(init)return -1;st.n_ds=0;st.n_vf=0;st.n_sc=0;st.n_sf=0;st.n_df=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dss[i].active=0;for(int i=0;i<N-2;i++)vfs[i].active=0;for(int i=0;i<N-4;i++)scs[i].active=0;for(int i=0;i<N-6;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)dfs[i].active=0;init=1;ps("[FST] Fashiontech initialized\n");return 0;}
int fst_design(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f1,N,t,c,a,b,d,e,y);}
int fst_fitting(int t,int c,int a,int b,int d,int e,int y){return add(vfs,&st.n_vf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fst_supply(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fst_sustain(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fst_digital(int t,int c,int a,int b,int d,int e,int y){return add(dfs,&st.n_df,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fst_report(void){ps("[FST] Design: ");pi(st.n_ds);ps(" AI=");pi(st.t_f1);ps("\nFit: ");pi(st.n_vf);ps(" AR=");pi(st.t_f2);ps("\nSCM: ");pi(st.n_sc);ps(" Plan=");pi(st.t_f3);ps("\nSust: ");pi(st.n_sf);ps(" Eco=");pi(st.t_f4);ps("\nDigi: ");pi(st.n_df);ps(" NFT=");pi(st.t_f5);ps("\n");}
void fst_state(void){ps("[FST] Ds=");pi(st.n_ds);ps(" Vf=");pi(st.n_vf);ps(" Sc=");pi(st.n_sc);ps(" Sf=");pi(st.n_sf);ps(" Df=");pi(st.n_df);ps("\n");}
int main(void){
ps("=== Fashion Tech Admin Demo ===\n\n");fst_init();
ps("Smart design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fst_design(t,c,150+(i*17),135+(i*14),115+(i*10),98+(i*6),2020+(i%5));}
ps("\nVirtual fitting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fst_fitting(t,c,139+(i*15),125+(i*12),107+(i*8),94+(i*5),2021+(i%4));}
ps("\nFashion supply chain...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fst_supply(t,c,131+(i*13),117+(i*10),101+(i*7),90+(i*4),2022+(i%3));}
ps("\nSustainable fashion...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fst_sustain(t,c,123+(i*11),111+(i*9),97+(i*6),87+(i*3),2023+(i%2));}
ps("\nDigital fashion...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fst_digital(t,c,117+(i*9),106+(i*7),93+(i*5),85+(i*3),2024);}
ps("\n");fst_report();fst_state();ps("\n=== Demo Complete ===\n");return 0;}
