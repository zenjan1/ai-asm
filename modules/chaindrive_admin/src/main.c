/* chaindrive_admin: Chain drive technology administration (v1.0)
 * Roller chain, silent chain, special chain, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chd_t;
typedef struct{int n_rl,n_si,n_ss,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chd_state_t;
static chd_t rls[N],sis[N-2],sss[N-4],acs[N-6],mks[N-6]; static chd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chd_init(void){if(init)return -1;st.n_rl=0;st.n_si=0;st.n_ss=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rls[i].active=0;for(int i=0;i<N-2;i++)sis[i].active=0;for(int i=0;i<N-4;i++)sss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CHD] Chaindrive initialized\n");return 0;}
int chd_roller(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f1,N,t,c,a,b,d,e,y);}
int chd_silent(int t,int c,int a,int b,int d,int e,int y){return add(sis,&st.n_si,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chd_special(int t,int c,int a,int b,int d,int e,int y){return add(sss,&st.n_ss,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chd_report(void){ps("[CHD] Rl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f1);ps("\nSi: ");pi(st.n_si);ps(" PCS=");pi(st.t_f2);ps("\nSs: ");pi(st.n_ss);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chd_state(void){ps("[CHD] Rl=");pi(st.n_rl);ps(" Si=");pi(st.n_si);ps(" Ss=");pi(st.n_ss);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chain Drive Admin Demo ===\n\n");chd_init();
ps("Roller chains...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chd_roller(t,c,348+(i*17),333+(i*14),313+(i*10),295+(i*6),2020+(i%5));}
ps("\nSilent chains...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chd_silent(t,c,337+(i*15),323+(i*12),305+(i*8),292+(i*5),2021+(i%4));}
ps("\nSpecial chains...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chd_special(t,c,329+(i*13),315+(i*10),299+(i*7),288+(i*4),2022+(i%3));}
ps("\nChain drive accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chd_accessory(t,c,321+(i*11),309+(i*9),295+(i*6),285+(i*3),2023+(i%2));}
ps("\nChain drive marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chd_market(t,c,315+(i*9),304+(i*7),291+(i*5),283+(i*3),2024);}
ps("\n");chd_report();chd_state();ps("\n=== Demo Complete ===\n");return 0;}
