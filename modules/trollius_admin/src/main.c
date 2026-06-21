/* trollius_admin: Trollius management technology administration (v1.0)
 * Trollius planning, trollius execution, trollius evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tll_t;
typedef struct{int n_tlp,n_tle,n_tlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tll_state_t;
static tll_t tllps[N],tllss[N-2],tllvss[N-4],tllas[N-6],tllmks[N-6]; static tll_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tll_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tll_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TLL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tll_init(void){if(init)return -1;st.n_tlp=0;st.n_tle=0;st.n_tlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tllps[i].active=0;for(int i=0;i<N-2;i++)tllss[i].active=0;for(int i=0;i<N-4;i++)tllvss[i].active=0;for(int i=0;i<N-6;i++)tllas[i].active=0;for(int i=0;i<N-6;i++)tllmks[i].active=0;init=1;ps("[TLL] Trollius initialized\n");return 0;}
int tll_planning(int t,int c,int a,int b,int d,int e,int y){return add(tllps,&st.n_tlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tll_execution(int t,int c,int a,int b,int d,int e,int y){return add(tllss,&st.n_tle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tll_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tllvss,&st.n_tlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tll_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tllas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tll_market(int t,int c,int a,int b,int d,int e,int y){return add(tllmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tll_report(void){ps("[TLL] Tlp: ");pi(st.n_tlp);ps(" PCS=");pi(st.t_f1);ps("\nTle: ");pi(st.n_tle);ps(" PCS=");pi(st.t_f2);ps("\nTlv: ");pi(st.n_tlv);ps(" PCS=");pi(st.t_f3);ps("\nTlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tll_state(void){ps("[TLL] Tlp=");pi(st.n_tlp);ps(" Tle=");pi(st.n_tle);ps(" Tlv=");pi(st.n_tlv);ps(" Tlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Trollius Admin Demo ===\n\n");tll_init();
ps("Trollius planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tll_planning(t,c,868+(i*17),857+(i*14),837+(i*10),819+(i*6),2020+(i%5));}
ps("\nTrollius execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tll_execution(t,c,857+(i*15),846+(i*12),828+(i*8),815+(i*5),2021+(i%4));}
ps("\nTrollius evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tll_evaluation(t,c,849+(i*13),838+(i*10),822+(i*7),811+(i*4),2022+(i%3));}
ps("\nTrollius accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tll_accessory(t,c,841+(i*11),832+(i*9),818+(i*6),808+(i*3),2023+(i%2));}
ps("\nTrollius marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tll_market(t,c,835+(i*9),826+(i*7),813+(i*5),805+(i*3),2024);}
ps("\n");tll_report();tll_state();ps("\n=== Demo Complete ===\n");return 0;}
