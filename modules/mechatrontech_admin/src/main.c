/* mechatrontech_admin: Mechatronics technology administration (v1.0)
 * Mechanical systems, electronic systems, control systems, integration, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mch_t;
typedef struct{int n_mc,n_el,n_ct,n_ig,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mch_state_t;
static mch_t mcs[N],els[N-2],cts[N-4],igs[N-6],mks[N-6]; static mch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mch_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mch_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mch_init(void){if(init)return -1;st.n_mc=0;st.n_el=0;st.n_ct=0;st.n_ig=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)els[i].active=0;for(int i=0;i<N-4;i++)cts[i].active=0;for(int i=0;i<N-6;i++)igs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MCH] Mechatrontech initialized\n");return 0;}
int mch_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int mch_electronic(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mch_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mch_integration(int t,int c,int a,int b,int d,int e,int y){return add(igs,&st.n_ig,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mch_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mch_report(void){ps("[MCH] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nEl: ");pi(st.n_el);ps(" PCS=");pi(st.t_f2);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f3);ps("\nIg: ");pi(st.n_ig);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mch_state(void){ps("[MCH] Mc=");pi(st.n_mc);ps(" El=");pi(st.n_el);ps(" Ct=");pi(st.n_ct);ps(" Ig=");pi(st.n_ig);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mechatronics Tech Admin Demo ===\n\n");mch_init();
ps("Mechanical systems...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mch_mechanical(t,c,296+(i*17),281+(i*14),261+(i*10),243+(i*6),2020+(i%5));}
ps("\nElectronic systems...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mch_electronic(t,c,285+(i*15),271+(i*12),253+(i*8),240+(i*5),2021+(i%4));}
ps("\nControl systems...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mch_control(t,c,277+(i*13),263+(i*10),247+(i*7),236+(i*4),2022+(i%3));}
ps("\nIntegration modules...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mch_integration(t,c,269+(i*11),257+(i*9),243+(i*6),233+(i*3),2023+(i%2));}
ps("\nTech marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mch_market(t,c,263+(i*9),252+(i*7),239+(i*5),231+(i*3),2024);}
ps("\n");mch_report();mch_state();ps("\n=== Demo Complete ===\n");return 0;}
