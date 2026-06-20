/* jewelrytech_admin: Jewelry technology administration (v1.0)
 * Rings, necklaces, earrings, bracelets, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jwl_t;
typedef struct{int n_rg,n_nk,n_er,n_br,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jwl_state_t;
static jwl_t rgs[N],nks[N-2],ers[N-4],brs[N-6],mks[N-6]; static jwl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jwl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jwl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JWL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jwl_init(void){if(init)return -1;st.n_rg=0;st.n_nk=0;st.n_er=0;st.n_br=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rgs[i].active=0;for(int i=0;i<N-2;i++)nks[i].active=0;for(int i=0;i<N-4;i++)ers[i].active=0;for(int i=0;i<N-6;i++)brs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[JWL] Jewelrytech initialized\n");return 0;}
int jwl_ring(int t,int c,int a,int b,int d,int e,int y){return add(rgs,&st.n_rg,&st.t_f1,N,t,c,a,b,d,e,y);}
int jwl_necklace(int t,int c,int a,int b,int d,int e,int y){return add(nks,&st.n_nk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jwl_earring(int t,int c,int a,int b,int d,int e,int y){return add(ers,&st.n_er,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jwl_bracelet(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jwl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jwl_report(void){ps("[JWL] Ring: ");pi(st.n_rg);ps(" PCS=");pi(st.t_f1);ps("\nNeck: ");pi(st.n_nk);ps(" PCS=");pi(st.t_f2);ps("\nEar: ");pi(st.n_er);ps(" PCS=");pi(st.t_f3);ps("\nBrac: ");pi(st.n_br);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jwl_state(void){ps("[JWL] Rg=");pi(st.n_rg);ps(" Nk=");pi(st.n_nk);ps(" Er=");pi(st.n_er);ps(" Br=");pi(st.n_br);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jewelry Tech Admin Demo ===\n\n");jwl_init();
ps("Rings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jwl_ring(t,c,203+(i*17),188+(i*14),168+(i*10),150+(i*6),2020+(i%5));}
ps("\nNecklaces...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jwl_necklace(t,c,192+(i*15),178+(i*12),160+(i*8),147+(i*5),2021+(i%4));}
ps("\nEarrings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jwl_earring(t,c,184+(i*13),170+(i*10),154+(i*7),143+(i*4),2022+(i%3));}
ps("\nBracelets...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jwl_bracelet(t,c,176+(i*11),164+(i*9),150+(i*6),140+(i*3),2023+(i%2));}
ps("\nJewelry marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jwl_market(t,c,170+(i*9),159+(i*7),146+(i*5),138+(i*3),2024);}
ps("\n");jwl_report();jwl_state();ps("\n=== Demo Complete ===\n");return 0;}
