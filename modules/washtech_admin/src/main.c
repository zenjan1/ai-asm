/* washtech_admin: Washer technology administration (v1.0)
 * Flat washer, spring washer, snap ring, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wsh_t;
typedef struct{int n_fl,n_sp,n_sn,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wsh_state_t;
static wsh_t fls[N],sps[N-2],sns[N-4],acs[N-6],mks[N-6]; static wsh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wsh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wsh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WSH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wsh_init(void){if(init)return -1;st.n_fl=0;st.n_sp=0;st.n_sn=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fls[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)sns[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WSH] Washtech initialized\n");return 0;}
int wsh_flat(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f1,N,t,c,a,b,d,e,y);}
int wsh_spring(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wsh_snap(int t,int c,int a,int b,int d,int e,int y){return add(sns,&st.n_sn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wsh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wsh_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wsh_report(void){ps("[WSH] Fl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f1);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f2);ps("\nSn: ");pi(st.n_sn);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wsh_state(void){ps("[WSH] Fl=");pi(st.n_fl);ps(" Sp=");pi(st.n_sp);ps(" Sn=");pi(st.n_sn);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Washer Tech Admin Demo ===\n\n");wsh_init();
ps("Flat washers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wsh_flat(t,c,363+(i*17),348+(i*14),328+(i*10),310+(i*6),2020+(i%5));}
ps("\nSpring washers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wsh_spring(t,c,352+(i*15),338+(i*12),320+(i*8),307+(i*5),2021+(i%4));}
ps("\nSnap rings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wsh_snap(t,c,344+(i*13),330+(i*10),314+(i*7),303+(i*4),2022+(i%3));}
ps("\nWasher accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wsh_accessory(t,c,336+(i*11),324+(i*9),310+(i*6),300+(i*3),2023+(i%2));}
ps("\nWasher marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wsh_market(t,c,330+(i*9),319+(i*7),306+(i*5),298+(i*3),2024);}
ps("\n");wsh_report();wsh_state();ps("\n=== Demo Complete ===\n");return 0;}
