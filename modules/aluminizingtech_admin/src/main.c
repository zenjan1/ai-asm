/* aluminizingtech_admin: Aluminizing technology administration (v1.0)
 * Hot dip aluminizing, pack aluminizing, gas aluminizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alm_t;
typedef struct{int n_hd,n_pk,n_gs,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alm_state_t;
static alm_t hds[N],pks[N-2],gss[N-4],acs[N-6],mks[N-6]; static alm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alm_init(void){if(init)return -1;st.n_hd=0;st.n_pk=0;st.n_gs=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hds[i].active=0;for(int i=0;i<N-2;i++)pks[i].active=0;for(int i=0;i<N-4;i++)gss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ALM] Aluminizingtech initialized\n");return 0;}
int alm_hotdip(int t,int c,int a,int b,int d,int e,int y){return add(hds,&st.n_hd,&st.t_f1,N,t,c,a,b,d,e,y);}
int alm_pack(int t,int c,int a,int b,int d,int e,int y){return add(pks,&st.n_pk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alm_gas(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alm_report(void){ps("[ALM] Hd: ");pi(st.n_hd);ps(" PCS=");pi(st.t_f1);ps("\nPk: ");pi(st.n_pk);ps(" PCS=");pi(st.t_f2);ps("\nGs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alm_state(void){ps("[ALM] Hd=");pi(st.n_hd);ps(" Pk=");pi(st.n_pk);ps(" Gs=");pi(st.n_gs);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aluminizing Tech Admin Demo ===\n\n");alm_init();
ps("Hot dip aluminizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alm_hotdip(t,c,333+(i*17),318+(i*14),298+(i*10),280+(i*6),2020+(i%5));}
ps("\nPack aluminizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alm_pack(t,c,322+(i*15),308+(i*12),290+(i*8),277+(i*5),2021+(i%4));}
ps("\nGas aluminizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alm_gas(t,c,314+(i*13),300+(i*10),284+(i*7),273+(i*4),2022+(i%3));}
ps("\nAluminizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alm_accessory(t,c,306+(i*11),294+(i*9),280+(i*6),270+(i*3),2023+(i%2));}
ps("\nAluminizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alm_market(t,c,300+(i*9),289+(i*7),276+(i*5),268+(i*3),2024);}
ps("\n");alm_report();alm_state();ps("\n=== Demo Complete ===\n");return 0;}
