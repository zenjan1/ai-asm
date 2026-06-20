/* textitech_admin: Textile technology administration (v1.0)
 * Smart textiles, functional fabrics, textile equipment, industrial textiles, textile design
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} txt_t;
typedef struct{int n_sm,n_fn,n_eq,n_it,n_td,t_f1,t_f2,t_f3,t_f4,t_f5;} txt_state_t;
static txt_t sms[N],fns[N-2],eqs[N-4],its[N-6],tds[N-6]; static txt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(txt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;txt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int txt_init(void){if(init)return -1;st.n_sm=0;st.n_fn=0;st.n_eq=0;st.n_it=0;st.n_td=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sms[i].active=0;for(int i=0;i<N-2;i++)fns[i].active=0;for(int i=0;i<N-4;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)its[i].active=0;for(int i=0;i<N-6;i++)tds[i].active=0;init=1;ps("[TXT] Textitech initialized\n");return 0;}
int txt_smart(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f1,N,t,c,a,b,d,e,y);}
int txt_functional(int t,int c,int a,int b,int d,int e,int y){return add(fns,&st.n_fn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int txt_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int txt_industrial(int t,int c,int a,int b,int d,int e,int y){return add(its,&st.n_it,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int txt_design(int t,int c,int a,int b,int d,int e,int y){return add(tds,&st.n_td,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void txt_report(void){ps("[TXT] Smart: ");pi(st.n_sm);ps(" eTex=");pi(st.t_f1);ps("\nFunc: ");pi(st.n_fn);ps(" WP=");pi(st.t_f2);ps("\nEquip: ");pi(st.n_eq);ps(" RPM=");pi(st.t_f3);ps("\nInd: ");pi(st.n_it);ps(" Geo=");pi(st.t_f4);ps("\nDesign: ");pi(st.n_td);ps(" CAD=");pi(st.t_f5);ps("\n");}
void txt_state(void){ps("[TXT] Sm=");pi(st.n_sm);ps(" Fn=");pi(st.n_fn);ps(" Eq=");pi(st.n_eq);ps(" It=");pi(st.n_it);ps(" Td=");pi(st.n_td);ps("\n");}
int main(void){
ps("=== Textile Tech Admin Demo ===\n\n");txt_init();
ps("Smart textiles...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;txt_smart(t,c,159+(i*17),144+(i*14),124+(i*10),107+(i*6),2020+(i%5));}
ps("\nFunctional fabrics...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;txt_functional(t,c,148+(i*15),134+(i*12),116+(i*8),103+(i*5),2021+(i%4));}
ps("\nTextile equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;txt_equipment(t,c,140+(i*13),126+(i*10),110+(i*7),99+(i*4),2022+(i%3));}
ps("\nIndustrial textiles...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;txt_industrial(t,c,132+(i*11),120+(i*9),106+(i*6),96+(i*3),2023+(i%2));}
ps("\nTextile design...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;txt_design(t,c,126+(i*9),115+(i*7),102+(i*5),94+(i*3),2024);}
ps("\n");txt_report();txt_state();ps("\n=== Demo Complete ===\n");return 0;}
