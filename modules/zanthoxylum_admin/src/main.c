/* zanthoxylum_admin: Zanthoxylum management technology administration (v1.0)
 * Zanthoxylum planning, zanthoxylum execution, zanthoxylum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ztx_t;
typedef struct{int n_zxp,n_zxe,n_zxv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ztx_state_t;
static ztx_t ztxps[N],ztxss[N-2],ztxvss[N-4],ztxas[N-6],ztxmks[N-6]; static ztx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ztx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ztx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZTX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ztx_init(void){if(init)return -1;st.n_zxp=0;st.n_zxe=0;st.n_zxv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ztxps[i].active=0;for(int i=0;i<N-2;i++)ztxss[i].active=0;for(int i=0;i<N-4;i++)ztxvss[i].active=0;for(int i=0;i<N-6;i++)ztxas[i].active=0;for(int i=0;i<N-6;i++)ztxmks[i].active=0;init=1;ps("[ZTX] Zanthoxylum initialized\n");return 0;}
int ztx_planning(int t,int c,int a,int b,int d,int e,int y){return add(ztxps,&st.n_zxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ztx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ztxss,&st.n_zxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ztx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ztxvss,&st.n_zxv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ztx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ztxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ztx_market(int t,int c,int a,int b,int d,int e,int y){return add(ztxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ztx_report(void){ps("[ZTX] Zxp: ");pi(st.n_zxp);ps(" PCS=");pi(st.t_f1);ps("\nZxe: ");pi(st.n_zxe);ps(" PCS=");pi(st.t_f2);ps("\nZxv: ");pi(st.n_zxv);ps(" PCS=");pi(st.t_f3);ps("\nZxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ztx_state(void){ps("[ZTX] Zxp=");pi(st.n_zxp);ps(" Zxe=");pi(st.n_zxe);ps(" Zxv=");pi(st.n_zxv);ps(" Zxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zanthoxylum Admin Demo ===\n\n");ztx_init();
ps("Zanthoxylum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ztx_planning(t,c,881+(i*17),870+(i*14),850+(i*10),832+(i*6),2020+(i%5));}
ps("\nZanthoxylum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ztx_execution(t,c,870+(i*15),859+(i*12),841+(i*8),828+(i*5),2021+(i%4));}
ps("\nZanthoxylum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ztx_evaluation(t,c,862+(i*13),851+(i*10),835+(i*7),824+(i*4),2022+(i%3));}
ps("\nZanthoxylum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ztx_accessory(t,c,854+(i*11),845+(i*9),831+(i*6),821+(i*3),2023+(i%2));}
ps("\nZanthoxylum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ztx_market(t,c,848+(i*9),839+(i*7),826+(i*5),818+(i*3),2024);}
ps("\n");ztx_report();ztx_state();ps("\n=== Demo Complete ===\n");return 0;}
