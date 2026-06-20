/* brand_admin: Brand management technology administration (v1.0)
 * Brand planning, brand building, brand maintenance, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brx_t;
typedef struct{int n_bp,n_bb,n_bm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brx_state_t;
static brx_t bps[N],bbs[N-2],bms[N-4],acs[N-6],mks[N-6]; static brx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brx_init(void){if(init)return -1;st.n_bp=0;st.n_bb=0;st.n_bm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bps[i].active=0;for(int i=0;i<N-2;i++)bbs[i].active=0;for(int i=0;i<N-4;i++)bms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BRX] Brand initialized\n");return 0;}
int brx_planning(int t,int c,int a,int b,int d,int e,int y){return add(bps,&st.n_bp,&st.t_f1,N,t,c,a,b,d,e,y);}
int brx_building(int t,int c,int a,int b,int d,int e,int y){return add(bbs,&st.n_bb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brx_maintenance(int t,int c,int a,int b,int d,int e,int y){return add(bms,&st.n_bm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brx_report(void){ps("[BRX] Bp: ");pi(st.n_bp);ps(" PCS=");pi(st.t_f1);ps("\nBb: ");pi(st.n_bb);ps(" PCS=");pi(st.t_f2);ps("\nBm: ");pi(st.n_bm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brx_state(void){ps("[BRX] Bp=");pi(st.n_bp);ps(" Bb=");pi(st.n_bb);ps(" Bm=");pi(st.n_bm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Brand Admin Demo ===\n\n");brx_init();
ps("Brand planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brx_planning(t,c,460+(i*17),449+(i*14),429+(i*10),411+(i*6),2020+(i%5));}
ps("\nBrand building...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brx_building(t,c,449+(i*15),438+(i*12),420+(i*8),407+(i*5),2021+(i%4));}
ps("\nBrand maintenance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brx_maintenance(t,c,441+(i*13),430+(i*10),414+(i*7),403+(i*4),2022+(i%3));}
ps("\nBrand accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brx_accessory(t,c,433+(i*11),424+(i*9),410+(i*6),400+(i*3),2023+(i%2));}
ps("\nBrand marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brx_market(t,c,427+(i*9),418+(i*7),405+(i*5),397+(i*3),2024);}
ps("\n");brx_report();brx_state();ps("\n=== Demo Complete ===\n");return 0;}
