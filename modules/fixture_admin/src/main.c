/* fixture_admin: Fixture technology administration (v1.0)
 * Lathe fixture, milling fixture, drilling fixture, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fxt_t;
typedef struct{int n_lt,n_ml,n_dr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fxt_state_t;
static fxt_t lts[N],mls[N-2],drs[N-4],acs[N-6],mks[N-6]; static fxt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fxt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fxt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fxt_init(void){if(init)return -1;st.n_lt=0;st.n_ml=0;st.n_dr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lts[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)drs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FXT] Fixture initialized\n");return 0;}
int fxt_lathe(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f1,N,t,c,a,b,d,e,y);}
int fxt_milling(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fxt_drilling(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fxt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fxt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fxt_report(void){ps("[FXT] Lt: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nDr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fxt_state(void){ps("[FXT] Lt=");pi(st.n_lt);ps(" Ml=");pi(st.n_ml);ps(" Dr=");pi(st.n_dr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fixture Admin Demo ===\n\n");fxt_init();
ps("Lathe fixtures...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fxt_lathe(t,c,388+(i*17),373+(i*14),353+(i*10),335+(i*6),2020+(i%5));}
ps("\nMilling fixtures...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fxt_milling(t,c,377+(i*15),363+(i*12),345+(i*8),332+(i*5),2021+(i%4));}
ps("\nDrilling fixtures...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fxt_drilling(t,c,369+(i*13),355+(i*10),339+(i*7),328+(i*4),2022+(i%3));}
ps("\nFixture accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxt_accessory(t,c,361+(i*11),349+(i*9),335+(i*6),325+(i*3),2023+(i%2));}
ps("\nFixture marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxt_market(t,c,355+(i*9),344+(i*7),331+(i*5),323+(i*3),2024);}
ps("\n");fxt_report();fxt_state();ps("\n=== Demo Complete ===\n");return 0;}
