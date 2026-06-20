/* weld_admin: Welding technology administration (v1.0)
 * Arc welding, resistance welding, special welding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wld_t;
typedef struct{int n_arc,n_rs,n_sw,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wld_state_t;
static wld_t ars[N],rss[N-2],sws[N-4],acs[N-6],mks[N-6]; static wld_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wld_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wld_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wld_init(void){if(init)return -1;st.n_arc=0;st.n_rs=0;st.n_sw=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ars[i].active=0;for(int i=0;i<N-2;i++)rss[i].active=0;for(int i=0;i<N-4;i++)sws[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WLD] Weld initialized\n");return 0;}
int wld_arc(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_arc,&st.t_f1,N,t,c,a,b,d,e,y);}
int wld_resistance(int t,int c,int a,int b,int d,int e,int y){return add(rss,&st.n_rs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wld_special(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wld_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wld_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wld_report(void){ps("[WLD] Ar: ");pi(st.n_arc);ps(" PCS=");pi(st.t_f1);ps("\nRs: ");pi(st.n_rs);ps(" PCS=");pi(st.t_f2);ps("\nSw: ");pi(st.n_sw);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wld_state(void){ps("[WLD] Ar=");pi(st.n_arc);ps(" Rs=");pi(st.n_rs);ps(" Sw=");pi(st.n_sw);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Weld Admin Demo ===\n\n");wld_init();
ps("Arc welding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wld_arc(t,c,380+(i*17),365+(i*14),345+(i*10),327+(i*6),2020+(i%5));}
ps("\nResistance welding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wld_resistance(t,c,369+(i*15),355+(i*12),337+(i*8),324+(i*5),2021+(i%4));}
ps("\nSpecial welding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wld_special(t,c,361+(i*13),347+(i*10),331+(i*7),320+(i*4),2022+(i%3));}
ps("\nWelding accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wld_accessory(t,c,353+(i*11),341+(i*9),327+(i*6),317+(i*3),2023+(i%2));}
ps("\nWelding marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wld_market(t,c,347+(i*9),336+(i*7),323+(i*5),315+(i*3),2024);}
ps("\n");wld_report();wld_state();ps("\n=== Demo Complete ===\n");return 0;}
