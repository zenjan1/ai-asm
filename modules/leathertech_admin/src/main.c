/* leathertech_admin: Leather technology administration (v1.0)
 * Leather processing, leather shoes, leather bags, leather clothing, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lth_t;
typedef struct{int n_lp,n_ls,n_lb,n_lc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lth_state_t;
static lth_t lps[N],lss[N-2],lbs[N-4],lcs[N-6],mks[N-6]; static lth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lth_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lth_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lth_init(void){if(init)return -1;st.n_lp=0;st.n_ls=0;st.n_lb=0;st.n_lc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lps[i].active=0;for(int i=0;i<N-2;i++)lss[i].active=0;for(int i=0;i<N-4;i++)lbs[i].active=0;for(int i=0;i<N-6;i++)lcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LTH] Leathertech initialized\n");return 0;}
int lth_process(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lth_shoe(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lth_bag(int t,int c,int a,int b,int d,int e,int y){return add(lbs,&st.n_lb,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lth_clothing(int t,int c,int a,int b,int d,int e,int y){return add(lcs,&st.n_lc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lth_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lth_report(void){ps("[LTH] Lp: ");pi(st.n_lp);ps(" PCS=");pi(st.t_f1);ps("\nLs: ");pi(st.n_ls);ps(" PCS=");pi(st.t_f2);ps("\nLb: ");pi(st.n_lb);ps(" PCS=");pi(st.t_f3);ps("\nLc: ");pi(st.n_lc);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lth_state(void){ps("[LTH] Lp=");pi(st.n_lp);ps(" Ls=");pi(st.n_ls);ps(" Lb=");pi(st.n_lb);ps(" Lc=");pi(st.n_lc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leather Tech Admin Demo ===\n\n");lth_init();
ps("Leather processing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lth_process(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nLeather shoes...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lth_shoe(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nLeather bags...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lth_bag(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nLeather clothing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lth_clothing(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nLeather marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lth_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");lth_report();lth_state();ps("\n=== Demo Complete ===\n");return 0;}
