/* powertech_admin: Power technology administration (v1.0)
 * Power generation, transmission, distribution, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pwr_t;
typedef struct{int n_gn,n_tr,n_ds,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pwr_state_t;
static pwr_t gns[N],trs[N-2],dss[N-4],acs[N-6],mks[N-6]; static pwr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pwr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pwr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PWR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pwr_init(void){if(init)return -1;st.n_gn=0;st.n_tr=0;st.n_ds=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gns[i].active=0;for(int i=0;i<N-2;i++)trs[i].active=0;for(int i=0;i<N-4;i++)dss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PWR] Powertech initialized\n");return 0;}
int pwr_generation(int t,int c,int a,int b,int d,int e,int y){return add(gns,&st.n_gn,&st.t_f1,N,t,c,a,b,d,e,y);}
int pwr_transmission(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pwr_distribution(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pwr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pwr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pwr_report(void){ps("[PWR] Gn: ");pi(st.n_gn);ps(" PCS=");pi(st.t_f1);ps("\nTr: ");pi(st.n_tr);ps(" PCS=");pi(st.t_f2);ps("\nDs: ");pi(st.n_ds);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pwr_state(void){ps("[PWR] Gn=");pi(st.n_gn);ps(" Tr=");pi(st.n_tr);ps(" Ds=");pi(st.n_ds);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Power Tech Admin Demo ===\n\n");pwr_init();
ps("Power generation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pwr_generation(t,c,314+(i*17),299+(i*14),279+(i*10),261+(i*6),2020+(i%5));}
ps("\nPower transmission...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pwr_transmission(t,c,303+(i*15),289+(i*12),271+(i*8),258+(i*5),2021+(i%4));}
ps("\nPower distribution...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pwr_distribution(t,c,295+(i*13),281+(i*10),265+(i*7),254+(i*4),2022+(i%3));}
ps("\nPower accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pwr_accessory(t,c,287+(i*11),275+(i*9),261+(i*6),251+(i*3),2023+(i%2));}
ps("\nPower marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pwr_market(t,c,281+(i*9),270+(i*7),257+(i*5),249+(i*3),2024);}
ps("\n");pwr_report();pwr_state();ps("\n=== Demo Complete ===\n");return 0;}
