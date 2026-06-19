/* public_economics_admin: Public economics administration (v1.0)
 * Public goods, externalities, public choice, tax design, fiscal federalism
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pub_t;
typedef struct{int n_pg,n_ex,n_pc,n_td,n_ff,t_f1,t_f2,t_f3,t_f4,t_f5;} pub_state_t;
static pub_t pgs[N],exs[N-2],pcs[N-4],tds[N-6],ffs[N-6]; static pub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PUB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pub_init(void){if(init)return -1;st.n_pg=0;st.n_ex=0;st.n_pc=0;st.n_td=0;st.n_ff=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pgs[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)pcs[i].active=0;for(int i=0;i<N-6;i++)tds[i].active=0;for(int i=0;i<N-6;i++)ffs[i].active=0;init=1;ps("[PUB] Public economics initialized\n");return 0;}
int pub_goods(int t,int c,int a,int b,int d,int e,int y){return add(pgs,&st.n_pg,&st.t_f1,N,t,c,a,b,d,e,y);}
int pub_external(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pub_choice(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pub_tax(int t,int c,int a,int b,int d,int e,int y){return add(tds,&st.n_td,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pub_federal(int t,int c,int a,int b,int d,int e,int y){return add(ffs,&st.n_ff,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pub_report(void){ps("[PUB] Goods: ");pi(st.n_pg);ps(" Pure=");pi(st.t_f1);ps("\nExtern: ");pi(st.n_ex);ps(" Pigou=");pi(st.t_f2);ps("\nChoice: ");pi(st.n_pc);ps(" Arrow=");pi(st.t_f3);ps("\nTax: ");pi(st.n_td);ps(" Ramsey=");pi(st.t_f4);ps("\nFederal: ");pi(st.n_ff);ps(" Tiebout=");pi(st.t_f5);ps("\n");}
void pub_state(void){ps("[PUB] Pg=");pi(st.n_pg);ps(" Ex=");pi(st.n_ex);ps(" Pc=");pi(st.n_pc);ps(" Td=");pi(st.n_td);ps(" Ff=");pi(st.n_ff);ps("\n");}
int main(void){
ps("=== Public Economics Admin Demo ===\n\n");pub_init();
ps("Public goods...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pub_goods(t,c,112+(i*17),97+(i*14),77+(i*10),60+(i*6),2020+(i%5));}
ps("\nExternalities...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pub_external(t,c,101+(i*15),87+(i*12),69+(i*8),56+(i*5),2021+(i%4));}
ps("\nPublic choice...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pub_choice(t,c,93+(i*13),79+(i*10),63+(i*7),52+(i*4),2022+(i%3));}
ps("\nTax design...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pub_tax(t,c,85+(i*11),73+(i*9),59+(i*6),49+(i*3),2023+(i%2));}
ps("\nFiscal federalism...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pub_federal(t,c,79+(i*9),68+(i*7),55+(i*5),47+(i*3),2024);}
ps("\n");pub_report();pub_state();ps("\n=== Demo Complete ===\n");return 0;}
