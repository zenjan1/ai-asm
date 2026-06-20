/* boilertech_admin: Boiler technology administration (v1.0)
 * Steam boilers, hot water boilers, special boilers, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blr_t;
typedef struct{int n_st,n_hw,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blr_state_t;
static blr_t sts[N],hws[N-2],sps[N-4],acs[N-6],mks[N-6]; static blr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blr_init(void){if(init)return -1;st.n_st=0;st.n_hw=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)hws[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLR] Boilertech initialized\n");return 0;}
int blr_steam(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int blr_hotwater(int t,int c,int a,int b,int d,int e,int y){return add(hws,&st.n_hw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blr_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blr_report(void){ps("[BLR] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nHw: ");pi(st.n_hw);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blr_state(void){ps("[BLR] St=");pi(st.n_st);ps(" Hw=");pi(st.n_hw);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Boiler Tech Admin Demo ===\n\n");blr_init();
ps("Steam boilers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blr_steam(t,c,260+(i*17),245+(i*14),225+(i*10),207+(i*6),2020+(i%5));}
ps("\nHot water boilers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blr_hotwater(t,c,249+(i*15),235+(i*12),217+(i*8),204+(i*5),2021+(i%4));}
ps("\nSpecial boilers...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blr_special(t,c,241+(i*13),227+(i*10),211+(i*7),200+(i*4),2022+(i%3));}
ps("\nBoiler accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blr_accessory(t,c,233+(i*11),221+(i*9),207+(i*6),197+(i*3),2023+(i%2));}
ps("\nBoiler marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blr_market(t,c,227+(i*9),216+(i*7),203+(i*5),195+(i*3),2024);}
ps("\n");blr_report();blr_state();ps("\n=== Demo Complete ===\n");return 0;}
