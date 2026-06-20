/* miningtech_admin: Mining technology administration (v1.0)
 * Coal mining, metal mining, non-metal mining, mining services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mng_t;
typedef struct{int n_co,n_me,n_nm,n_ms,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mng_state_t;
static mng_t cos[N],mes[N-2],nms[N-4],mss[N-6],mks[N-6]; static mng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mng_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mng_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MNG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mng_init(void){if(init)return -1;st.n_co=0;st.n_me=0;st.n_nm=0;st.n_ms=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cos[i].active=0;for(int i=0;i<N-2;i++)mes[i].active=0;for(int i=0;i<N-4;i++)nms[i].active=0;for(int i=0;i<N-6;i++)mss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MNG] Miningtech initialized\n");return 0;}
int mng_coal(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f1,N,t,c,a,b,d,e,y);}
int mng_metal(int t,int c,int a,int b,int d,int e,int y){return add(mes,&st.n_me,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mng_nonmetal(int t,int c,int a,int b,int d,int e,int y){return add(nms,&st.n_nm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mng_service(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mng_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mng_report(void){ps("[MNG] Co: ");pi(st.n_co);ps(" PCS=");pi(st.t_f1);ps("\nMe: ");pi(st.n_me);ps(" PCS=");pi(st.t_f2);ps("\nNm: ");pi(st.n_nm);ps(" PCS=");pi(st.t_f3);ps("\nMs: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mng_state(void){ps("[MNG] Co=");pi(st.n_co);ps(" Me=");pi(st.n_me);ps(" Nm=");pi(st.n_nm);ps(" Ms=");pi(st.n_ms);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mining Tech Admin Demo ===\n\n");mng_init();
ps("Coal mining...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mng_coal(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nMetal mining...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mng_metal(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nNon-metal mining...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mng_nonmetal(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nMining services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mng_service(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nMining marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mng_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");mng_report();mng_state();ps("\n=== Demo Complete ===\n");return 0;}
