/* diemakingtech_admin: Die making technology administration (v1.0)
 * Stamping dies, plastic dies, casting dies, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dmk_t;
typedef struct{int n_st,n_pl,n_cs,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dmk_state_t;
static dmk_t sts[N],pls[N-2],css[N-4],acs[N-6],mks[N-6]; static dmk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dmk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dmk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DMK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dmk_init(void){if(init)return -1;st.n_st=0;st.n_pl=0;st.n_cs=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)pls[i].active=0;for(int i=0;i<N-4;i++)css[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DMK] Diemakingtech initialized\n");return 0;}
int dmk_stamping(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int dmk_plastic(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dmk_casting(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dmk_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dmk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dmk_report(void){ps("[DMK] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f2);ps("\nCs: ");pi(st.n_cs);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dmk_state(void){ps("[DMK] St=");pi(st.n_st);ps(" Pl=");pi(st.n_pl);ps(" Cs=");pi(st.n_cs);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Die Making Tech Admin Demo ===\n\n");dmk_init();
ps("Stamping dies...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dmk_stamping(t,c,294+(i*17),279+(i*14),259+(i*10),241+(i*6),2020+(i%5));}
ps("\nPlastic dies...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dmk_plastic(t,c,283+(i*15),269+(i*12),251+(i*8),238+(i*5),2021+(i%4));}
ps("\nCasting dies...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dmk_casting(t,c,275+(i*13),261+(i*10),245+(i*7),234+(i*4),2022+(i%3));}
ps("\nDie accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dmk_accessory(t,c,267+(i*11),255+(i*9),241+(i*6),231+(i*3),2023+(i%2));}
ps("\nDie marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dmk_market(t,c,261+(i*9),250+(i*7),237+(i*5),229+(i*3),2024);}
ps("\n");dmk_report();dmk_state();ps("\n=== Demo Complete ===\n");return 0;}
