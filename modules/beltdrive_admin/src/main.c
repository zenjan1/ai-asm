/* beltdrive_admin: Belt drive technology administration (v1.0)
 * Flat belt drive, V-belt drive, synchronous belt drive, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bld_t;
typedef struct{int n_fl,n_vb,n_sy,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bld_state_t;
static bld_t fls[N],vbs[N-2],sys[N-4],acs[N-6],mks[N-6]; static bld_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bld_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bld_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bld_init(void){if(init)return -1;st.n_fl=0;st.n_vb=0;st.n_sy=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fls[i].active=0;for(int i=0;i<N-2;i++)vbs[i].active=0;for(int i=0;i<N-4;i++)sys[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLD] Beltdrive initialized\n");return 0;}
int bld_flat(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f1,N,t,c,a,b,d,e,y);}
int bld_vbelt(int t,int c,int a,int b,int d,int e,int y){return add(vbs,&st.n_vb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bld_sync(int t,int c,int a,int b,int d,int e,int y){return add(sys,&st.n_sy,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bld_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bld_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bld_report(void){ps("[BLD] Fl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f1);ps("\nVb: ");pi(st.n_vb);ps(" PCS=");pi(st.t_f2);ps("\nSy: ");pi(st.n_sy);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bld_state(void){ps("[BLD] Fl=");pi(st.n_fl);ps(" Vb=");pi(st.n_vb);ps(" Sy=");pi(st.n_sy);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Belt Drive Admin Demo ===\n\n");bld_init();
ps("Flat belt drive...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bld_flat(t,c,349+(i*17),334+(i*14),314+(i*10),296+(i*6),2020+(i%5));}
ps("\nV-belt drive...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bld_vbelt(t,c,338+(i*15),324+(i*12),306+(i*8),293+(i*5),2021+(i%4));}
ps("\nSynchronous belt drive...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bld_sync(t,c,330+(i*13),316+(i*10),300+(i*7),289+(i*4),2022+(i%3));}
ps("\nBelt drive accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bld_accessory(t,c,322+(i*11),310+(i*9),296+(i*6),286+(i*3),2023+(i%2));}
ps("\nBelt drive marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bld_market(t,c,316+(i*9),305+(i*7),292+(i*5),284+(i*3),2024);}
ps("\n");bld_report();bld_state();ps("\n=== Demo Complete ===\n");return 0;}
