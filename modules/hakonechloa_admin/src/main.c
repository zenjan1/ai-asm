/* hakonechloa_admin: Hakonechloa management technology administration (v1.0)
 * Hakonechloa planning, hakonechloa execution, hakonechloa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hk_t;
typedef struct{int n_hkp,n_hke,n_hkv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hk_state_t;
static hk_t hkps[N],hkss[N-2],hkvss[N-4],hkas[N-6],hkmks[N-6]; static hk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HKN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hkn_init(void){if(init)return -1;st.n_hkp=0;st.n_hke=0;st.n_hkv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hkps[i].active=0;for(int i=0;i<N-2;i++)hkss[i].active=0;for(int i=0;i<N-4;i++)hkvss[i].active=0;for(int i=0;i<N-6;i++)hkas[i].active=0;for(int i=0;i<N-6;i++)hkmks[i].active=0;init=1;ps("[HKN] Hakonechloa initialized\n");return 0;}
int hkn_planning(int t,int c,int a,int b,int d,int e,int y){return add(hkps,&st.n_hkp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hkn_execution(int t,int c,int a,int b,int d,int e,int y){return add(hkss,&st.n_hke,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hkn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hkvss,&st.n_hkv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hkn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hkas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hkn_market(int t,int c,int a,int b,int d,int e,int y){return add(hkmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hkn_report(void){ps("[HKN] Hkp: ");pi(st.n_hkp);ps(" PCS=");pi(st.t_f1);ps("\nHke: ");pi(st.n_hke);ps(" PCS=");pi(st.t_f2);ps("\nHkv: ");pi(st.n_hkv);ps(" PCS=");pi(st.t_f3);ps("\nHkc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hkn_state(void){ps("[HKN] Hkp=");pi(st.n_hkp);ps(" Hke=");pi(st.n_hke);ps(" Hkv=");pi(st.n_hkv);ps(" Hkc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hakonechloa Admin Demo ===\n\n");hkn_init();
ps("Hakonechloa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hkn_planning(t,c,780+(i*17),769+(i*14),749+(i*10),731+(i*6),2020+(i%5));}
ps("\nHakonechloa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hkn_execution(t,c,769+(i*15),758+(i*12),740+(i*8),727+(i*5),2021+(i%4));}
ps("\nHakonechloa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hkn_evaluation(t,c,761+(i*13),750+(i*10),736+(i*7),725+(i*4),2022+(i%3));}
ps("\nHakonechloa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hkn_accessory(t,c,753+(i*11),744+(i*9),730+(i*6),720+(i*3),2023+(i%2));}
ps("\nHakonechloa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hkn_market(t,c,747+(i*9),738+(i*7),725+(i*5),717+(i*3),2024);}
ps("\n");hkn_report();hkn_state();ps("\n=== Demo Complete ===\n");return 0;}
