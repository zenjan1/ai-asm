/* quenchtech_admin: Tempering technology administration (v1.0)
 * Low-temperature tempering, medium-temperature, high-temperature, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} qnc_t;
typedef struct{int n_lt,n_mt,n_ht,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} qnc_state_t;
static qnc_t lts[N],mts[N-2],hts[N-4],acs[N-6],mks[N-6]; static qnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qnc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;qnc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[QNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int qnc_init(void){if(init)return -1;st.n_lt=0;st.n_mt=0;st.n_ht=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lts[i].active=0;for(int i=0;i<N-2;i++)mts[i].active=0;for(int i=0;i<N-4;i++)hts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[QNC] Quenchtech initialized\n");return 0;}
int qnc_low(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f1,N,t,c,a,b,d,e,y);}
int qnc_medium(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int qnc_high(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int qnc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int qnc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void qnc_report(void){ps("[QNC] Lt: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f1);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f2);ps("\nHt: ");pi(st.n_ht);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void qnc_state(void){ps("[QNC] Lt=");pi(st.n_lt);ps(" Mt=");pi(st.n_mt);ps(" Ht=");pi(st.n_ht);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tempering Tech Admin Demo ===\n\n");qnc_init();
ps("Low-temperature tempering...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;qnc_low(t,c,327+(i*17),312+(i*14),292+(i*10),274+(i*6),2020+(i%5));}
ps("\nMedium-temperature tempering...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;qnc_medium(t,c,316+(i*15),302+(i*12),284+(i*8),271+(i*5),2021+(i%4));}
ps("\nHigh-temperature tempering...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;qnc_high(t,c,308+(i*13),294+(i*10),278+(i*7),267+(i*4),2022+(i%3));}
ps("\nTempering accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qnc_accessory(t,c,300+(i*11),288+(i*9),274+(i*6),264+(i*3),2023+(i%2));}
ps("\nTempering marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qnc_market(t,c,294+(i*9),283+(i*7),270+(i*5),262+(i*3),2024);}
ps("\n");qnc_report();qnc_state();ps("\n=== Demo Complete ===\n");return 0;}
