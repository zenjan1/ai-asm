/* boronizingtech_admin: Boronizing technology administration (v1.0)
 * Solid boronizing, liquid boronizing, gas boronizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brz_t;
typedef struct{int n_so,n_lq,n_gs,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brz_state_t;
static brz_t sos[N],lqs[N-2],gss[N-4],acs[N-6],mks[N-6]; static brz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brz_init(void){if(init)return -1;st.n_so=0;st.n_lq=0;st.n_gs=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sos[i].active=0;for(int i=0;i<N-2;i++)lqs[i].active=0;for(int i=0;i<N-4;i++)gss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BRZ] Boronizingtech initialized\n");return 0;}
int brz_solid(int t,int c,int a,int b,int d,int e,int y){return add(sos,&st.n_so,&st.t_f1,N,t,c,a,b,d,e,y);}
int brz_liquid(int t,int c,int a,int b,int d,int e,int y){return add(lqs,&st.n_lq,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brz_gas(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brz_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brz_report(void){ps("[BRZ] So: ");pi(st.n_so);ps(" PCS=");pi(st.t_f1);ps("\nLq: ");pi(st.n_lq);ps(" PCS=");pi(st.t_f2);ps("\nGs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brz_state(void){ps("[BRZ] So=");pi(st.n_so);ps(" Lq=");pi(st.n_lq);ps(" Gs=");pi(st.n_gs);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Boronizing Tech Admin Demo ===\n\n");brz_init();
ps("Solid boronizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brz_solid(t,c,331+(i*17),316+(i*14),296+(i*10),278+(i*6),2020+(i%5));}
ps("\nLiquid boronizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brz_liquid(t,c,320+(i*15),306+(i*12),288+(i*8),275+(i*5),2021+(i%4));}
ps("\nGas boronizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brz_gas(t,c,312+(i*13),298+(i*10),282+(i*7),271+(i*4),2022+(i%3));}
ps("\nBoronizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brz_accessory(t,c,304+(i*11),292+(i*9),278+(i*6),268+(i*3),2023+(i%2));}
ps("\nBoronizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brz_market(t,c,298+(i*9),287+(i*7),274+(i*5),266+(i*3),2024);}
ps("\n");brz_report();brz_state();ps("\n=== Demo Complete ===\n");return 0;}
