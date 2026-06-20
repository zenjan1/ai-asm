/* nuttech_admin: Nut technology administration (v1.0)
 * Hex nut, lock nut, special nut, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ntt_t;
typedef struct{int n_hx,n_lk,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ntt_state_t;
static ntt_t hxs[N],lks[N-2],sps[N-4],acs[N-6],mks[N-6]; static ntt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ntt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ntt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ntt_init(void){if(init)return -1;st.n_hx=0;st.n_lk=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hxs[i].active=0;for(int i=0;i<N-2;i++)lks[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NTT] Nuttech initialized\n");return 0;}
int ntt_hex(int t,int c,int a,int b,int d,int e,int y){return add(hxs,&st.n_hx,&st.t_f1,N,t,c,a,b,d,e,y);}
int ntt_lock(int t,int c,int a,int b,int d,int e,int y){return add(lks,&st.n_lk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ntt_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ntt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ntt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ntt_report(void){ps("[NTT] Hx: ");pi(st.n_hx);ps(" PCS=");pi(st.t_f1);ps("\nLk: ");pi(st.n_lk);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ntt_state(void){ps("[NTT] Hx=");pi(st.n_hx);ps(" Lk=");pi(st.n_lk);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nut Tech Admin Demo ===\n\n");ntt_init();
ps("Hex nuts...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ntt_hex(t,c,362+(i*17),347+(i*14),327+(i*10),309+(i*6),2020+(i%5));}
ps("\nLock nuts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ntt_lock(t,c,351+(i*15),337+(i*12),319+(i*8),306+(i*5),2021+(i%4));}
ps("\nSpecial nuts...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ntt_special(t,c,343+(i*13),329+(i*10),313+(i*7),302+(i*4),2022+(i%3));}
ps("\nNut accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntt_accessory(t,c,335+(i*11),323+(i*9),309+(i*6),299+(i*3),2023+(i%2));}
ps("\nNut marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntt_market(t,c,329+(i*9),318+(i*7),305+(i*5),297+(i*3),2024);}
ps("\n");ntt_report();ntt_state();ps("\n=== Demo Complete ===\n");return 0;}
