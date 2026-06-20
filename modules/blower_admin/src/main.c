/* blower_admin: Blower technology administration (v1.0)
 * Centrifugal blower, roots blower, rotary blower, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blr_t;
typedef struct{int n_cf,n_rt,n_rr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blr_state_t;
static blr_t cfs[N],rts[N-2],rrs[N-4],acs[N-6],mks[N-6]; static blr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blr_init(void){if(init)return -1;st.n_cf=0;st.n_rt=0;st.n_rr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cfs[i].active=0;for(int i=0;i<N-2;i++)rts[i].active=0;for(int i=0;i<N-4;i++)rrs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLR] Blower initialized\n");return 0;}
int blr_centrifugal(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f1,N,t,c,a,b,d,e,y);}
int blr_roots(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blr_rotary(int t,int c,int a,int b,int d,int e,int y){return add(rrs,&st.n_rr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blr_report(void){ps("[BLR] Cf: ");pi(st.n_cf);ps(" PCS=");pi(st.t_f1);ps("\nRt: ");pi(st.n_rt);ps(" PCS=");pi(st.t_f2);ps("\nRr: ");pi(st.n_rr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blr_state(void){ps("[BLR] Cf=");pi(st.n_cf);ps(" Rt=");pi(st.n_rt);ps(" Rr=");pi(st.n_rr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Blower Admin Demo ===\n\n");blr_init();
ps("Centrifugal blowers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blr_centrifugal(t,c,371+(i*17),356+(i*14),336+(i*10),318+(i*6),2020+(i%5));}
ps("\nRoots blowers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blr_roots(t,c,360+(i*15),346+(i*12),328+(i*8),315+(i*5),2021+(i%4));}
ps("\nRotary blowers...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blr_rotary(t,c,352+(i*13),338+(i*10),322+(i*7),311+(i*4),2022+(i%3));}
ps("\nBlower accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blr_accessory(t,c,344+(i*11),332+(i*9),318+(i*6),308+(i*3),2023+(i%2));}
ps("\nBlower marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blr_market(t,c,338+(i*9),327+(i*7),314+(i*5),306+(i*3),2024);}
ps("\n");blr_report();blr_state();ps("\n=== Demo Complete ===\n");return 0;}
