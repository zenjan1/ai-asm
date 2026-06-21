/* sambucus_admin: Sambucus management technology administration (v1.0)
 * Sambucus planning, sambucus execution, sambucus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} smb_t;
typedef struct{int n_sbp,n_sbe,n_sbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} smb_state_t;
static smb_t smbps[N],smbss[N-2],smbvss[N-4],smbas[N-6],smbmks[N-6]; static smb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(smb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;smb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SMB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int smb_init(void){if(init)return -1;st.n_sbp=0;st.n_sbe=0;st.n_sbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)smbps[i].active=0;for(int i=0;i<N-2;i++)smbss[i].active=0;for(int i=0;i<N-4;i++)smbvss[i].active=0;for(int i=0;i<N-6;i++)smbas[i].active=0;for(int i=0;i<N-6;i++)smbmks[i].active=0;init=1;ps("[SMB] Sambucus initialized\n");return 0;}
int smb_planning(int t,int c,int a,int b,int d,int e,int y){return add(smbps,&st.n_sbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int smb_execution(int t,int c,int a,int b,int d,int e,int y){return add(smbss,&st.n_sbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int smb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(smbvss,&st.n_sbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int smb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(smbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int smb_market(int t,int c,int a,int b,int d,int e,int y){return add(smbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void smb_report(void){ps("[SMB] Sbp: ");pi(st.n_sbp);ps(" PCS=");pi(st.t_f1);ps("\nSbe: ");pi(st.n_sbe);ps(" PCS=");pi(st.t_f2);ps("\nSbv: ");pi(st.n_sbv);ps(" PCS=");pi(st.t_f3);ps("\nSbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void smb_state(void){ps("[SMB] Sbp=");pi(st.n_sbp);ps(" Sbe=");pi(st.n_sbe);ps(" Sbv=");pi(st.n_sbv);ps(" Sbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sambucus Admin Demo ===\n\n");smb_init();
ps("Sambucus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;smb_planning(t,c,860+(i*17),849+(i*14),829+(i*10),811+(i*6),2020+(i%5));}
ps("\nSambucus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;smb_execution(t,c,849+(i*15),838+(i*12),820+(i*8),807+(i*5),2021+(i%4));}
ps("\nSambucus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;smb_evaluation(t,c,841+(i*13),830+(i*10),814+(i*7),803+(i*4),2022+(i%3));}
ps("\nSambucus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;smb_accessory(t,c,833+(i*11),824+(i*9),810+(i*6),800+(i*3),2023+(i%2));}
ps("\nSambucus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;smb_market(t,c,827+(i*9),818+(i*7),805+(i*5),797+(i*3),2024);}
ps("\n");smb_report();smb_state();ps("\n=== Demo Complete ===\n");return 0;}
