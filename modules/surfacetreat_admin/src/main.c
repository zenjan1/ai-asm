/* surfacetreat_admin: Surface treatment technology administration (v1.0)
 * Mechanical treatment, chemical treatment, electrochemical treatment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sft_t;
typedef struct{int n_mc,n_ch,n_el,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sft_state_t;
static sft_t mcs[N],chs[N-2],els[N-4],acs[N-6],mks[N-6]; static sft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SFT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sft_init(void){if(init)return -1;st.n_mc=0;st.n_ch=0;st.n_el=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)els[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SFT] Surfacetreat initialized\n");return 0;}
int sft_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int sft_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sft_electrochemical(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sft_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sft_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sft_report(void){ps("[SFT] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f2);ps("\nEl: ");pi(st.n_el);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sft_state(void){ps("[SFT] Mc=");pi(st.n_mc);ps(" Ch=");pi(st.n_ch);ps(" El=");pi(st.n_el);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Surface Treatment Admin Demo ===\n\n");sft_init();
ps("Mechanical treatment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sft_mechanical(t,c,324+(i*17),309+(i*14),289+(i*10),271+(i*6),2020+(i%5));}
ps("\nChemical treatment...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sft_chemical(t,c,313+(i*15),299+(i*12),281+(i*8),268+(i*5),2021+(i%4));}
ps("\nElectrochemical treatment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sft_electrochemical(t,c,305+(i*13),291+(i*10),275+(i*7),264+(i*4),2022+(i%3));}
ps("\nTreatment accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sft_accessory(t,c,297+(i*11),285+(i*9),271+(i*6),261+(i*3),2023+(i%2));}
ps("\nTreatment marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sft_market(t,c,291+(i*9),280+(i*7),267+(i*5),259+(i*3),2024);}
ps("\n");sft_report();sft_state();ps("\n=== Demo Complete ===\n");return 0;}
