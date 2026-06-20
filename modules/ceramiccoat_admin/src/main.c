/* ceramiccoat_admin: Ceramic coating technology administration (v1.0)
 * Oxide ceramic, non-oxide ceramic, composite ceramic, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crc_t;
typedef struct{int n_ox,n_no,n_co,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crc_state_t;
static crc_t oxs[N],nos[N-2],cos[N-4],acs[N-6],mks[N-6]; static crc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crc_init(void){if(init)return -1;st.n_ox=0;st.n_no=0;st.n_co=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oxs[i].active=0;for(int i=0;i<N-2;i++)nos[i].active=0;for(int i=0;i<N-4;i++)cos[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRC] Ceramiccoat initialized\n");return 0;}
int crc_oxide(int t,int c,int a,int b,int d,int e,int y){return add(oxs,&st.n_ox,&st.t_f1,N,t,c,a,b,d,e,y);}
int crc_nonoxide(int t,int c,int a,int b,int d,int e,int y){return add(nos,&st.n_no,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crc_composite(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crc_report(void){ps("[CRC] Ox: ");pi(st.n_ox);ps(" PCS=");pi(st.t_f1);ps("\nNo: ");pi(st.n_no);ps(" PCS=");pi(st.t_f2);ps("\nCo: ");pi(st.n_co);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crc_state(void){ps("[CRC] Ox=");pi(st.n_ox);ps(" No=");pi(st.n_no);ps(" Co=");pi(st.n_co);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ceramic Coat Admin Demo ===\n\n");crc_init();
ps("Oxide ceramic coating...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crc_oxide(t,c,344+(i*17),329+(i*14),309+(i*10),291+(i*6),2020+(i%5));}
ps("\nNon-oxide ceramic coating...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crc_nonoxide(t,c,333+(i*15),319+(i*12),301+(i*8),288+(i*5),2021+(i%4));}
ps("\nComposite ceramic coating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crc_composite(t,c,325+(i*13),311+(i*10),295+(i*7),284+(i*4),2022+(i%3));}
ps("\nCeramic coating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crc_accessory(t,c,317+(i*11),305+(i*9),291+(i*6),281+(i*3),2023+(i%2));}
ps("\nCeramic coating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crc_market(t,c,311+(i*9),300+(i*7),287+(i*5),279+(i*3),2024);}
ps("\n");crc_report();crc_state();ps("\n=== Demo Complete ===\n");return 0;}
