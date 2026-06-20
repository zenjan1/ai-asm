/* daisy_admin: Daisy management technology administration (v1.0)
 * Daisy planning, daisy execution, daisy evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dai_t;
typedef struct{int n_dp,n_de,n_dv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dai_state_t;
static dai_t dps[N],des[N-2],dvs[N-4],acs[N-6],mks[N-6]; static dai_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dai_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dai_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dai_init(void){if(init)return -1;st.n_dp=0;st.n_de=0;st.n_dv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dps[i].active=0;for(int i=0;i<N-2;i++)des[i].active=0;for(int i=0;i<N-4;i++)dvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DAI] Daisy initialized\n");return 0;}
int dai_planning(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dai_execution(int t,int c,int a,int b,int d,int e,int y){return add(des,&st.n_de,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dai_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dvs,&st.n_dv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dai_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dai_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dai_report(void){ps("[DAI] Dp: ");pi(st.n_dp);ps(" PCS=");pi(st.t_f1);ps("\nDe: ");pi(st.n_de);ps(" PCS=");pi(st.t_f2);ps("\nDv: ");pi(st.n_dv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dai_state(void){ps("[DAI] Dp=");pi(st.n_dp);ps(" De=");pi(st.n_de);ps(" Dv=");pi(st.n_dv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Daisy Admin Demo ===\n\n");dai_init();
ps("Daisy planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dai_planning(t,c,545+(i*17),534+(i*14),514+(i*10),496+(i*6),2020+(i%5));}
ps("\nDaisy execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dai_execution(t,c,534+(i*15),523+(i*12),505+(i*8),492+(i*5),2021+(i%4));}
ps("\nDaisy evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dai_evaluation(t,c,526+(i*13),515+(i*10),499+(i*7),488+(i*4),2022+(i%3));}
ps("\nDaisy accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dai_accessory(t,c,518+(i*11),509+(i*9),495+(i*6),485+(i*3),2023+(i%2));}
ps("\nDaisy marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dai_market(t,c,512+(i*9),503+(i*7),490+(i*5),482+(i*3),2024);}
ps("\n");dai_report();dai_state();ps("\n=== Demo Complete ===\n");return 0;}
