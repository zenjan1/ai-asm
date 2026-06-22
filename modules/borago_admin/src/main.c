/* borago_admin: Borago management technology administration (v1.0)
 * Borago planning, borago execution, borago evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bora_t;
typedef struct{int n_borap,n_bora,n_bora2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bora_state_t;
static bora_t boras[N],borae[N-2],bora2[N-4],boraac[N-6],boram[N-6]; static bora_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bora_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bora_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BORA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bora_init(void){if(init)return -1;st.n_borap=0;st.n_bora=0;st.n_bora2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)boras[i].active=0;for(int i=0;i<N-2;i++)borae[i].active=0;for(int i=0;i<N-4;i++)bora2[i].active=0;for(int i=0;i<N-6;i++)boraac[i].active=0;for(int i=0;i<N-6;i++)boram[i].active=0;init=1;ps("[BORA] Borago initialized\n");return 0;}
int bora_planning(int t,int c,int a,int b,int d,int e,int y){return add(boras,&st.n_borap,&st.t_f1,N,t,c,a,b,d,e,y);}
int bora_execution(int t,int c,int a,int b,int d,int e,int y){return add(borae,&st.n_bora,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bora_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bora2,&st.n_bora2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bora_accessory(int t,int c,int a,int b,int d,int e,int y){return add(boraac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bora_market(int t,int c,int a,int b,int d,int e,int y){return add(boram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bora_report(void){ps("[BORA] Rap: ");pi(st.n_borap);ps(" PCS=");pi(st.t_f1);ps("\nRae: ");pi(st.n_bora);ps(" PCS=");pi(st.t_f2);ps("\nRa2: ");pi(st.n_bora2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bora_state(void){ps("[BORA] Rap=");pi(st.n_borap);ps(" Ra=");pi(st.n_bora);ps(" Ra2=");pi(st.n_bora2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Borago Admin Demo ===\n\n");bora_init();
ps("Borago planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bora_planning(t,c,1440+(i*17),1429+(i*14),1409+(i*10),1391+(i*6),2020+(i%5));}
ps("\nBorago execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bora_execution(t,c,1429+(i*15),1418+(i*12),1400+(i*8),1387+(i*5),2021+(i%4));}
ps("\nBorago evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bora_evaluation(t,c,1421+(i*13),1410+(i*10),1394+(i*7),1383+(i*4),2022+(i%3));}
ps("\nBorago accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bora_accessory(t,c,1413+(i*11),1404+(i*9),1390+(i*6),1380+(i*3),2023+(i%2));}
ps("\nBorago marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bora_market(t,c,1407+(i*9),1398+(i*7),1385+(i*5),1377+(i*3),2024);}
ps("\n");bora_report();bora_state();ps("\n=== Demo Complete ===\n");return 0;}
