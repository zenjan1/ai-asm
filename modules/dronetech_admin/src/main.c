/* dronetech_admin: Drone technology administration (v1.0)
 * Consumer drones, industrial drones, military drones, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drn_t;
typedef struct{int n_cs,n_id,n_ml,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drn_state_t;
static drn_t css[N],ids[N-2],mls[N-4],acs[N-6],mks[N-6]; static drn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(drn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drn_init(void){if(init)return -1;st.n_cs=0;st.n_id=0;st.n_ml=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)css[i].active=0;for(int i=0;i<N-2;i++)ids[i].active=0;for(int i=0;i<N-4;i++)mls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DRN] Dronetech initialized\n");return 0;}
int drn_consumer(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f1,N,t,c,a,b,d,e,y);}
int drn_industrial(int t,int c,int a,int b,int d,int e,int y){return add(ids,&st.n_id,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drn_military(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drn_report(void){ps("[DRN] Cs: ");pi(st.n_cs);ps(" PCS=");pi(st.t_f1);ps("\nId: ");pi(st.n_id);ps(" PCS=");pi(st.t_f2);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drn_state(void){ps("[DRN] Cs=");pi(st.n_cs);ps(" Id=");pi(st.n_id);ps(" Ml=");pi(st.n_ml);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Drone Tech Admin Demo ===\n\n");drn_init();
ps("Consumer drones...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drn_consumer(t,c,255+(i*17),240+(i*14),220+(i*10),202+(i*6),2020+(i%5));}
ps("\nIndustrial drones...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drn_industrial(t,c,244+(i*15),230+(i*12),212+(i*8),199+(i*5),2021+(i%4));}
ps("\nMilitary drones...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drn_military(t,c,236+(i*13),222+(i*10),206+(i*7),195+(i*4),2022+(i%3));}
ps("\nDrone accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drn_accessory(t,c,228+(i*11),216+(i*9),202+(i*6),192+(i*3),2023+(i%2));}
ps("\nDrone marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drn_market(t,c,222+(i*9),211+(i*7),198+(i*5),190+(i*3),2024);}
ps("\n");drn_report();drn_state();ps("\n=== Demo Complete ===\n");return 0;}
