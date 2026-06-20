/* ironsteel_admin: Iron and steel technology administration (v1.0)
 * Iron smelting, steel production, aluminum production, metal products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ist_t;
typedef struct{int n_iron,n_steel,n_alu,n_mp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ist_state_t;
static ist_t irons[N],steels[N-2],alus[N-4],mps[N-6],mks[N-6]; static ist_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ist_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ist_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ist_init(void){if(init)return -1;st.n_iron=0;st.n_steel=0;st.n_alu=0;st.n_mp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)irons[i].active=0;for(int i=0;i<N-2;i++)steels[i].active=0;for(int i=0;i<N-4;i++)alus[i].active=0;for(int i=0;i<N-6;i++)mps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IST] Ironsteel initialized\n");return 0;}
int ist_iron(int t,int c,int a,int b,int d,int e,int y){return add(irons,&st.n_iron,&st.t_f1,N,t,c,a,b,d,e,y);}
int ist_steel(int t,int c,int a,int b,int d,int e,int y){return add(steels,&st.n_steel,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ist_alu(int t,int c,int a,int b,int d,int e,int y){return add(alus,&st.n_alu,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ist_product(int t,int c,int a,int b,int d,int e,int y){return add(mps,&st.n_mp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ist_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ist_report(void){ps("[IST] Iron: ");pi(st.n_iron);ps(" Ton=");pi(st.t_f1);ps("\nSteel: ");pi(st.n_steel);ps(" Ton=");pi(st.t_f2);ps("\nAlu: ");pi(st.n_alu);ps(" kg=");pi(st.t_f3);ps("\nProd: ");pi(st.n_mp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ist_state(void){ps("[IST] Iron=");pi(st.n_iron);ps(" Steel=");pi(st.n_steel);ps(" Alu=");pi(st.n_alu);ps(" Mp=");pi(st.n_mp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Iron Steel Tech Admin Demo ===\n\n");ist_init();
ps("Iron smelting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ist_iron(t,c,211+(i*17),196+(i*14),176+(i*10),158+(i*6),2020+(i%5));}
ps("\nSteel production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ist_steel(t,c,200+(i*15),186+(i*12),168+(i*8),155+(i*5),2021+(i%4));}
ps("\nAluminum production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ist_alu(t,c,192+(i*13),178+(i*10),162+(i*7),151+(i*4),2022+(i%3));}
ps("\nMetal products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ist_product(t,c,184+(i*11),172+(i*9),158+(i*6),148+(i*3),2023+(i%2));}
ps("\nMetal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ist_market(t,c,178+(i*9),167+(i*7),154+(i*5),146+(i*3),2024);}
ps("\n");ist_report();ist_state();ps("\n=== Demo Complete ===\n");return 0;}
