/* mediatech_admin: Media technology administration (v1.0)
 * News media, video media, social media, advertising, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mdt_t;
typedef struct{int n_nm,n_vm,n_sm,n_ad,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mdt_state_t;
static mdt_t nms[N],vms[N-2],sms[N-4],ads[N-6],mks[N-6]; static mdt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mdt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mdt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mdt_init(void){if(init)return -1;st.n_nm=0;st.n_vm=0;st.n_sm=0;st.n_ad=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nms[i].active=0;for(int i=0;i<N-2;i++)vms[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)ads[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MDT] Mediatech initialized\n");return 0;}
int mdt_news(int t,int c,int a,int b,int d,int e,int y){return add(nms,&st.n_nm,&st.t_f1,N,t,c,a,b,d,e,y);}
int mdt_video(int t,int c,int a,int b,int d,int e,int y){return add(vms,&st.n_vm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mdt_social(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mdt_advert(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mdt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mdt_report(void){ps("[MDT] Nm: ");pi(st.n_nm);ps(" PCS=");pi(st.t_f1);ps("\nVm: ");pi(st.n_vm);ps(" PCS=");pi(st.t_f2);ps("\nSm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f3);ps("\nAd: ");pi(st.n_ad);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mdt_state(void){ps("[MDT] Nm=");pi(st.n_nm);ps(" Vm=");pi(st.n_vm);ps(" Sm=");pi(st.n_sm);ps(" Ad=");pi(st.n_ad);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Media Tech Admin Demo ===\n\n");mdt_init();
ps("News media...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mdt_news(t,c,238+(i*17),223+(i*14),203+(i*10),185+(i*6),2020+(i%5));}
ps("\nVideo media...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mdt_video(t,c,227+(i*15),213+(i*12),195+(i*8),182+(i*5),2021+(i%4));}
ps("\nSocial media...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mdt_social(t,c,219+(i*13),205+(i*10),189+(i*7),178+(i*4),2022+(i%3));}
ps("\nAdvertising...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdt_advert(t,c,211+(i*11),199+(i*9),185+(i*6),175+(i*3),2023+(i%2));}
ps("\nMedia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdt_market(t,c,205+(i*9),194+(i*7),181+(i*5),173+(i*3),2024);}
ps("\n");mdt_report();mdt_state();ps("\n=== Demo Complete ===\n");return 0;}
