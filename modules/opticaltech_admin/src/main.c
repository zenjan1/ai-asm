/* opticaltech_admin: Optical technology administration (v1.0)
 * Optical elements, optical instruments, laser equipment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} opt_t;
typedef struct{int n_el,n_in,n_ls,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} opt_state_t;
static opt_t els[N],ins[N-2],lss[N-4],acs[N-6],mks[N-6]; static opt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(opt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;opt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int opt_init(void){if(init)return -1;st.n_el=0;st.n_in=0;st.n_ls=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)els[i].active=0;for(int i=0;i<N-2;i++)ins[i].active=0;for(int i=0;i<N-4;i++)lss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OPT] Opticaltech initialized\n");return 0;}
int opt_element(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f1,N,t,c,a,b,d,e,y);}
int opt_instrument(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int opt_laser(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int opt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int opt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void opt_report(void){ps("[OPT] El: ");pi(st.n_el);ps(" PCS=");pi(st.t_f1);ps("\nIn: ");pi(st.n_in);ps(" PCS=");pi(st.t_f2);ps("\nLs: ");pi(st.n_ls);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void opt_state(void){ps("[OPT] El=");pi(st.n_el);ps(" In=");pi(st.n_in);ps(" Ls=");pi(st.n_ls);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Optical Tech Admin Demo ===\n\n");opt_init();
ps("Optical elements...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;opt_element(t,c,308+(i*17),293+(i*14),273+(i*10),255+(i*6),2020+(i%5));}
ps("\nOptical instruments...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;opt_instrument(t,c,297+(i*15),283+(i*12),265+(i*8),252+(i*5),2021+(i%4));}
ps("\nLaser equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;opt_laser(t,c,289+(i*13),275+(i*10),259+(i*7),248+(i*4),2022+(i%3));}
ps("\nOptical accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opt_accessory(t,c,281+(i*11),269+(i*9),255+(i*6),245+(i*3),2023+(i%2));}
ps("\nOptical marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opt_market(t,c,275+(i*9),264+(i*7),251+(i*5),243+(i*3),2024);}
ps("\n");opt_report();opt_state();ps("\n=== Demo Complete ===\n");return 0;}
