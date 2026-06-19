/* retailtech_admin: Retailtech administration (v1.0)
 * Smart store, omnichannel retail, new retail platform, smart marketing, supply chain management
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rtt_t;
typedef struct{int n_ss,n_or,n_nr,n_sm,n_sc,t_f1,t_f2,t_f3,t_f4,t_f5;} rtt_state_t;
static rtt_t sss[N],ors[N-2],nrs[N-4],sms[N-6],scs[N-6]; static rtt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rtt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rtt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rtt_init(void){if(init)return -1;st.n_ss=0;st.n_or=0;st.n_nr=0;st.n_sm=0;st.n_sc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sss[i].active=0;for(int i=0;i<N-2;i++)ors[i].active=0;for(int i=0;i<N-4;i++)nrs[i].active=0;for(int i=0;i<N-6;i++)sms[i].active=0;for(int i=0;i<N-6;i++)scs[i].active=0;init=1;ps("[RTT] Retailtech initialized\n");return 0;}
int rtt_store(int t,int c,int a,int b,int d,int e,int y){return add(sss,&st.n_ss,&st.t_f1,N,t,c,a,b,d,e,y);}
int rtt_omni(int t,int c,int a,int b,int d,int e,int y){return add(ors,&st.n_or,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rtt_newretail(int t,int c,int a,int b,int d,int e,int y){return add(nrs,&st.n_nr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rtt_marketing(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rtt_supply(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rtt_report(void){ps("[RTT] Store: ");pi(st.n_ss);ps(" IoT=");pi(st.t_f1);ps("\nOmni: ");pi(st.n_or);ps(" O2O=");pi(st.t_f2);ps("\nNR: ");pi(st.n_nr);ps(" Live=");pi(st.t_f3);ps("\nMkt: ");pi(st.n_sm);ps(" Rec=");pi(st.t_f4);ps("\nSCM: ");pi(st.n_sc);ps(" FC=");pi(st.t_f5);ps("\n");}
void rtt_state(void){ps("[RTT] Ss=");pi(st.n_ss);ps(" Or=");pi(st.n_or);ps(" Nr=");pi(st.n_nr);ps(" Sm=");pi(st.n_sm);ps(" Sc=");pi(st.n_sc);ps("\n");}
int main(void){
ps("=== Retailtech Admin Demo ===\n\n");rtt_init();
ps("Smart store...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rtt_store(t,c,142+(i*17),127+(i*14),107+(i*10),90+(i*6),2020+(i%5));}
ps("\nOmnichannel retail...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rtt_omni(t,c,131+(i*15),117+(i*12),99+(i*8),86+(i*5),2021+(i%4));}
ps("\nNew retail platform...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rtt_newretail(t,c,123+(i*13),109+(i*10),93+(i*7),82+(i*4),2022+(i%3));}
ps("\nSmart marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rtt_marketing(t,c,115+(i*11),103+(i*9),89+(i*6),79+(i*3),2023+(i%2));}
ps("\nSupply chain management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rtt_supply(t,c,109+(i*9),98+(i*7),85+(i*5),77+(i*3),2024);}
ps("\n");rtt_report();rtt_state();ps("\n=== Demo Complete ===\n");return 0;}
