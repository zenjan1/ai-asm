/* delphinium_admin: Delphinium management technology administration (v1.0)
 * Delphinium planning, delphinium execution, delphinium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} del_t;
typedef struct{int n_dlp,n_dle,n_dlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} del_state_t;
static del_t dlps[N],dles[N-2],dlvs[N-4],acs[N-6],mks[N-6]; static del_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(del_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;del_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int del_init(void){if(init)return -1;st.n_dlp=0;st.n_dle=0;st.n_dlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dlps[i].active=0;for(int i=0;i<N-2;i++)dles[i].active=0;for(int i=0;i<N-4;i++)dlvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DEL] Delphinium initialized\n");return 0;}
int del_planning(int t,int c,int a,int b,int d,int e,int y){return add(dlps,&st.n_dlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int del_execution(int t,int c,int a,int b,int d,int e,int y){return add(dles,&st.n_dle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int del_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dlvs,&st.n_dlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int del_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int del_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void del_report(void){ps("[DEL] Dlp: ");pi(st.n_dlp);ps(" PCS=");pi(st.t_f1);ps("\nDle: ");pi(st.n_dle);ps(" PCS=");pi(st.t_f2);ps("\nDlv: ");pi(st.n_dlv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void del_state(void){ps("[DEL] Dlp=");pi(st.n_dlp);ps(" Dle=");pi(st.n_dle);ps(" Dlv=");pi(st.n_dlv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Delphinium Admin Demo ===\n\n");del_init();
ps("Delphinium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;del_planning(t,c,577+(i*17),566+(i*14),546+(i*10),528+(i*6),2020+(i%5));}
ps("\nDelphinium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;del_execution(t,c,566+(i*15),555+(i*12),537+(i*8),524+(i*5),2021+(i%4));}
ps("\nDelphinium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;del_evaluation(t,c,558+(i*13),547+(i*10),531+(i*7),520+(i*4),2022+(i%3));}
ps("\nDelphinium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;del_accessory(t,c,550+(i*11),541+(i*9),527+(i*6),517+(i*3),2023+(i%2));}
ps("\nDelphinium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;del_market(t,c,544+(i*9),535+(i*7),522+(i*5),514+(i*3),2024);}
ps("\n");del_report();del_state();ps("\n=== Demo Complete ===\n");return 0;}
