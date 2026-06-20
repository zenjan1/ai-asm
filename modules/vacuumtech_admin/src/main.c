/* vacuumtech_admin: Vacuum technology administration (v1.0)
 * Vacuum pumps, vacuum chambers, vacuum measurement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vac_t;
typedef struct{int n_pm,n_ch,n_ms,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vac_state_t;
static vac_t pms[N],chs[N-2],mss[N-4],acs[N-6],mks[N-6]; static vac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vac_init(void){if(init)return -1;st.n_pm=0;st.n_ch=0;st.n_ms=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pms[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)mss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[VAC] Vacuumtech initialized\n");return 0;}
int vac_pump(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f1,N,t,c,a,b,d,e,y);}
int vac_chamber(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vac_measurement(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vac_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vac_report(void){ps("[VAC] Pm: ");pi(st.n_pm);ps(" PCS=");pi(st.t_f1);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f2);ps("\nMs: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vac_state(void){ps("[VAC] Pm=");pi(st.n_pm);ps(" Ch=");pi(st.n_ch);ps(" Ms=");pi(st.n_ms);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vacuum Tech Admin Demo ===\n\n");vac_init();
ps("Vacuum pumps...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vac_pump(t,c,309+(i*17),294+(i*14),274+(i*10),256+(i*6),2020+(i%5));}
ps("\nVacuum chambers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vac_chamber(t,c,298+(i*15),284+(i*12),266+(i*8),253+(i*5),2021+(i%4));}
ps("\nVacuum measurement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vac_measurement(t,c,290+(i*13),276+(i*10),260+(i*7),249+(i*4),2022+(i%3));}
ps("\nVacuum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vac_accessory(t,c,282+(i*11),270+(i*9),256+(i*6),246+(i*3),2023+(i%2));}
ps("\nVacuum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vac_market(t,c,276+(i*9),265+(i*7),252+(i*5),244+(i*3),2024);}
ps("\n");vac_report();vac_state();ps("\n=== Demo Complete ===\n");return 0;}
