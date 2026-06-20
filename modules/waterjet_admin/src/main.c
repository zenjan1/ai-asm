/* waterjet_admin: Waterjet cutting technology administration (v1.0)
 * Pure water cutting, abrasive waterjet cutting, special waterjet, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wjt_t;
typedef struct{int n_pw,n_ab,n_sp,n_ac,n_mt,t_f1,t_f2,t_f3,t_f4,t_f5;} wjt_state_t;
static wjt_t pws[N],abs[N-2],sps[N-4],acs[N-6],mts[N-6]; static wjt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wjt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wjt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WJT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wjt_init(void){if(init)return -1;st.n_pw=0;st.n_ab=0;st.n_sp=0;st.n_ac=0;st.n_mt=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pws[i].active=0;for(int i=0;i<N-2;i++)abs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mts[i].active=0;init=1;ps("[WJT] Waterjet initialized\n");return 0;}
int wjt_purewater(int t,int c,int a,int b,int d,int e,int y){return add(pws,&st.n_pw,&st.t_f1,N,t,c,a,b,d,e,y);}
int wjt_abrasive(int t,int c,int a,int b,int d,int e,int y){return add(abs,&st.n_ab,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wjt_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wjt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wjt_market(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wjt_report(void){ps("[WJT] Pw: ");pi(st.n_pw);ps(" PCS=");pi(st.t_f1);ps("\nAb: ");pi(st.n_ab);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMt: ");pi(st.n_mt);ps(" USD=");pi(st.t_f5);ps("\n");}
void wjt_state(void){ps("[WJT] Pw=");pi(st.n_pw);ps(" Ab=");pi(st.n_ab);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mt=");pi(st.n_mt);ps("\n");}
int main(void){
ps("=== Waterjet Admin Demo ===\n\n");wjt_init();
ps("Pure water cutting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wjt_purewater(t,c,411+(i*17),400+(i*14),380+(i*10),362+(i*6),2020+(i%5));}
ps("\nAbrasive waterjet cutting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wjt_abrasive(t,c,400+(i*15),389+(i*12),371+(i*8),358+(i*5),2021+(i%4));}
ps("\nSpecial waterjet...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wjt_special(t,c,392+(i*13),381+(i*10),365+(i*7),354+(i*4),2022+(i%3));}
ps("\nWaterjet accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wjt_accessory(t,c,384+(i*11),375+(i*9),361+(i*6),351+(i*3),2023+(i%2));}
ps("\nWaterjet marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wjt_market(t,c,378+(i*9),369+(i*7),356+(i*5),348+(i*3),2024);}
ps("\n");wjt_report();wjt_state();ps("\n=== Demo Complete ===\n");return 0;}
