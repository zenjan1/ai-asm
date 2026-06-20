/* pinjoint_admin: Pin joint technology administration (v1.0)
 * Cylindrical pin, conical pin, special pin, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pnj_t;
typedef struct{int n_cy,n_co,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pnj_state_t;
static pnj_t cys[N],cos[N-2],sps[N-4],acs[N-6],mks[N-6]; static pnj_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnj_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pnj_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNJ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pnj_init(void){if(init)return -1;st.n_cy=0;st.n_co=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cys[i].active=0;for(int i=0;i<N-2;i++)cos[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PNJ] Pinjoint initialized\n");return 0;}
int pnj_cylindrical(int t,int c,int a,int b,int d,int e,int y){return add(cys,&st.n_cy,&st.t_f1,N,t,c,a,b,d,e,y);}
int pnj_conical(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pnj_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pnj_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pnj_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pnj_report(void){ps("[PNJ] Cy: ");pi(st.n_cy);ps(" PCS=");pi(st.t_f1);ps("\nCo: ");pi(st.n_co);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pnj_state(void){ps("[PNJ] Cy=");pi(st.n_cy);ps(" Co=");pi(st.n_co);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pin Joint Admin Demo ===\n\n");pnj_init();
ps("Cylindrical pins...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pnj_cylindrical(t,c,364+(i*17),349+(i*14),329+(i*10),311+(i*6),2020+(i%5));}
ps("\nConical pins...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pnj_conical(t,c,353+(i*15),339+(i*12),321+(i*8),308+(i*5),2021+(i%4));}
ps("\nSpecial pins...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pnj_special(t,c,345+(i*13),331+(i*10),315+(i*7),304+(i*4),2022+(i%3));}
ps("\nPin joint accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnj_accessory(t,c,337+(i*11),325+(i*9),311+(i*6),301+(i*3),2023+(i%2));}
ps("\nPin joint marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnj_market(t,c,331+(i*9),320+(i*7),307+(i*5),299+(i*3),2024);}
ps("\n");pnj_report();pnj_state();ps("\n=== Demo Complete ===\n");return 0;}
