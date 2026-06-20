/* laser_admin: Laser processing technology administration (v1.0)
 * Laser cutting, laser welding, laser marking/engraving, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lsr_t;
typedef struct{int n_ct,n_wd,n_mk,n_ac,n_mt,t_f1,t_f2,t_f3,t_f4,t_f5;} lsr_state_t;
static lsr_t cts[N],wds[N-2],mks[N-4],acs[N-6],mts[N-6]; static lsr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lsr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lsr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LSR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lsr_init(void){if(init)return -1;st.n_ct=0;st.n_wd=0;st.n_mk=0;st.n_ac=0;st.n_mt=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cts[i].active=0;for(int i=0;i<N-2;i++)wds[i].active=0;for(int i=0;i<N-4;i++)mks[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mts[i].active=0;init=1;ps("[LSR] Laser initialized\n");return 0;}
int lsr_cutting(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f1,N,t,c,a,b,d,e,y);}
int lsr_welding(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lsr_marking(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lsr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lsr_market(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lsr_report(void){ps("[LSR] Ct: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f1);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f2);ps("\nMk: ");pi(st.n_mk);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMt: ");pi(st.n_mt);ps(" USD=");pi(st.t_f5);ps("\n");}
void lsr_state(void){ps("[LSR] Ct=");pi(st.n_ct);ps(" Wd=");pi(st.n_wd);ps(" Mk=");pi(st.n_mk);ps(" Ac=");pi(st.n_ac);ps(" Mt=");pi(st.n_mt);ps("\n");}
int main(void){
ps("=== Laser Admin Demo ===\n\n");lsr_init();
ps("Laser cutting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lsr_cutting(t,c,409+(i*17),398+(i*14),378+(i*10),360+(i*6),2020+(i%5));}
ps("\nLaser welding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lsr_welding(t,c,398+(i*15),387+(i*12),369+(i*8),356+(i*5),2021+(i%4));}
ps("\nLaser marking/engraving...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lsr_marking(t,c,390+(i*13),379+(i*10),363+(i*7),352+(i*4),2022+(i%3));}
ps("\nLaser accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lsr_accessory(t,c,382+(i*11),373+(i*9),359+(i*6),349+(i*3),2023+(i%2));}
ps("\nLaser marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lsr_market(t,c,376+(i*9),367+(i*7),354+(i*5),346+(i*3),2024);}
ps("\n");lsr_report();lsr_state();ps("\n=== Demo Complete ===\n");return 0;}
