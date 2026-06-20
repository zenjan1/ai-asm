/* shafttech_admin: Shaft technology administration (v1.0)
 * Drive shafts, support shafts, special shafts, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sft_t;
typedef struct{int n_dr,n_su,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sft_state_t;
static sft_t drs[N],sus[N-2],sps[N-4],acs[N-6],mks[N-6]; static sft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SFT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sft_init(void){if(init)return -1;st.n_dr=0;st.n_su=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drs[i].active=0;for(int i=0;i<N-2;i++)sus[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SFT] Shafttech initialized\n");return 0;}
int sft_drive(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f1,N,t,c,a,b,d,e,y);}
int sft_support(int t,int c,int a,int b,int d,int e,int y){return add(sus,&st.n_su,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sft_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sft_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sft_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sft_report(void){ps("[SFT] Dr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f1);ps("\nSu: ");pi(st.n_su);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sft_state(void){ps("[SFT] Dr=");pi(st.n_dr);ps(" Su=");pi(st.n_su);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Shaft Tech Admin Demo ===\n\n");sft_init();
ps("Drive shafts...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sft_drive(t,c,346+(i*17),331+(i*14),311+(i*10),293+(i*6),2020+(i%5));}
ps("\nSupport shafts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sft_support(t,c,335+(i*15),321+(i*12),303+(i*8),290+(i*5),2021+(i%4));}
ps("\nSpecial shafts...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sft_special(t,c,327+(i*13),313+(i*10),297+(i*7),286+(i*4),2022+(i%3));}
ps("\nShaft accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sft_accessory(t,c,319+(i*11),307+(i*9),293+(i*6),283+(i*3),2023+(i%2));}
ps("\nShaft marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sft_market(t,c,313+(i*9),302+(i*7),289+(i*5),281+(i*3),2024);}
ps("\n");sft_report();sft_state();ps("\n=== Demo Complete ===\n");return 0;}
