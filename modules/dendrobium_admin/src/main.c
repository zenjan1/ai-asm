/* dendrobium_admin: Dendrobium management technology administration (v1.0)
 * Dendrobium planning, dendrobium execution, dendrobium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dnb_t;
typedef struct{int n_dnbp,n_dnbe,n_dnb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dnb_state_t;
static dnb_t dnbps[N],dnbss[N-2],dnbvss[N-4],dnbas[N-6],dnbmks[N-6]; static dnb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dnb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dnb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DNB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dnb_init(void){if(init)return -1;st.n_dnbp=0;st.n_dnbe=0;st.n_dnb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dnbps[i].active=0;for(int i=0;i<N-2;i++)dnbss[i].active=0;for(int i=0;i<N-4;i++)dnbvss[i].active=0;for(int i=0;i<N-6;i++)dnbas[i].active=0;for(int i=0;i<N-6;i++)dnbmks[i].active=0;init=1;ps("[DNB] Dendrobium initialized\n");return 0;}
int dnb_planning(int t,int c,int a,int b,int d,int e,int y){return add(dnbps,&st.n_dnbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dnb_execution(int t,int c,int a,int b,int d,int e,int y){return add(dnbss,&st.n_dnbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dnb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dnbvss,&st.n_dnb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dnb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dnbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dnb_market(int t,int c,int a,int b,int d,int e,int y){return add(dnbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dnb_report(void){ps("[DNB] Denp: ");pi(st.n_dnbp);ps(" PCS=");pi(st.t_f1);ps("\nDene: ");pi(st.n_dnbe);ps(" PCS=");pi(st.t_f2);ps("\nDenv: ");pi(st.n_dnb2);ps(" PCS=");pi(st.t_f3);ps("\nDenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dnb_state(void){ps("[DNB] Denp=");pi(st.n_dnbp);ps(" Dene=");pi(st.n_dnbe);ps(" Denv=");pi(st.n_dnb2);ps(" Denc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dendrobium Admin Demo ===\n\n");dnb_init();
ps("Dendrobium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dnb_planning(t,c,1258+(i*17),1247+(i*14),1227+(i*10),1209+(i*6),2020+(i%5));}
ps("\nDendrobium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dnb_execution(t,c,1247+(i*15),1236+(i*12),1218+(i*8),1205+(i*5),2021+(i%4));}
ps("\nDendrobium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dnb_evaluation(t,c,1239+(i*13),1228+(i*10),1212+(i*7),1201+(i*4),2022+(i%3));}
ps("\nDendrobium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dnb_accessory(t,c,1231+(i*11),1222+(i*9),1208+(i*6),1198+(i*3),2023+(i%2));}
ps("\nDendrobium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dnb_market(t,c,1225+(i*9),1216+(i*7),1203+(i*5),1195+(i*3),2024);}
ps("\n");dnb_report();dnb_state();ps("\n=== Demo Complete ===\n");return 0;}
