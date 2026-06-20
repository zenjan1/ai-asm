/* furnitech_admin: Furniture technology administration (v1.0)
 * Solid wood furniture, panel furniture, soft furniture, office furniture, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frn_t;
typedef struct{int n_sw,n_pn,n_sf,n_of,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} frn_state_t;
static frn_t sws[N],pns[N-2],sfs[N-4],ofs[N-6],mks[N-6]; static frn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frn_init(void){if(init)return -1;st.n_sw=0;st.n_pn=0;st.n_sf=0;st.n_of=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sws[i].active=0;for(int i=0;i<N-2;i++)pns[i].active=0;for(int i=0;i<N-4;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)ofs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FRN] Furnitech initialized\n");return 0;}
int frn_solid(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f1,N,t,c,a,b,d,e,y);}
int frn_panel(int t,int c,int a,int b,int d,int e,int y){return add(pns,&st.n_pn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frn_soft(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frn_office(int t,int c,int a,int b,int d,int e,int y){return add(ofs,&st.n_of,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frn_report(void){ps("[FRN] Sw: ");pi(st.n_sw);ps(" PCS=");pi(st.t_f1);ps("\nPn: ");pi(st.n_pn);ps(" PCS=");pi(st.t_f2);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f3);ps("\nOf: ");pi(st.n_of);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void frn_state(void){ps("[FRN] Sw=");pi(st.n_sw);ps(" Pn=");pi(st.n_pn);ps(" Sf=");pi(st.n_sf);ps(" Of=");pi(st.n_of);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Furniture Tech Admin Demo ===\n\n");frn_init();
ps("Solid wood furniture...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frn_solid(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nPanel furniture...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frn_panel(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nSoft furniture...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frn_soft(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nOffice furniture...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frn_office(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nFurniture marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frn_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");frn_report();frn_state();ps("\n=== Demo Complete ===\n");return 0;}
