/* zizaniopsis_admin: Zizaniopsis management technology administration (v1.0)
 * Zizaniopsis planning, zizaniopsis execution, zizaniopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ziza_t;
typedef struct{int n_zizap,n_ziza,n_ziza2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ziza_state_t;
static ziza_t zizas[N],zizae[N-2],zizav[N-4],zizaac[N-6],zizam[N-6]; static ziza_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ziza_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ziza_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZIZA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ziza_init(void){if(init)return -1;st.n_zizap=0;st.n_ziza=0;st.n_ziza2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zizas[i].active=0;for(int i=0;i<N-2;i++)zizae[i].active=0;for(int i=0;i<N-4;i++)zizav[i].active=0;for(int i=0;i<N-6;i++)zizaac[i].active=0;for(int i=0;i<N-6;i++)zizam[i].active=0;init=1;ps("[ZIZA] Zizaniopsis initialized\n");return 0;}
int ziza_planning(int t,int c,int a,int b,int d,int e,int y){return add(zizas,&st.n_zizap,&st.t_f1,N,t,c,a,b,d,e,y);}
int ziza_execution(int t,int c,int a,int b,int d,int e,int y){return add(zizae,&st.n_ziza,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ziza_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zizav,&st.n_ziza2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ziza_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zizaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ziza_market(int t,int c,int a,int b,int d,int e,int y){return add(zizam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ziza_report(void){ps("[ZIZA] Zizap: ");pi(st.n_zizap);ps(" PCS=");pi(st.t_f1);ps("\nZizae: ");pi(st.n_ziza);ps(" PCS=");pi(st.t_f2);ps("\nZizav: ");pi(st.n_ziza2);ps(" PCS=");pi(st.t_f3);ps("\nZizac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ziza_state(void){ps("[ZIZA] Zizap=");pi(st.n_zizap);ps(" Zizae=");pi(st.n_ziza);ps(" Zizav=");pi(st.n_ziza2);ps(" Zizac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zizaniopsis Admin Demo ===\n\n");ziza_init();
ps("Zizaniopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ziza_planning(t,c,1380+(i*17),1369+(i*14),1349+(i*10),1331+(i*6),2020+(i%5));}
ps("\nZizaniopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ziza_execution(t,c,1369+(i*15),1358+(i*12),1340+(i*8),1327+(i*5),2021+(i%4));}
ps("\nZizaniopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ziza_evaluation(t,c,1361+(i*13),1350+(i*10),1334+(i*7),1323+(i*4),2022+(i%3));}
ps("\nZizaniopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ziza_accessory(t,c,1353+(i*11),1344+(i*9),1330+(i*6),1320+(i*3),2023+(i%2));}
ps("\nZizaniopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ziza_market(t,c,1347+(i*9),1338+(i*7),1325+(i*5),1317+(i*3),2024);}
ps("\n");ziza_report();ziza_state();ps("\n=== Demo Complete ===\n");return 0;}
