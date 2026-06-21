/* lupinus_admin: Lupinus management technology administration (v1.0)
 * Lupinus planning, lupinus execution, lupinus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lpn_t;
typedef struct{int n_lpp,n_lpe,n_lpv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lpn_state_t;
static lpn_t lpnps[N],lpness[N-2],lpnvss[N-4],lpnas[N-6],lpnmks[N-6]; static lpn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lpn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lpn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LPN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lpn_init(void){if(init)return -1;st.n_lpp=0;st.n_lpe=0;st.n_lpv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lpnps[i].active=0;for(int i=0;i<N-2;i++)lpness[i].active=0;for(int i=0;i<N-4;i++)lpnvss[i].active=0;for(int i=0;i<N-6;i++)lpnas[i].active=0;for(int i=0;i<N-6;i++)lpnmks[i].active=0;init=1;ps("[LPN] Lupinus initialized\n");return 0;}
int lpn_planning(int t,int c,int a,int b,int d,int e,int y){return add(lpnps,&st.n_lpp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lpn_execution(int t,int c,int a,int b,int d,int e,int y){return add(lpness,&st.n_lpe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lpn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lpnvss,&st.n_lpv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lpn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lpnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lpn_market(int t,int c,int a,int b,int d,int e,int y){return add(lpnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lpn_report(void){ps("[LPN] Lpp: ");pi(st.n_lpp);ps(" PCS=");pi(st.t_f1);ps("\nLpe: ");pi(st.n_lpe);ps(" PCS=");pi(st.t_f2);ps("\nLpv: ");pi(st.n_lpv);ps(" PCS=");pi(st.t_f3);ps("\nLpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lpn_state(void){ps("[LPN] Lpp=");pi(st.n_lpp);ps(" Lpe=");pi(st.n_lpe);ps(" Lpv=");pi(st.n_lpv);ps(" Lpc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lupinus Admin Demo ===\n\n");lpn_init();
ps("Lupinus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lpn_planning(t,c,834+(i*17),823+(i*14),803+(i*10),785+(i*6),2020+(i%5));}
ps("\nLupinus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lpn_execution(t,c,823+(i*15),812+(i*12),794+(i*8),781+(i*5),2021+(i%4));}
ps("\nLupinus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lpn_evaluation(t,c,815+(i*13),804+(i*10),788+(i*7),777+(i*4),2022+(i%3));}
ps("\nLupinus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lpn_accessory(t,c,807+(i*11),798+(i*9),784+(i*6),774+(i*3),2023+(i%2));}
ps("\nLupinus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lpn_market(t,c,801+(i*9),792+(i*7),779+(i*5),771+(i*3),2024);}
ps("\n");lpn_report();lpn_state();ps("\n=== Demo Complete ===\n");return 0;}
