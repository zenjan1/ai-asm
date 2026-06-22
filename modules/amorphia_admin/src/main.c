/* amorphia_admin: Amorphia management technology administration (v1.0)
 * Amorphia planning, amorphia execution, amorphia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} amor_t;
typedef struct{int n_amorp,n_amor,n_amor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} amor_state_t;
static amor_t amors[N],amore[N-2],amor2[N-4],amorac[N-6],amoram[N-6]; static amor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(amor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;amor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int amor_init(void){if(init)return -1;st.n_amorp=0;st.n_amor=0;st.n_amor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)amors[i].active=0;for(int i=0;i<N-2;i++)amore[i].active=0;for(int i=0;i<N-4;i++)amor2[i].active=0;for(int i=0;i<N-6;i++)amorac[i].active=0;for(int i=0;i<N-6;i++)amoram[i].active=0;init=1;ps("[AMOR] Amorphia initialized\n");return 0;}
int amor_planning(int t,int c,int a,int b,int d,int e,int y){return add(amors,&st.n_amorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int amor_execution(int t,int c,int a,int b,int d,int e,int y){return add(amore,&st.n_amor,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int amor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(amor2,&st.n_amor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int amor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(amorac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int amor_market(int t,int c,int a,int b,int d,int e,int y){return add(amoram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void amor_report(void){ps("[AMOR] Morp: ");pi(st.n_amorp);ps(" PCS=");pi(st.t_f1);ps("\nMore: ");pi(st.n_amor);ps(" PCS=");pi(st.t_f2);ps("\nMor2: ");pi(st.n_amor2);ps(" PCS=");pi(st.t_f3);ps("\nMorac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void amor_state(void){ps("[AMOR] Morp=");pi(st.n_amorp);ps(" More=");pi(st.n_amor);ps(" Mor2=");pi(st.n_amor2);ps(" Morac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Amorphia Admin Demo ===\n\n");amor_init();
ps("Amorphia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;amor_planning(t,c,1403+(i*17),1392+(i*14),1372+(i*10),1354+(i*6),2020+(i%5));}
ps("\nAmorphia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;amor_execution(t,c,1392+(i*15),1381+(i*12),1363+(i*8),1350+(i*5),2021+(i%4));}
ps("\nAmorphia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;amor_evaluation(t,c,1384+(i*13),1373+(i*10),1357+(i*7),1346+(i*4),2022+(i%3));}
ps("\nAmorphia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amor_accessory(t,c,1376+(i*11),1367+(i*9),1353+(i*6),1343+(i*3),2023+(i%2));}
ps("\nAmorphia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;amor_market(t,c,1370+(i*9),1361+(i*7),1348+(i*5),1340+(i*3),2024);}
ps("\n");amor_report();amor_state();ps("\n=== Demo Complete ===\n");return 0;}
