/* mercurialis_admin: Mercurialis management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} merc_t;
typedef struct{int n_mercop,n_merc,n_merc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} merc_state_t;
static merc_t mercox[N],merce[N-2],merc2[N-4],mercac[N-6],mercam[N-6]; static merc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(merc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;merc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MERC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int merc_init(void){if(init)return -1;st.n_mercop=0;st.n_merc=0;st.n_merc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mercox[i].active=0;for(int i=0;i<N-2;i++)merce[i].active=0;for(int i=0;i<N-4;i++)merc2[i].active=0;for(int i=0;i<N-6;i++)mercac[i].active=0;for(int i=0;i<N-6;i++)mercam[i].active=0;init=1;ps("[MERC] Mercurialis initialized\n");return 0;}
int merc_planning(int t,int c,int a,int b,int d,int e,int y){return add(mercox,&st.n_mercop,&st.t_f1,N,t,c,a,b,d,e,y);}
int merc_execution(int t,int c,int a,int b,int d,int e,int y){return add(merce,&st.n_merc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int merc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(merc2,&st.n_merc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int merc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mercac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int merc_market(int t,int c,int a,int b,int d,int e,int y){return add(mercam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void merc_report(void){ps("[MERC] Merop: ");pi(st.n_mercop);ps(" PCS=");pi(st.t_f1);ps("\nMerce: ");pi(st.n_merc);ps(" PCS=");pi(st.t_f2);ps("\nMerc2: ");pi(st.n_merc2);ps(" PCS=");pi(st.t_f3);ps("\nMrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void merc_state(void){ps("[MERC] Merop=");pi(st.n_mercop);ps(" Merc=");pi(st.n_merc);ps(" Rc2=");pi(st.n_merc2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mercurialis Admin Demo ===\n\n");merc_init();
ps("Mercurialis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;merc_planning(t,c,1571-(i*17),1560-(i*14),1540-(i*10),1522-(i*6),2020+(i%5));}
ps("\nMercurialis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;merc_execution(t,c,1560-(i*15),1549-(i*12),1531-(i*8),1518-(i*5),2021+(i%4));}
ps("\nMercurialis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;merc_evaluation(t,c,1552-(i*13),1541-(i*10),1525-(i*7),1514-(i*4),2022+(i%3));}
ps("\nMercurialis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;merc_accessory(t,c,1544-(i*11),1535-(i*9),1521-(i*6),1511-(i*3),2023+(i%2));}
ps("\nMercurialis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;merc_market(t,c,1538-(i*9),1529-(i*7),1516-(i*5),1508-(i*3),2024);}
ps("\n");merc_report();merc_state();ps("\n=== Demo Complete ===\n");return 0;}
