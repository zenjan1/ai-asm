/* zea_admin: Zea management technology administration (v1.0)
 * Zea planning, zea execution, zea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zea_t;
typedef struct{int n_zep,n_zee,n_zev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zea_state_t;
static zea_t zeaps[N],zeass[N-2],zeavss[N-4],zeas[N-6],zeamks[N-6]; static zea_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zea_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zea_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZEA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zea_init(void){if(init)return -1;st.n_zep=0;st.n_zee=0;st.n_zev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zeaps[i].active=0;for(int i=0;i<N-2;i++)zeass[i].active=0;for(int i=0;i<N-4;i++)zeavss[i].active=0;for(int i=0;i<N-6;i++)zeas[i].active=0;for(int i=0;i<N-6;i++)zeamks[i].active=0;init=1;ps("[ZEA] Zea initialized\n");return 0;}
int zea_planning(int t,int c,int a,int b,int d,int e,int y){return add(zeaps,&st.n_zep,&st.t_f1,N,t,c,a,b,d,e,y);}
int zea_execution(int t,int c,int a,int b,int d,int e,int y){return add(zeass,&st.n_zee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zea_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zeavss,&st.n_zev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zea_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zea_market(int t,int c,int a,int b,int d,int e,int y){return add(zeamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zea_report(void){ps("[ZEA] Zep: ");pi(st.n_zep);ps(" PCS=");pi(st.t_f1);ps("\nZee: ");pi(st.n_zee);ps(" PCS=");pi(st.t_f2);ps("\nZev: ");pi(st.n_zev);ps(" PCS=");pi(st.t_f3);ps("\nZec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zea_state(void){ps("[ZEA] Zep=");pi(st.n_zep);ps(" Zee=");pi(st.n_zee);ps(" Zev=");pi(st.n_zev);ps(" Zec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zea Admin Demo ===\n\n");zea_init();
ps("Zea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zea_planning(t,c,880+(i*17),869+(i*14),849+(i*10),831+(i*6),2020+(i%5));}
ps("\nZea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zea_execution(t,c,869+(i*15),858+(i*12),840+(i*8),827+(i*5),2021+(i%4));}
ps("\nZea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zea_evaluation(t,c,861+(i*13),850+(i*10),834+(i*7),823+(i*4),2022+(i%3));}
ps("\nZea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zea_accessory(t,c,853+(i*11),844+(i*9),830+(i*6),820+(i*3),2023+(i%2));}
ps("\nZea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zea_market(t,c,847+(i*9),838+(i*7),825+(i*5),817+(i*3),2024);}
ps("\n");zea_report();zea_state();ps("\n=== Demo Complete ===\n");return 0;}
