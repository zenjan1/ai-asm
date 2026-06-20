/* baketech_admin: Bake technology administration (v1.0)
 * Bread making, cake making, cookie making, pastry making, bake equipment
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bkt_t;
typedef struct{int n_bd,n_ck,n_ci,n_ps,n_eq,t_f1,t_f2,t_f3,t_f4,t_f5;} bkt_state_t;
static bkt_t bds[N],cks[N-2],cis[N-4],pss[N-6],eqs[N-6]; static bkt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bkt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bkt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BKT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bkt_init(void){if(init)return -1;st.n_bd=0;st.n_ck=0;st.n_ci=0;st.n_ps=0;st.n_eq=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bds[i].active=0;for(int i=0;i<N-2;i++)cks[i].active=0;for(int i=0;i<N-4;i++)cis[i].active=0;for(int i=0;i<N-6;i++)pss[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;init=1;ps("[BKT] Baketech initialized\n");return 0;}
int bkt_bread(int t,int c,int a,int b,int d,int e,int y){return add(bds,&st.n_bd,&st.t_f1,N,t,c,a,b,d,e,y);}
int bkt_cake(int t,int c,int a,int b,int d,int e,int y){return add(cks,&st.n_ck,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bkt_cookie(int t,int c,int a,int b,int d,int e,int y){return add(cis,&st.n_ci,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bkt_pastry(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bkt_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bkt_report(void){ps("[BKT] Bread: ");pi(st.n_bd);ps(" Loaf=");pi(st.t_f1);ps("\nCake: ");pi(st.n_ck);ps(" Layer=");pi(st.t_f2);ps("\nCookie: ");pi(st.n_ci);ps(" PCS=");pi(st.t_f3);ps("\nPastry: ");pi(st.n_ps);ps(" Type=");pi(st.t_f4);ps("\nEquip: ");pi(st.n_eq);ps(" Oven=");pi(st.t_f5);ps("\n");}
void bkt_state(void){ps("[BKT] Bd=");pi(st.n_bd);ps(" Ck=");pi(st.n_ck);ps(" Ci=");pi(st.n_ci);ps(" Ps=");pi(st.n_ps);ps(" Eq=");pi(st.n_eq);ps("\n");}
int main(void){
ps("=== Bake Tech Admin Demo ===\n\n");bkt_init();
ps("Bread making...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bkt_bread(t,c,177+(i*17),162+(i*14),142+(i*10),125+(i*6),2020+(i%5));}
ps("\nCake making...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bkt_cake(t,c,166+(i*15),152+(i*12),134+(i*8),121+(i*5),2021+(i%4));}
ps("\nCookie making...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bkt_cookie(t,c,158+(i*13),144+(i*10),128+(i*7),117+(i*4),2022+(i%3));}
ps("\nPastry making...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bkt_pastry(t,c,150+(i*11),138+(i*9),124+(i*6),114+(i*3),2023+(i%2));}
ps("\nBake equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bkt_equipment(t,c,144+(i*9),133+(i*7),120+(i*5),112+(i*3),2024);}
ps("\n");bkt_report();bkt_state();ps("\n=== Demo Complete ===\n");return 0;}
