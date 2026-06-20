/* forgtech_admin: Forging technology administration (v1.0)
 * Free forging, die forging, special forging, forging equipment, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frg_t;
typedef struct{int n_ff,n_df,n_sf,n_fe,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} frg_state_t;
static frg_t ffs[N],dfs[N-2],sfs[N-4],fes[N-6],mks[N-6]; static frg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frg_init(void){if(init)return -1;st.n_ff=0;st.n_df=0;st.n_sf=0;st.n_fe=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ffs[i].active=0;for(int i=0;i<N-2;i++)dfs[i].active=0;for(int i=0;i<N-4;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)fes[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FRG] Forgtech initialized\n");return 0;}
int frg_free(int t,int c,int a,int b,int d,int e,int y){return add(ffs,&st.n_ff,&st.t_f1,N,t,c,a,b,d,e,y);}
int frg_die(int t,int c,int a,int b,int d,int e,int y){return add(dfs,&st.n_df,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frg_special(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frg_equipment(int t,int c,int a,int b,int d,int e,int y){return add(fes,&st.n_fe,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frg_report(void){ps("[FRG] Ff: ");pi(st.n_ff);ps(" PCS=");pi(st.t_f1);ps("\nDf: ");pi(st.n_df);ps(" PCS=");pi(st.t_f2);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f3);ps("\nFe: ");pi(st.n_fe);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void frg_state(void){ps("[FRG] Ff=");pi(st.n_ff);ps(" Df=");pi(st.n_df);ps(" Sf=");pi(st.n_sf);ps(" Fe=");pi(st.n_fe);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Forging Tech Admin Demo ===\n\n");frg_init();
ps("Free forging...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frg_free(t,c,247+(i*17),232+(i*14),212+(i*10),194+(i*6),2020+(i%5));}
ps("\nDie forging...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frg_die(t,c,236+(i*15),222+(i*12),204+(i*8),191+(i*5),2021+(i%4));}
ps("\nSpecial forging...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frg_special(t,c,228+(i*13),214+(i*10),198+(i*7),187+(i*4),2022+(i%3));}
ps("\nForging equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frg_equipment(t,c,220+(i*11),208+(i*9),194+(i*6),184+(i*3),2023+(i%2));}
ps("\nForging marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frg_market(t,c,214+(i*9),203+(i*7),190+(i*5),182+(i*3),2024);}
ps("\n");frg_report();frg_state();ps("\n=== Demo Complete ===\n");return 0;}
