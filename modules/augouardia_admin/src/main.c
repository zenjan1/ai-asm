/* augouardia_admin: Augouardia management technology administration (v1.0)
 * Augouardia planning, augouardia execution, augouardia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} augo_t;
typedef struct{int n_augop,n_augo,n_augo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} augo_state_t;
static augo_t augos[N],augoe[N-2],augo2[N-4],augoac[N-6],augoam[N-6]; static augo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(augo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;augo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AUGO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int augo_init(void){if(init)return -1;st.n_augop=0;st.n_augo=0;st.n_augo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)augos[i].active=0;for(int i=0;i<N-2;i++)augoe[i].active=0;for(int i=0;i<N-4;i++)augo2[i].active=0;for(int i=0;i<N-6;i++)augoac[i].active=0;for(int i=0;i<N-6;i++)augoam[i].active=0;init=1;ps("[AUGO] Augouardia initialized\n");return 0;}
int augo_planning(int t,int c,int a,int b,int d,int e,int y){return add(augos,&st.n_augop,&st.t_f1,N,t,c,a,b,d,e,y);}
int augo_execution(int t,int c,int a,int b,int d,int e,int y){return add(augoe,&st.n_augo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int augo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(augo2,&st.n_augo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int augo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(augoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int augo_market(int t,int c,int a,int b,int d,int e,int y){return add(augoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void augo_report(void){ps("[AUGO] Gop: ");pi(st.n_augop);ps(" PCS=");pi(st.t_f1);ps("\nGoes: ");pi(st.n_augo);ps(" PCS=");pi(st.t_f2);ps("\nGo2: ");pi(st.n_augo2);ps(" PCS=");pi(st.t_f3);ps("\nGoac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void augo_state(void){ps("[AUGO] Gop=");pi(st.n_augop);ps(" Goe=");pi(st.n_augo);ps(" Go2=");pi(st.n_augo2);ps(" Goac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Augouardia Admin Demo ===\n\n");augo_init();
ps("Augouardia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;augo_planning(t,c,1422+(i*17),1411+(i*14),1391+(i*10),1373+(i*6),2020+(i%5));}
ps("\nAugouardia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;augo_execution(t,c,1411+(i*15),1400+(i*12),1382+(i*8),1369+(i*5),2021+(i%4));}
ps("\nAugouardia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;augo_evaluation(t,c,1403+(i*13),1392+(i*10),1376+(i*7),1365+(i*4),2022+(i%3));}
ps("\nAugouardia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;augo_accessory(t,c,1395+(i*11),1386+(i*9),1372+(i*6),1362+(i*3),2023+(i%2));}
ps("\nAugouardia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;augo_market(t,c,1389+(i*9),1380+(i*7),1367+(i*5),1359+(i*3),2024);}
ps("\n");augo_report();augo_state();ps("\n=== Demo Complete ===\n");return 0;}
