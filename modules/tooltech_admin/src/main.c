/* tooltech_admin: Tool technology administration (v1.0)
 * Hand tools, power tools, pneumatic tools, measuring tools, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tls_t;
typedef struct{int n_ht,n_pt,n_pn,n_mt,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tls_state_t;
static tls_t hts[N],pts[N-2],pns[N-4],mts[N-6],mks[N-6]; static tls_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tls_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tls_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TLS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tls_init(void){if(init)return -1;st.n_ht=0;st.n_pt=0;st.n_pn=0;st.n_mt=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hts[i].active=0;for(int i=0;i<N-2;i++)pts[i].active=0;for(int i=0;i<N-4;i++)pns[i].active=0;for(int i=0;i<N-6;i++)mts[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TLS] Tooltech initialized\n");return 0;}
int tls_hand(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f1,N,t,c,a,b,d,e,y);}
int tls_power(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tls_pneumatic(int t,int c,int a,int b,int d,int e,int y){return add(pns,&st.n_pn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tls_measure(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tls_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tls_report(void){ps("[TLS] Ht: ");pi(st.n_ht);ps(" PCS=");pi(st.t_f1);ps("\nPt: ");pi(st.n_pt);ps(" PCS=");pi(st.t_f2);ps("\nPn: ");pi(st.n_pn);ps(" PCS=");pi(st.t_f3);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tls_state(void){ps("[TLS] Ht=");pi(st.n_ht);ps(" Pt=");pi(st.n_pt);ps(" Pn=");pi(st.n_pn);ps(" Mt=");pi(st.n_mt);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tool Tech Admin Demo ===\n\n");tls_init();
ps("Hand tools...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tls_hand(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nPower tools...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tls_power(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nPneumatic tools...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tls_pneumatic(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nMeasuring tools...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tls_measure(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nTool marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tls_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");tls_report();tls_state();ps("\n=== Demo Complete ===\n");return 0;}
