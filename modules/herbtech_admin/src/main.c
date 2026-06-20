/* herbtech_admin: Herbal technology administration (v1.0)
 * Herb planting, herb processing, herb products, herb R&D, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hrb_t;
typedef struct{int n_hp,n_hg,n_hpd,n_hr,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hrb_state_t;
static hrb_t hps[N],hgs[N-2],hpds[N-4],hrs[N-6],mks[N-6]; static hrb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hrb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hrb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HRB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hrb_init(void){if(init)return -1;st.n_hp=0;st.n_hg=0;st.n_hpd=0;st.n_hr=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hps[i].active=0;for(int i=0;i<N-2;i++)hgs[i].active=0;for(int i=0;i<N-4;i++)hpds[i].active=0;for(int i=0;i<N-6;i++)hrs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HRB] Herbtech initialized\n");return 0;}
int hrb_plant(int t,int c,int a,int b,int d,int e,int y){return add(hps,&st.n_hp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hrb_process(int t,int c,int a,int b,int d,int e,int y){return add(hgs,&st.n_hg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hrb_product(int t,int c,int a,int b,int d,int e,int y){return add(hpds,&st.n_hpd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hrb_research(int t,int c,int a,int b,int d,int e,int y){return add(hrs,&st.n_hr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hrb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hrb_report(void){ps("[HRB] Plant: ");pi(st.n_hp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_hg);ps(" kg=");pi(st.t_f2);ps("\nProd: ");pi(st.n_hpd);ps(" SKU=");pi(st.t_f3);ps("\nR&D: ");pi(st.n_hr);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hrb_state(void){ps("[HRB] Hp=");pi(st.n_hp);ps(" Hg=");pi(st.n_hg);ps(" Hpd=");pi(st.n_hpd);ps(" Hr=");pi(st.n_hr);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Herb Tech Admin Demo ===\n\n");hrb_init();
ps("Herb planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hrb_plant(t,c,195+(i*17),180+(i*14),160+(i*10),142+(i*6),2020+(i%5));}
ps("\nHerb processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hrb_process(t,c,184+(i*15),170+(i*12),152+(i*8),139+(i*5),2021+(i%4));}
ps("\nHerb products...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hrb_product(t,c,176+(i*13),162+(i*10),146+(i*7),135+(i*4),2022+(i%3));}
ps("\nHerb R&D...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrb_research(t,c,168+(i*11),156+(i*9),142+(i*6),132+(i*3),2023+(i%2));}
ps("\nHerb marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrb_market(t,c,162+(i*9),151+(i*7),138+(i*5),130+(i*3),2024);}
ps("\n");hrb_report();hrb_state();ps("\n=== Demo Complete ===\n");return 0;}
