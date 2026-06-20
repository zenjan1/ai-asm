/* meddev_admin: Medical device technology administration (v1.0)
 * Medical devices, medical imaging, in vitro diagnostics, medical consumables, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mdd_t;
typedef struct{int n_md,n_mi,n_ivd,n_mc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mdd_state_t;
static mdd_t mds[N],mis[N-2],ivds[N-4],mcs[N-6],mks[N-6]; static mdd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mdd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mdd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mdd_init(void){if(init)return -1;st.n_md=0;st.n_mi=0;st.n_ivd=0;st.n_mc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mds[i].active=0;for(int i=0;i<N-2;i++)mis[i].active=0;for(int i=0;i<N-4;i++)ivds[i].active=0;for(int i=0;i<N-6;i++)mcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MDD] Meddev initialized\n");return 0;}
int mdd_device(int t,int c,int a,int b,int d,int e,int y){return add(mds,&st.n_md,&st.t_f1,N,t,c,a,b,d,e,y);}
int mdd_imaging(int t,int c,int a,int b,int d,int e,int y){return add(mis,&st.n_mi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mdd_ivd(int t,int c,int a,int b,int d,int e,int y){return add(ivds,&st.n_ivd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mdd_consumable(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mdd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mdd_report(void){ps("[MDD] Md: ");pi(st.n_md);ps(" PCS=");pi(st.t_f1);ps("\nMi: ");pi(st.n_mi);ps(" PCS=");pi(st.t_f2);ps("\nIvd: ");pi(st.n_ivd);ps(" Tests=");pi(st.t_f3);ps("\nMc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mdd_state(void){ps("[MDD] Md=");pi(st.n_md);ps(" Mi=");pi(st.n_mi);ps(" Ivd=");pi(st.n_ivd);ps(" Mc=");pi(st.n_mc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Medical Device Tech Admin Demo ===\n\n");mdd_init();
ps("Medical devices...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mdd_device(t,c,225+(i*17),210+(i*14),190+(i*10),172+(i*6),2020+(i%5));}
ps("\nMedical imaging...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mdd_imaging(t,c,214+(i*15),200+(i*12),182+(i*8),169+(i*5),2021+(i%4));}
ps("\nIn vitro diagnostics...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mdd_ivd(t,c,206+(i*13),192+(i*10),176+(i*7),165+(i*4),2022+(i%3));}
ps("\nMedical consumables...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdd_consumable(t,c,198+(i*11),186+(i*9),172+(i*6),162+(i*3),2023+(i%2));}
ps("\nMedical marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdd_market(t,c,192+(i*9),181+(i*7),168+(i*5),160+(i*3),2024);}
ps("\n");mdd_report();mdd_state();ps("\n=== Demo Complete ===\n");return 0;}
