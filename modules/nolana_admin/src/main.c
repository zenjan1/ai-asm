/* nolana_admin: Nolana Chilean bellflower management (v1.0)
 * Nolana paradoxa cultivation, coastal gardens, salt tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nol_t;
typedef struct{int n_nolc,n_nolb,n_nols,n_nolg,n_nolf,t_f1,t_f2,t_f3,t_f4,t_f5;} nol_state_t;
static nol_t nolcs[N],nolbs[N-2],nolss[N-4],nolgs[N-6],nolfs[N-6]; static nol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nol_init(void){if(init)return -1;st.n_nolc=0;st.n_nolb=0;st.n_nols=0;st.n_nolg=0;st.n_nolf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nolcs[i].active=0;for(int i=0;i<N-2;i++)nolbs[i].active=0;for(int i=0;i<N-4;i++)nolss[i].active=0;for(int i=0;i<N-6;i++)nolgs[i].active=0;for(int i=0;i<N-6;i++)nolfs[i].active=0;init=1;ps("[NOL] Nolana initialized\n");return 0;}
int nol_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(nolcs,&st.n_nolc,&st.t_f1,N,t,c,a,b,d,e,y);}
int nol_bellflower(int t,int c,int a,int b,int d,int e,int y){return add(nolbs,&st.n_nolb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nol_salt(int t,int c,int a,int b,int d,int e,int y){return add(nolss,&st.n_nols,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nol_ground(int t,int c,int a,int b,int d,int e,int y){return add(nolgs,&st.n_nolg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nol_foliage(int t,int c,int a,int b,int d,int e,int y){return add(nolfs,&st.n_nolf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nol_report(void){ps("[NOL] Cultivation: ");pi(st.n_nolc);ps(" PCS=");pi(st.t_f1);ps("\nBellflower: ");pi(st.n_nolb);ps(" PCS=");pi(st.t_f2);ps("\nSalt: ");pi(st.n_nols);ps(" PCS=");pi(st.t_f3);ps("\nGround: ");pi(st.n_nolg);ps(" PCS=");pi(st.t_f4);ps("\nFoliage: ");pi(st.n_nolf);ps(" USD=");pi(st.t_f5);ps("\n");}
void nol_state(void){ps("[NOL] Cul=");pi(st.n_nolc);ps(" Bel=");pi(st.n_nolb);ps(" Sal=");pi(st.n_nols);ps(" Gro=");pi(st.n_nolg);ps(" Fol=");pi(st.n_nolf);ps("\n");}
int main(void){
ps("=== Nolana Admin Demo ===\n\n");nol_init();
ps("Nolana cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nol_cultivation(t,c,997+(i*17),986+(i*14),966+(i*10),948+(i*6),2020+(i%5));}
ps("\nBellflower blooms...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nol_bellflower(t,c,986+(i*15),975+(i*12),957+(i*8),944+(i*5),2021+(i%4));}
ps("\nSalt tolerance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nol_salt(t,c,978+(i*13),967+(i*10),951+(i*7),940+(i*4),2022+(i%3));}
ps("\nGround cover...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nol_ground(t,c,970+(i*11),961+(i*9),947+(i*6),937+(i*3),2023+(i%2));}
ps("\nSucculent foliage...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nol_foliage(t,c,964+(i*9),955+(i*7),942+(i*5),934+(i*3),2024);}
ps("\n");nol_report();nol_state();ps("\n=== Demo Complete ===\n");return 0;}
