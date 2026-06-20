/* punchingtech_admin: Punching technology administration (v1.0)
 * Plate punching, profile punching, CNC punching, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pnc_t;
typedef struct{int n_pl,n_pf,n_cn,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pnc_state_t;
static pnc_t pls[N],pfs[N-2],cns[N-4],acs[N-6],mks[N-6]; static pnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pnc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pnc_init(void){if(init)return -1;st.n_pl=0;st.n_pf=0;st.n_cn=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)pfs[i].active=0;for(int i=0;i<N-4;i++)cns[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PNC] Punchingtech initialized\n");return 0;}
int pnc_plate(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int pnc_profile(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pnc_cnc(int t,int c,int a,int b,int d,int e,int y){return add(cns,&st.n_cn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pnc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pnc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pnc_report(void){ps("[PNC] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nPf: ");pi(st.n_pf);ps(" PCS=");pi(st.t_f2);ps("\nCn: ");pi(st.n_cn);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pnc_state(void){ps("[PNC] Pl=");pi(st.n_pl);ps(" Pf=");pi(st.n_pf);ps(" Cn=");pi(st.n_cn);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Punching Tech Admin Demo ===\n\n");pnc_init();
ps("Plate punching...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pnc_plate(t,c,283+(i*17),268+(i*14),248+(i*10),230+(i*6),2020+(i%5));}
ps("\nProfile punching...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pnc_profile(t,c,272+(i*15),258+(i*12),240+(i*8),227+(i*5),2021+(i%4));}
ps("\nCNC punching...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pnc_cnc(t,c,264+(i*13),250+(i*10),234+(i*7),223+(i*4),2022+(i%3));}
ps("\nPunching accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnc_accessory(t,c,256+(i*11),244+(i*9),230+(i*6),220+(i*3),2023+(i%2));}
ps("\nPunching marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnc_market(t,c,250+(i*9),239+(i*7),226+(i*5),218+(i*3),2024);}
ps("\n");pnc_report();pnc_state();ps("\n=== Demo Complete ===\n");return 0;}
