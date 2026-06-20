/* metrology_admin: Metrology technology administration (v1.0)
 * Dimensional metrology, mechanical metrology, thermal metrology, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtg_t;
typedef struct{int n_dm,n_mc,n_th,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mtg_state_t;
static mtg_t dms[N],mcs[N-2],ths[N-4],acs[N-6],mks[N-6]; static mtg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtg_init(void){if(init)return -1;st.n_dm=0;st.n_mc=0;st.n_th=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dms[i].active=0;for(int i=0;i<N-2;i++)mcs[i].active=0;for(int i=0;i<N-4;i++)ths[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MTG] Metrology initialized\n");return 0;}
int mtg_dimensional(int t,int c,int a,int b,int d,int e,int y){return add(dms,&st.n_dm,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtg_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtg_thermal(int t,int c,int a,int b,int d,int e,int y){return add(ths,&st.n_th,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtg_report(void){ps("[MTG] Dm: ");pi(st.n_dm);ps(" PCS=");pi(st.t_f1);ps("\nMc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f2);ps("\nTh: ");pi(st.n_th);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mtg_state(void){ps("[MTG] Dm=");pi(st.n_dm);ps(" Mc=");pi(st.n_mc);ps(" Th=");pi(st.n_th);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Metrology Admin Demo ===\n\n");mtg_init();
ps("Dimensional metrology...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtg_dimensional(t,c,423+(i*17),412+(i*14),392+(i*10),374+(i*6),2020+(i%5));}
ps("\nMechanical metrology...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtg_mechanical(t,c,412+(i*15),401+(i*12),383+(i*8),370+(i*5),2021+(i%4));}
ps("\nThermal metrology...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtg_thermal(t,c,404+(i*13),393+(i*10),377+(i*7),366+(i*4),2022+(i%3));}
ps("\nMetrology accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtg_accessory(t,c,396+(i*11),387+(i*9),373+(i*6),363+(i*3),2023+(i%2));}
ps("\nMetrology marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtg_market(t,c,390+(i*9),381+(i*7),368+(i*5),360+(i*3),2024);}
ps("\n");mtg_report();mtg_state();ps("\n=== Demo Complete ===\n");return 0;}
