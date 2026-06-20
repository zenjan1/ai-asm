/* material_admin: Material technology administration (v1.0)
 * Metal materials, non-metal materials, material handling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtx_t;
typedef struct{int n_mt,n_nm,n_hd,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mtx_state_t;
static mtx_t mts[N],nms[N-2],hds[N-4],acs[N-6],mks[N-6]; static mtx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtx_init(void){if(init)return -1;st.n_mt=0;st.n_nm=0;st.n_hd=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mts[i].active=0;for(int i=0;i<N-2;i++)nms[i].active=0;for(int i=0;i<N-4;i++)hds[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MTX] Material initialized\n");return 0;}
int mtx_metal(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtx_nonmetal(int t,int c,int a,int b,int d,int e,int y){return add(nms,&st.n_nm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtx_handling(int t,int c,int a,int b,int d,int e,int y){return add(hds,&st.n_hd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtx_report(void){ps("[MTX] Mt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f1);ps("\nNm: ");pi(st.n_nm);ps(" PCS=");pi(st.t_f2);ps("\nHd: ");pi(st.n_hd);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mtx_state(void){ps("[MTX] Mt=");pi(st.n_mt);ps(" Nm=");pi(st.n_nm);ps(" Hd=");pi(st.n_hd);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Material Admin Demo ===\n\n");mtx_init();
ps("Metal materials...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtx_metal(t,c,431+(i*17),420+(i*14),400+(i*10),382+(i*6),2020+(i%5));}
ps("\nNon-metal materials...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtx_nonmetal(t,c,420+(i*15),409+(i*12),391+(i*8),378+(i*5),2021+(i%4));}
ps("\nMaterial handling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtx_handling(t,c,412+(i*13),401+(i*10),385+(i*7),374+(i*4),2022+(i%3));}
ps("\nMaterial accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtx_accessory(t,c,404+(i*11),395+(i*9),381+(i*6),371+(i*3),2023+(i%2));}
ps("\nMaterial marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtx_market(t,c,398+(i*9),389+(i*7),376+(i*5),368+(i*3),2024);}
ps("\n");mtx_report();mtx_state();ps("\n=== Demo Complete ===\n");return 0;}
