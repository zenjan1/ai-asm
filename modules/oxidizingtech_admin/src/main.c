/* oxidizingtech_admin: Oxidizing technology administration (v1.0)
 * Alkaline oxidizing, acidic oxidizing, anodic oxidizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oxd_t;
typedef struct{int n_ak,n_ad,n_an,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oxd_state_t;
static oxd_t aks[N],ads[N-2],ans[N-4],acs[N-6],mks[N-6]; static oxd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oxd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oxd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OXD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oxd_init(void){if(init)return -1;st.n_ak=0;st.n_ad=0;st.n_an=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aks[i].active=0;for(int i=0;i<N-2;i++)ads[i].active=0;for(int i=0;i<N-4;i++)ans[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OXD] Oxidizingtech initialized\n");return 0;}
int oxd_alkaline(int t,int c,int a,int b,int d,int e,int y){return add(aks,&st.n_ak,&st.t_f1,N,t,c,a,b,d,e,y);}
int oxd_acidic(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oxd_anodic(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oxd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oxd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oxd_report(void){ps("[OXD] Ak: ");pi(st.n_ak);ps(" PCS=");pi(st.t_f1);ps("\nAd: ");pi(st.n_ad);ps(" PCS=");pi(st.t_f2);ps("\nAn: ");pi(st.n_an);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oxd_state(void){ps("[OXD] Ak=");pi(st.n_ak);ps(" Ad=");pi(st.n_ad);ps(" An=");pi(st.n_an);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oxidizing Tech Admin Demo ===\n\n");oxd_init();
ps("Alkaline oxidizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oxd_alkaline(t,c,335+(i*17),320+(i*14),300+(i*10),282+(i*6),2020+(i%5));}
ps("\nAcidic oxidizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oxd_acidic(t,c,324+(i*15),310+(i*12),292+(i*8),279+(i*5),2021+(i%4));}
ps("\nAnodic oxidizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oxd_anodic(t,c,316+(i*13),302+(i*10),286+(i*7),275+(i*4),2022+(i%3));}
ps("\nOxidizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxd_accessory(t,c,308+(i*11),296+(i*9),282+(i*6),272+(i*3),2023+(i%2));}
ps("\nOxidizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxd_market(t,c,302+(i*9),291+(i*7),278+(i*5),270+(i*3),2024);}
ps("\n");oxd_report();oxd_state();ps("\n=== Demo Complete ===\n");return 0;}
