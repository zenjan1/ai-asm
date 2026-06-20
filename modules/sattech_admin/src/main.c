/* sattech_admin: Satellite and launch technology administration (v1.0)
 * Satellite manufacturing, launch vehicles, spacecraft, ground systems, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} satt_t;
typedef struct{int n_sat,n_lv,n_sc,n_gs,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} satt_state_t;
static satt_t sats[N],lvs[N-2],scs[N-4],gss[N-6],mks[N-6]; static satt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(satt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;satt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SATT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int satt_init(void){if(init)return -1;st.n_sat=0;st.n_lv=0;st.n_sc=0;st.n_gs=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sats[i].active=0;for(int i=0;i<N-2;i++)lvs[i].active=0;for(int i=0;i<N-4;i++)scs[i].active=0;for(int i=0;i<N-6;i++)gss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SATT] Sattech initialized\n");return 0;}
int satt_satellite(int t,int c,int a,int b,int d,int e,int y){return add(sats,&st.n_sat,&st.t_f1,N,t,c,a,b,d,e,y);}
int satt_launch(int t,int c,int a,int b,int d,int e,int y){return add(lvs,&st.n_lv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int satt_spacecraft(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int satt_ground(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int satt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void satt_report(void){ps("[SATT] Sat: ");pi(st.n_sat);ps(" PCS=");pi(st.t_f1);ps("\nLv: ");pi(st.n_lv);ps(" PCS=");pi(st.t_f2);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f3);ps("\nGs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void satt_state(void){ps("[SATT] Sat=");pi(st.n_sat);ps(" Lv=");pi(st.n_lv);ps(" Sc=");pi(st.n_sc);ps(" Gs=");pi(st.n_gs);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Satellite Tech Admin Demo ===\n\n");satt_init();
ps("Satellite manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;satt_satellite(t,c,223+(i*17),208+(i*14),188+(i*10),170+(i*6),2020+(i%5));}
ps("\nLaunch vehicles...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;satt_launch(t,c,212+(i*15),198+(i*12),180+(i*8),167+(i*5),2021+(i%4));}
ps("\nSpacecraft...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;satt_spacecraft(t,c,204+(i*13),190+(i*10),174+(i*7),163+(i*4),2022+(i%3));}
ps("\nGround systems...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;satt_ground(t,c,196+(i*11),184+(i*9),170+(i*6),160+(i*3),2023+(i%2));}
ps("\nSpace marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;satt_market(t,c,190+(i*9),179+(i*7),166+(i*5),158+(i*3),2024);}
ps("\n");satt_report();satt_state();ps("\n=== Demo Complete ===\n");return 0;}
