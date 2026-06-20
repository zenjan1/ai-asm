/* engine_admin: Engine technology administration (v1.0)
 * Gasoline engine, diesel engine, alternative fuel engine, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eng_t;
typedef struct{int n_ga,n_di,n_af,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eng_state_t;
static eng_t gas[N],dis[N-2],afs[N-4],acs[N-6],mks[N-6]; static eng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eng_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eng_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ENG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eng_init(void){if(init)return -1;st.n_ga=0;st.n_di=0;st.n_af=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gas[i].active=0;for(int i=0;i<N-2;i++)dis[i].active=0;for(int i=0;i<N-4;i++)afs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ENG] Engine initialized\n");return 0;}
int eng_gasoline(int t,int c,int a,int b,int d,int e,int y){return add(gas,&st.n_ga,&st.t_f1,N,t,c,a,b,d,e,y);}
int eng_diesel(int t,int c,int a,int b,int d,int e,int y){return add(dis,&st.n_di,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eng_altfuel(int t,int c,int a,int b,int d,int e,int y){return add(afs,&st.n_af,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eng_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eng_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eng_report(void){ps("[ENG] Ga: ");pi(st.n_ga);ps(" PCS=");pi(st.t_f1);ps("\nDi: ");pi(st.n_di);ps(" PCS=");pi(st.t_f2);ps("\nAf: ");pi(st.n_af);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eng_state(void){ps("[ENG] Ga=");pi(st.n_ga);ps(" Di=");pi(st.n_di);ps(" Af=");pi(st.n_af);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Engine Admin Demo ===\n\n");eng_init();
ps("Gasoline engines...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eng_gasoline(t,c,375+(i*17),360+(i*14),340+(i*10),322+(i*6),2020+(i%5));}
ps("\nDiesel engines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eng_diesel(t,c,364+(i*15),350+(i*12),332+(i*8),319+(i*5),2021+(i%4));}
ps("\nAlternative fuel engines...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eng_altfuel(t,c,356+(i*13),342+(i*10),326+(i*7),315+(i*4),2022+(i%3));}
ps("\nEngine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eng_accessory(t,c,348+(i*11),336+(i*9),322+(i*6),312+(i*3),2023+(i%2));}
ps("\nEngine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eng_market(t,c,342+(i*9),331+(i*7),318+(i*5),310+(i*3),2024);}
ps("\n");eng_report();eng_state();ps("\n=== Demo Complete ===\n");return 0;}
