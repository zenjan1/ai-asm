/* vogelae_admin: Vogelae management technology administration (v1.0)
 * Vogelae planning, vogelae execution, vogelae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} voge_t;
typedef struct{int n_vogep,n_voge,n_voge2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} voge_state_t;
static voge_t voges[N],vogee[N-2],vogev[N-4],vogac[N-6],vogm[N-6]; static voge_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(voge_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;voge_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VOGE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int voge_init(void){if(init)return -1;st.n_vogep=0;st.n_voge=0;st.n_voge2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)voges[i].active=0;for(int i=0;i<N-2;i++)vogee[i].active=0;for(int i=0;i<N-4;i++)vogev[i].active=0;for(int i=0;i<N-6;i++)vogac[i].active=0;for(int i=0;i<N-6;i++)vogm[i].active=0;init=1;ps("[VOGE] Vogelae initialized\n");return 0;}
int voge_planning(int t,int c,int a,int b,int d,int e,int y){return add(voges,&st.n_vogep,&st.t_f1,N,t,c,a,b,d,e,y);}
int voge_execution(int t,int c,int a,int b,int d,int e,int y){return add(vogee,&st.n_voge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int voge_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vogev,&st.n_voge2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int voge_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vogac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int voge_market(int t,int c,int a,int b,int d,int e,int y){return add(vogm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void voge_report(void){ps("[VOGE] Vogep: ");pi(st.n_vogep);ps(" PCS=");pi(st.t_f1);ps("\nVogee: ");pi(st.n_voge);ps(" PCS=");pi(st.t_f2);ps("\nVogev: ");pi(st.n_voge2);ps(" PCS=");pi(st.t_f3);ps("\nVogac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void voge_state(void){ps("[VOGE] Vogep=");pi(st.n_vogep);ps(" Vogee=");pi(st.n_voge);ps(" Vogev=");pi(st.n_voge2);ps(" Vogac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vogelae Admin Demo ===\n\n");voge_init();
ps("Vogelae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;voge_planning(t,c,1369+(i*17),1358+(i*14),1338+(i*10),1320+(i*6),2020+(i%5));}
ps("\nVogelae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;voge_execution(t,c,1358+(i*15),1347+(i*12),1329+(i*8),1316+(i*5),2021+(i%4));}
ps("\nVogelae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;voge_evaluation(t,c,1350+(i*13),1339+(i*10),1323+(i*7),1312+(i*4),2022+(i%3));}
ps("\nVogelae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;voge_accessory(t,c,1342+(i*11),1333+(i*9),1319+(i*6),1309+(i*3),2023+(i%2));}
ps("\nVogelae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;voge_market(t,c,1336+(i*9),1327+(i*7),1314+(i*5),1306+(i*3),2024);}
ps("\n");voge_report();voge_state();ps("\n=== Demo Complete ===\n");return 0;}
