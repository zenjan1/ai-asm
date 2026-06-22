/* bacharis_admin: Bacharis management technology administration (v1.0)
 * Bacharis planning, bacharis execution, bacharis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bach_t;
typedef struct{int n_bachp,n_bach,n_bach2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bach_state_t;
static bach_t bachs[N],bache[N-2],bach2[N-4],bachac[N-6],bacham[N-6]; static bach_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bach_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bach_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BACH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bach_init(void){if(init)return -1;st.n_bachp=0;st.n_bach=0;st.n_bach2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bachs[i].active=0;for(int i=0;i<N-2;i++)bache[i].active=0;for(int i=0;i<N-4;i++)bach2[i].active=0;for(int i=0;i<N-6;i++)bachac[i].active=0;for(int i=0;i<N-6;i++)bacham[i].active=0;init=1;ps("[BACH] Bacharis initialized\n");return 0;}
int bach_planning(int t,int c,int a,int b,int d,int e,int y){return add(bachs,&st.n_bachp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bach_execution(int t,int c,int a,int b,int d,int e,int y){return add(bache,&st.n_bach,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bach_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bach2,&st.n_bach2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bach_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bachac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bach_market(int t,int c,int a,int b,int d,int e,int y){return add(bacham,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bach_report(void){ps("[BACH] Hcp: ");pi(st.n_bachp);ps(" PCS=");pi(st.t_f1);ps("\nHce: ");pi(st.n_bach);ps(" PCS=");pi(st.t_f2);ps("\nHc2: ");pi(st.n_bach2);ps(" PCS=");pi(st.t_f3);ps("\nHcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bach_state(void){ps("[BACH] Hcp=");pi(st.n_bachp);ps(" Hce=");pi(st.n_bach);ps(" Hc2=");pi(st.n_bach2);ps(" Hcac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bacharis Admin Demo ===\n\n");bach_init();
ps("Bacharis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bach_planning(t,c,1427+(i*17),1416+(i*14),1396+(i*10),1378+(i*6),2020+(i%5));}
ps("\nBacharis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bach_execution(t,c,1416+(i*15),1405+(i*12),1387+(i*8),1374+(i*5),2021+(i%4));}
ps("\nBacharis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bach_evaluation(t,c,1408+(i*13),1397+(i*10),1381+(i*7),1370+(i*4),2022+(i%3));}
ps("\nBacharis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bach_accessory(t,c,1400+(i*11),1391+(i*9),1377+(i*6),1367+(i*3),2023+(i%2));}
ps("\nBacharis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bach_market(t,c,1394+(i*9),1385+(i*7),1372+(i*5),1364+(i*3),2024);}
ps("\n");bach_report();bach_state();ps("\n=== Demo Complete ===\n");return 0;}
