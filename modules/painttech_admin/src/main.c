/* painttech_admin: Paint technology administration (v1.0)
 * Paint manufacturing, pigment production, coating application, special paints, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pnt_t;
typedef struct{int n_ptm,n_pgm,n_ctg,n_spc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pnt_state_t;
static pnt_t ptms[N],pgms[N-2],ctgs[N-4],spcs[N-6],mks[N-6]; static pnt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pnt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pnt_init(void){if(init)return -1;st.n_ptm=0;st.n_pgm=0;st.n_ctg=0;st.n_spc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ptms[i].active=0;for(int i=0;i<N-2;i++)pgms[i].active=0;for(int i=0;i<N-4;i++)ctgs[i].active=0;for(int i=0;i<N-6;i++)spcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PNT] Painttech initialized\n");return 0;}
int pnt_paint(int t,int c,int a,int b,int d,int e,int y){return add(ptms,&st.n_ptm,&st.t_f1,N,t,c,a,b,d,e,y);}
int pnt_pigment(int t,int c,int a,int b,int d,int e,int y){return add(pgms,&st.n_pgm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pnt_coat(int t,int c,int a,int b,int d,int e,int y){return add(ctgs,&st.n_ctg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pnt_special(int t,int c,int a,int b,int d,int e,int y){return add(spcs,&st.n_spc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pnt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pnt_report(void){ps("[PNT] Paint: ");pi(st.n_ptm);ps(" L=");pi(st.t_f1);ps("\nPigment: ");pi(st.n_pgm);ps(" kg=");pi(st.t_f2);ps("\nCoat: ");pi(st.n_ctg);ps(" m2=");pi(st.t_f3);ps("\nSpec: ");pi(st.n_spc);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pnt_state(void){ps("[PNT] Ptm=");pi(st.n_ptm);ps(" Pgm=");pi(st.n_pgm);ps(" Ctg=");pi(st.n_ctg);ps(" Spc=");pi(st.n_spc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Paint Tech Admin Demo ===\n\n");pnt_init();
ps("Paint manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pnt_paint(t,c,213+(i*17),198+(i*14),178+(i*10),160+(i*6),2020+(i%5));}
ps("\nPigment production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pnt_pigment(t,c,202+(i*15),188+(i*12),170+(i*8),157+(i*5),2021+(i%4));}
ps("\nCoating application...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pnt_coat(t,c,194+(i*13),180+(i*10),164+(i*7),153+(i*4),2022+(i%3));}
ps("\nSpecial paints...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnt_special(t,c,186+(i*11),174+(i*9),160+(i*6),150+(i*3),2023+(i%2));}
ps("\nPaint marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnt_market(t,c,180+(i*9),169+(i*7),156+(i*5),148+(i*3),2024);}
ps("\n");pnt_report();pnt_state();ps("\n=== Demo Complete ===\n");return 0;}
