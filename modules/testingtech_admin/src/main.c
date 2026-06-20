/* testingtech_admin: Testing technology administration (v1.0)
 * Mechanical, hardness, fatigue testing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tst_t;
typedef struct{int n_mc,n_hd,n_ft,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tst_state_t;
static tst_t mcs[N],hds[N-2],fts[N-4],acs[N-6],mks[N-6]; static tst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tst_init(void){if(init)return -1;st.n_mc=0;st.n_hd=0;st.n_ft=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)hds[i].active=0;for(int i=0;i<N-4;i++)fts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TST] Testingtech initialized\n");return 0;}
int tst_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int tst_hardness(int t,int c,int a,int b,int d,int e,int y){return add(hds,&st.n_hd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tst_fatigue(int t,int c,int a,int b,int d,int e,int y){return add(fts,&st.n_ft,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tst_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tst_report(void){ps("[TST] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nHd: ");pi(st.n_hd);ps(" PCS=");pi(st.t_f2);ps("\nFt: ");pi(st.n_ft);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tst_state(void){ps("[TST] Mc=");pi(st.n_mc);ps(" Hd=");pi(st.n_hd);ps(" Ft=");pi(st.n_ft);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Testing Tech Admin Demo ===\n\n");tst_init();
ps("Mechanical testing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tst_mechanical(t,c,288+(i*17),273+(i*14),253+(i*10),235+(i*6),2020+(i%5));}
ps("\nHardness testing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tst_hardness(t,c,277+(i*15),263+(i*12),245+(i*8),232+(i*5),2021+(i%4));}
ps("\nFatigue testing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tst_fatigue(t,c,269+(i*13),255+(i*10),239+(i*7),228+(i*4),2022+(i%3));}
ps("\nTesting accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tst_accessory(t,c,261+(i*11),249+(i*9),235+(i*6),225+(i*3),2023+(i%2));}
ps("\nTesting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tst_market(t,c,255+(i*9),244+(i*7),231+(i*5),223+(i*3),2024);}
ps("\n");tst_report();tst_state();ps("\n=== Demo Complete ===\n");return 0;}
