/* gametech_admin: Game technology administration (v1.0)
 * Game development, game engines, mobile games, PC games, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gmt_t;
typedef struct{int n_gd,n_ge,n_mg,n_pg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gmt_state_t;
static gmt_t gds[N],ges[N-2],mgs[N-4],pgs[N-6],mks[N-6]; static gmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gmt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gmt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GMT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gmt_init(void){if(init)return -1;st.n_gd=0;st.n_ge=0;st.n_mg=0;st.n_pg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gds[i].active=0;for(int i=0;i<N-2;i++)ges[i].active=0;for(int i=0;i<N-4;i++)mgs[i].active=0;for(int i=0;i<N-6;i++)pgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GMT] Gametech initialized\n");return 0;}
int gmt_develop(int t,int c,int a,int b,int d,int e,int y){return add(gds,&st.n_gd,&st.t_f1,N,t,c,a,b,d,e,y);}
int gmt_engine(int t,int c,int a,int b,int d,int e,int y){return add(ges,&st.n_ge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gmt_mobile(int t,int c,int a,int b,int d,int e,int y){return add(mgs,&st.n_mg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gmt_pc(int t,int c,int a,int b,int d,int e,int y){return add(pgs,&st.n_pg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gmt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gmt_report(void){ps("[GMT] Gd: ");pi(st.n_gd);ps(" PCS=");pi(st.t_f1);ps("\nGe: ");pi(st.n_ge);ps(" PCS=");pi(st.t_f2);ps("\nMg: ");pi(st.n_mg);ps(" PCS=");pi(st.t_f3);ps("\nPg: ");pi(st.n_pg);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gmt_state(void){ps("[GMT] Gd=");pi(st.n_gd);ps(" Ge=");pi(st.n_ge);ps(" Mg=");pi(st.n_mg);ps(" Pg=");pi(st.n_pg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Game Tech Admin Demo ===\n\n");gmt_init();
ps("Game development...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gmt_develop(t,c,235+(i*17),220+(i*14),200+(i*10),182+(i*6),2020+(i%5));}
ps("\nGame engines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gmt_engine(t,c,224+(i*15),210+(i*12),192+(i*8),179+(i*5),2021+(i%4));}
ps("\nMobile games...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gmt_mobile(t,c,216+(i*13),202+(i*10),186+(i*7),175+(i*4),2022+(i%3));}
ps("\nPC games...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gmt_pc(t,c,208+(i*11),196+(i*9),182+(i*6),172+(i*3),2023+(i%2));}
ps("\nGame marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gmt_market(t,c,202+(i*9),191+(i*7),178+(i*5),170+(i*3),2024);}
ps("\n");gmt_report();gmt_state();ps("\n=== Demo Complete ===\n");return 0;}
