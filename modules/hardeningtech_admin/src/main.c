/* hardeningtech_admin: Hardening technology administration (v1.0)
 * Full hardening, surface hardening, local hardening, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hdt_t;
typedef struct{int n_fl,n_sf,n_lc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hdt_state_t;
static hdt_t fls[N],sfs[N-2],lcs[N-4],acs[N-6],mks[N-6]; static hdt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hdt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hdt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hdt_init(void){if(init)return -1;st.n_fl=0;st.n_sf=0;st.n_lc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fls[i].active=0;for(int i=0;i<N-2;i++)sfs[i].active=0;for(int i=0;i<N-4;i++)lcs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HDT] Hardeningtech initialized\n");return 0;}
int hdt_full(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f1,N,t,c,a,b,d,e,y);}
int hdt_surface(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hdt_local(int t,int c,int a,int b,int d,int e,int y){return add(lcs,&st.n_lc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hdt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hdt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hdt_report(void){ps("[HDT] Fl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f1);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f2);ps("\nLc: ");pi(st.n_lc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hdt_state(void){ps("[HDT] Fl=");pi(st.n_fl);ps(" Sf=");pi(st.n_sf);ps(" Lc=");pi(st.n_lc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hardening Tech Admin Demo ===\n\n");hdt_init();
ps("Full hardening...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hdt_full(t,c,325+(i*17),310+(i*14),290+(i*10),272+(i*6),2020+(i%5));}
ps("\nSurface hardening...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hdt_surface(t,c,314+(i*15),300+(i*12),282+(i*8),269+(i*5),2021+(i%4));}
ps("\nLocal hardening...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hdt_local(t,c,306+(i*13),292+(i*10),276+(i*7),265+(i*4),2022+(i%3));}
ps("\nHardening accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hdt_accessory(t,c,298+(i*11),286+(i*9),272+(i*6),262+(i*3),2023+(i%2));}
ps("\nHardening marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hdt_market(t,c,292+(i*9),281+(i*7),268+(i*5),260+(i*3),2024);}
ps("\n");hdt_report();hdt_state();ps("\n=== Demo Complete ===\n");return 0;}
