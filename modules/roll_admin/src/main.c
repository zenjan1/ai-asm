/* roll_admin: Rolling mill technology administration (v1.0)
 * Hot rolling mill, cold rolling mill, special rolling mill, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rll_t;
typedef struct{int n_ht,n_cd,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rll_state_t;
static rll_t hts[N],cds[N-2],sps[N-4],acs[N-6],mks[N-6]; static rll_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rll_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rll_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RLL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rll_init(void){if(init)return -1;st.n_ht=0;st.n_cd=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hts[i].active=0;for(int i=0;i<N-2;i++)cds[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RLL] Roll initialized\n");return 0;}
int rll_hot(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f1,N,t,c,a,b,d,e,y);}
int rll_cold(int t,int c,int a,int b,int d,int e,int y){return add(cds,&st.n_cd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rll_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rll_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rll_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rll_report(void){ps("[RLL] Ht: ");pi(st.n_ht);ps(" PCS=");pi(st.t_f1);ps("\nCd: ");pi(st.n_cd);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rll_state(void){ps("[RLL] Ht=");pi(st.n_ht);ps(" Cd=");pi(st.n_cd);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Roll Admin Demo ===\n\n");rll_init();
ps("Hot rolling mills...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rll_hot(t,c,403+(i*17),388+(i*14),368+(i*10),350+(i*6),2020+(i%5));}
ps("\nCold rolling mills...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rll_cold(t,c,392+(i*15),378+(i*12),360+(i*8),347+(i*5),2021+(i%4));}
ps("\nSpecial rolling mills...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rll_special(t,c,384+(i*13),370+(i*10),354+(i*7),343+(i*4),2022+(i%3));}
ps("\nRoll accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rll_accessory(t,c,376+(i*11),364+(i*9),350+(i*6),340+(i*3),2023+(i%2));}
ps("\nRoll marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rll_market(t,c,370+(i*9),359+(i*7),346+(i*5),338+(i*3),2024);}
ps("\n");rll_report();rll_state();ps("\n=== Demo Complete ===\n");return 0;}
