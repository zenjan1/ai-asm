/* baileyia_admin: Baileyia management technology administration (v1.0)
 * Baileyia planning, baileyia execution, baileyia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bai_t;
typedef struct{int n_bap,n_bae,n_bav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bai_state_t;
static bai_t baps[N],bass[N-2],bavss[N-4],baas[N-6],bamks[N-6]; static bai_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bai_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bai_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bai_init(void){if(init)return -1;st.n_bap=0;st.n_bae=0;st.n_bav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)baps[i].active=0;for(int i=0;i<N-2;i++)bass[i].active=0;for(int i=0;i<N-4;i++)bavss[i].active=0;for(int i=0;i<N-6;i++)baas[i].active=0;for(int i=0;i<N-6;i++)bamks[i].active=0;init=1;ps("[BAI] Baileyia initialized\n");return 0;}
int bai_planning(int t,int c,int a,int b,int d,int e,int y){return add(baps,&st.n_bap,&st.t_f1,N,t,c,a,b,d,e,y);}
int bai_execution(int t,int c,int a,int b,int d,int e,int y){return add(bass,&st.n_bae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bai_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bavss,&st.n_bav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bai_accessory(int t,int c,int a,int b,int d,int e,int y){return add(baas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bai_market(int t,int c,int a,int b,int d,int e,int y){return add(bamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bai_report(void){ps("[BAI] Bap: ");pi(st.n_bap);ps(" PCS=");pi(st.t_f1);ps("\nBae: ");pi(st.n_bae);ps(" PCS=");pi(st.t_f2);ps("\nBav: ");pi(st.n_bav);ps(" PCS=");pi(st.t_f3);ps("\nBac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bai_state(void){ps("[BAI] Bap=");pi(st.n_bap);ps(" Bae=");pi(st.n_bae);ps(" Bav=");pi(st.n_bav);ps(" Bac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Baileyia Admin Demo ===\n\n");bai_init();
ps("Baileyia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bai_planning(t,c,810+(i*17),799+(i*14),779+(i*10),761+(i*6),2020+(i%5));}
ps("\nBaileyia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bai_execution(t,c,799+(i*15),788+(i*12),770+(i*8),757+(i*5),2021+(i%4));}
ps("\nBaileyia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bai_evaluation(t,c,791+(i*13),780+(i*10),766+(i*7),755+(i*4),2022+(i%3));}
ps("\nBaileyia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bai_accessory(t,c,783+(i*11),774+(i*9),760+(i*6),750+(i*3),2023+(i%2));}
ps("\nBaileyia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bai_market(t,c,777+(i*9),768+(i*7),755+(i*5),747+(i*3),2024);}
ps("\n");bai_report();bai_state();ps("\n=== Demo Complete ===\n");return 0;}
