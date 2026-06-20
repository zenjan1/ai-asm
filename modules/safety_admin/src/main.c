/* safety_admin: Safety management technology administration (v1.0)
 * Safety prevention, safety management, occupational health, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sfx_t;
typedef struct{int n_pv,n_ms,n_oh,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sfx_state_t;
static sfx_t pvs[N],mss[N-2],ohs[N-4],acs[N-6],mks[N-6]; static sfx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sfx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sfx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SFX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sfx_init(void){if(init)return -1;st.n_pv=0;st.n_ms=0;st.n_oh=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pvs[i].active=0;for(int i=0;i<N-2;i++)mss[i].active=0;for(int i=0;i<N-4;i++)ohs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SFX] Safety initialized\n");return 0;}
int sfx_prevention(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f1,N,t,c,a,b,d,e,y);}
int sfx_management(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sfx_health(int t,int c,int a,int b,int d,int e,int y){return add(ohs,&st.n_oh,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sfx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sfx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sfx_report(void){ps("[SFX] Pv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f1);ps("\nMs: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f2);ps("\nOh: ");pi(st.n_oh);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sfx_state(void){ps("[SFX] Pv=");pi(st.n_pv);ps(" Ms=");pi(st.n_ms);ps(" Oh=");pi(st.n_oh);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Safety Admin Demo ===\n\n");sfx_init();
ps("Safety prevention...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sfx_prevention(t,c,441+(i*17),430+(i*14),410+(i*10),392+(i*6),2020+(i%5));}
ps("\nSafety management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sfx_management(t,c,430+(i*15),419+(i*12),401+(i*8),388+(i*5),2021+(i%4));}
ps("\nOccupational health...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sfx_health(t,c,422+(i*13),411+(i*10),395+(i*7),384+(i*4),2022+(i%3));}
ps("\nSafety accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sfx_accessory(t,c,414+(i*11),405+(i*9),391+(i*6),381+(i*3),2023+(i%2));}
ps("\nSafety marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sfx_market(t,c,408+(i*9),399+(i*7),386+(i*5),378+(i*3),2024);}
ps("\n");sfx_report();sfx_state();ps("\n=== Demo Complete ===\n");return 0;}
