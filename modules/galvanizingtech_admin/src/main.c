/* galvanizingtech_admin: Galvanizing technology administration (v1.0)
 * Batch hot-dip galvanizing, continuous galvanizing, zinc alloy, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gvn_t;
typedef struct{int n_bt,n_ct,n_al,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gvn_state_t;
static gvn_t bts[N],cts[N-2],als[N-4],acs[N-6],mks[N-6]; static gvn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gvn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gvn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GVN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gvn_init(void){if(init)return -1;st.n_bt=0;st.n_ct=0;st.n_al=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bts[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)als[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GVN] Galvanizingtech initialized\n");return 0;}
int gvn_batch(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f1,N,t,c,a,b,d,e,y);}
int gvn_continuous(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gvn_alloy(int t,int c,int a,int b,int d,int e,int y){return add(als,&st.n_al,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gvn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gvn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gvn_report(void){ps("[GVN] Bt: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nAl: ");pi(st.n_al);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gvn_state(void){ps("[GVN] Bt=");pi(st.n_bt);ps(" Ct=");pi(st.n_ct);ps(" Al=");pi(st.n_al);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Galvanizing Tech Admin Demo ===\n\n");gvn_init();
ps("Batch hot-dip galvanizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gvn_batch(t,c,341+(i*17),326+(i*14),306+(i*10),288+(i*6),2020+(i%5));}
ps("\nContinuous galvanizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gvn_continuous(t,c,330+(i*15),316+(i*12),298+(i*8),285+(i*5),2021+(i%4));}
ps("\nZinc alloy galvanizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gvn_alloy(t,c,322+(i*13),308+(i*10),292+(i*7),281+(i*4),2022+(i%3));}
ps("\nGalvanizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gvn_accessory(t,c,314+(i*11),302+(i*9),288+(i*6),278+(i*3),2023+(i%2));}
ps("\nGalvanizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gvn_market(t,c,308+(i*9),297+(i*7),284+(i*5),276+(i*3),2024);}
ps("\n");gvn_report();gvn_state();ps("\n=== Demo Complete ===\n");return 0;}
