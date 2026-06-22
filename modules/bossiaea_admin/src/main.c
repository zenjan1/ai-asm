/* bossiaea_admin: Bossiaea management technology administration (v1.0)
 * Bossiaea planning, bossiaea execution, bossiaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} boss_t;
typedef struct{int n_bossp,n_boss,n_boss2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} boss_state_t;
static boss_t bosss[N],bosse[N-2],boss2[N-4],bossac[N-6],bossam[N-6]; static boss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(boss_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;boss_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BOSS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int boss_init(void){if(init)return -1;st.n_bossp=0;st.n_boss=0;st.n_boss2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bosss[i].active=0;for(int i=0;i<N-2;i++)bosse[i].active=0;for(int i=0;i<N-4;i++)boss2[i].active=0;for(int i=0;i<N-6;i++)bossac[i].active=0;for(int i=0;i<N-6;i++)bossam[i].active=0;init=1;ps("[BOSS] Bossiaea initialized\n");return 0;}
int boss_planning(int t,int c,int a,int b,int d,int e,int y){return add(bosss,&st.n_bossp,&st.t_f1,N,t,c,a,b,d,e,y);}
int boss_execution(int t,int c,int a,int b,int d,int e,int y){return add(bosse,&st.n_boss,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int boss_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(boss2,&st.n_boss2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int boss_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bossac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int boss_market(int t,int c,int a,int b,int d,int e,int y){return add(bossam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void boss_report(void){ps("[BOSS] Sp: ");pi(st.n_bossp);ps(" PCS=");pi(st.t_f1);ps("\nSse: ");pi(st.n_boss);ps(" PCS=");pi(st.t_f2);ps("\nSs2: ");pi(st.n_boss2);ps(" PCS=");pi(st.t_f3);ps("\nSac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void boss_state(void){ps("[BOSS] Sp=");pi(st.n_bossp);ps(" Ss=");pi(st.n_boss);ps(" Ss2=");pi(st.n_boss2);ps(" Sac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bossiaea Admin Demo ===\n\n");boss_init();
ps("Bossiaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;boss_planning(t,c,1441+(i*17),1430+(i*14),1410+(i*10),1392+(i*6),2020+(i%5));}
ps("\nBossiaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;boss_execution(t,c,1430+(i*15),1419+(i*12),1401+(i*8),1388+(i*5),2021+(i%4));}
ps("\nBossiaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;boss_evaluation(t,c,1422+(i*13),1411+(i*10),1395+(i*7),1384+(i*4),2022+(i%3));}
ps("\nBossiaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;boss_accessory(t,c,1414+(i*11),1405+(i*9),1391+(i*6),1381+(i*3),2023+(i%2));}
ps("\nBossiaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;boss_market(t,c,1408+(i*9),1399+(i*7),1386+(i*5),1378+(i*3),2024);}
ps("\n");boss_report();boss_state();ps("\n=== Demo Complete ===\n");return 0;}
