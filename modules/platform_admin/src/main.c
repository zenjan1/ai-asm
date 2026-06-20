/* platform_admin: Platform management technology administration (v1.0)
 * Platform planning, platform operations, platform evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pfm_t;
typedef struct{int n_pp,n_po,n_pe,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pfm_state_t;
static pfm_t pps[N],pos[N-2],pes[N-4],acs[N-6],mks[N-6]; static pfm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pfm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pfm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PFM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pfm_init(void){if(init)return -1;st.n_pp=0;st.n_po=0;st.n_pe=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)pos[i].active=0;for(int i=0;i<N-4;i++)pes[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PFM] Platform initialized\n");return 0;}
int pfm_planning(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pfm_operations(int t,int c,int a,int b,int d,int e,int y){return add(pos,&st.n_po,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pfm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pfm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pfm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pfm_report(void){ps("[PFM] Pp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f1);ps("\nPo: ");pi(st.n_po);ps(" PCS=");pi(st.t_f2);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pfm_state(void){ps("[PFM] Pp=");pi(st.n_pp);ps(" Po=");pi(st.n_po);ps(" Pe=");pi(st.n_pe);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Platform Admin Demo ===\n\n");pfm_init();
ps("Platform planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pfm_planning(t,c,484+(i*17),473+(i*14),453+(i*10),435+(i*6),2020+(i%5));}
ps("\nPlatform operations...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pfm_operations(t,c,473+(i*15),462+(i*12),444+(i*8),431+(i*5),2021+(i%4));}
ps("\nPlatform evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pfm_evaluation(t,c,465+(i*13),454+(i*10),438+(i*7),427+(i*4),2022+(i%3));}
ps("\nPlatform accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pfm_accessory(t,c,457+(i*11),448+(i*9),434+(i*6),424+(i*3),2023+(i%2));}
ps("\nPlatform marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pfm_market(t,c,451+(i*9),442+(i*7),429+(i*5),421+(i*3),2024);}
ps("\n");pfm_report();pfm_state();ps("\n=== Demo Complete ===\n");return 0;}
