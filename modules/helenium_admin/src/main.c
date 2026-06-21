/* helenium_admin: Helenium management technology administration (v1.0)
 * Helenium planning, helenium execution, helenium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hn_t;
typedef struct{int n_hlp,n_hle,n_hlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hn_state_t;
static hn_t hlps[N],hlss[N-2],hlvss[N-4],hlas[N-6],hlmks[N-6]; static hn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HLN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hln_init(void){if(init)return -1;st.n_hlp=0;st.n_hle=0;st.n_hlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hlps[i].active=0;for(int i=0;i<N-2;i++)hlss[i].active=0;for(int i=0;i<N-4;i++)hlvss[i].active=0;for(int i=0;i<N-6;i++)hlas[i].active=0;for(int i=0;i<N-6;i++)hlmks[i].active=0;init=1;ps("[HLN] Helenium initialized\n");return 0;}
int hln_planning(int t,int c,int a,int b,int d,int e,int y){return add(hlps,&st.n_hlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hln_execution(int t,int c,int a,int b,int d,int e,int y){return add(hlss,&st.n_hle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hln_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hlvss,&st.n_hlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hln_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hln_market(int t,int c,int a,int b,int d,int e,int y){return add(hlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hln_report(void){ps("[HLN] Hlp: ");pi(st.n_hlp);ps(" PCS=");pi(st.t_f1);ps("\nHle: ");pi(st.n_hle);ps(" PCS=");pi(st.t_f2);ps("\nHlv: ");pi(st.n_hlv);ps(" PCS=");pi(st.t_f3);ps("\nHlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hln_state(void){ps("[HLN] Hlp=");pi(st.n_hlp);ps(" Hle=");pi(st.n_hle);ps(" Hlv=");pi(st.n_hlv);ps(" Hlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Helenium Admin Demo ===\n\n");hln_init();
ps("Helenium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hln_planning(t,c,733+(i*17),722+(i*14),702+(i*10),684+(i*6),2020+(i%5));}
ps("\nHelenium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hln_execution(t,c,722+(i*15),711+(i*12),693+(i*8),680+(i*5),2021+(i%4));}
ps("\nHelenium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hln_evaluation(t,c,714+(i*13),703+(i*10),687+(i*7),676+(i*4),2022+(i%3));}
ps("\nHelenium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hln_accessory(t,c,706+(i*11),697+(i*9),683+(i*6),673+(i*3),2023+(i%2));}
ps("\nHelenium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hln_market(t,c,700+(i*9),691+(i*7),678+(i*5),670+(i*3),2024);}
ps("\n");hln_report();hln_state();ps("\n=== Demo Complete ===\n");return 0;}
