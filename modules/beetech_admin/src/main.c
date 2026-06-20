/* beetech_admin: Bee technology administration (v1.0)
 * Bee breeding, bee product processing, bee equipment, bee disease, bee tourism
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bet_t;
typedef struct{int n_be,n_pr,n_eq,n_di,n_to,t_f1,t_f2,t_f3,t_f4,t_f5;} bet_state_t;
static bet_t bes[N],prs[N-2],eqs[N-4],dis[N-6],tos[N-6]; static bet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bet_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bet_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bet_init(void){if(init)return -1;st.n_be=0;st.n_pr=0;st.n_eq=0;st.n_di=0;st.n_to=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bes[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)dis[i].active=0;for(int i=0;i<N-6;i++)tos[i].active=0;init=1;ps("[BET] Beetech initialized\n");return 0;}
int bet_bee(int t,int c,int a,int b,int d,int e,int y){return add(bes,&st.n_be,&st.t_f1,N,t,c,a,b,d,e,y);}
int bet_product(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bet_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bet_disease(int t,int c,int a,int b,int d,int e,int y){return add(dis,&st.n_di,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bet_tourism(int t,int c,int a,int b,int d,int e,int y){return add(tos,&st.n_to,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bet_report(void){ps("[BET] Bee: ");pi(st.n_be);ps(" Hive=");pi(st.t_f1);ps("\nProd: ");pi(st.n_pr);ps(" kg=");pi(st.t_f2);ps("\nEquip: ");pi(st.n_eq);ps(" Box=");pi(st.t_f3);ps("\nDis: ");pi(st.n_di);ps(" Mite=");pi(st.t_f4);ps("\nTour: ");pi(st.n_to);ps(" Pax=");pi(st.t_f5);ps("\n");}
void bet_state(void){ps("[BET] Be=");pi(st.n_be);ps(" Pr=");pi(st.n_pr);ps(" Eq=");pi(st.n_eq);ps(" Di=");pi(st.n_di);ps(" To=");pi(st.n_to);ps("\n");}
int main(void){
ps("=== Bee Tech Admin Demo ===\n\n");bet_init();
ps("Bee breeding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bet_bee(t,c,172+(i*17),157+(i*14),137+(i*10),120+(i*6),2020+(i%5));}
ps("\nBee product processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bet_product(t,c,161+(i*15),147+(i*12),129+(i*8),116+(i*5),2021+(i%4));}
ps("\nBee equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bet_equipment(t,c,153+(i*13),139+(i*10),123+(i*7),112+(i*4),2022+(i%3));}
ps("\nBee disease control...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bet_disease(t,c,145+(i*11),133+(i*9),119+(i*6),109+(i*3),2023+(i%2));}
ps("\nBee tourism...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bet_tourism(t,c,139+(i*9),128+(i*7),115+(i*5),107+(i*3),2024);}
ps("\n");bet_report();bet_state();ps("\n=== Demo Complete ===\n");return 0;}
