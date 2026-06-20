/* fastenertech_admin: Fastener technology administration (v1.0)
 * Bolts, nuts, screws, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fst_t;
typedef struct{int n_bt,n_nt,n_sc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fst_state_t;
static fst_t bts[N],nts[N-2],scs[N-4],acs[N-6],mks[N-6]; static fst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fst_init(void){if(init)return -1;st.n_bt=0;st.n_nt=0;st.n_sc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bts[i].active=0;for(int i=0;i<N-2;i++)nts[i].active=0;for(int i=0;i<N-4;i++)scs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FST] Fastenertech initialized\n");return 0;}
int fst_bolt(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f1,N,t,c,a,b,d,e,y);}
int fst_nut(int t,int c,int a,int b,int d,int e,int y){return add(nts,&st.n_nt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fst_screw(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fst_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fst_report(void){ps("[FST] Bt: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f1);ps("\nNt: ");pi(st.n_nt);ps(" PCS=");pi(st.t_f2);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fst_state(void){ps("[FST] Bt=");pi(st.n_bt);ps(" Nt=");pi(st.n_nt);ps(" Sc=");pi(st.n_sc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fastener Tech Admin Demo ===\n\n");fst_init();
ps("Bolts...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fst_bolt(t,c,267+(i*17),252+(i*14),232+(i*10),214+(i*6),2020+(i%5));}
ps("\nNuts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fst_nut(t,c,256+(i*15),242+(i*12),224+(i*8),211+(i*5),2021+(i%4));}
ps("\nScrews...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fst_screw(t,c,248+(i*13),234+(i*10),218+(i*7),207+(i*4),2022+(i%3));}
ps("\nFastener accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fst_accessory(t,c,240+(i*11),228+(i*9),214+(i*6),204+(i*3),2023+(i%2));}
ps("\nFastener marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fst_market(t,c,234+(i*9),223+(i*7),210+(i*5),202+(i*3),2024);}
ps("\n");fst_report();fst_state();ps("\n=== Demo Complete ===\n");return 0;}
