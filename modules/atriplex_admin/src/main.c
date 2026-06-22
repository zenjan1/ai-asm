/* atriplex_admin: Atriplex management technology administration (v1.0)
 * Atriplex planning, atriplex execution, atriplex evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} atri_t;
typedef struct{int n_atrip,n_atri,n_atri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} atri_state_t;
static atri_t atris[N],atrie[N-2],atri2[N-4],atriac[N-6],atriam[N-6]; static atri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(atri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;atri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ATRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int atri_init(void){if(init)return -1;st.n_atrip=0;st.n_atri=0;st.n_atri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)atris[i].active=0;for(int i=0;i<N-2;i++)atrie[i].active=0;for(int i=0;i<N-4;i++)atri2[i].active=0;for(int i=0;i<N-6;i++)atriac[i].active=0;for(int i=0;i<N-6;i++)atriam[i].active=0;init=1;ps("[ATRI] Atriplex initialized\n");return 0;}
int atri_planning(int t,int c,int a,int b,int d,int e,int y){return add(atris,&st.n_atrip,&st.t_f1,N,t,c,a,b,d,e,y);}
int atri_execution(int t,int c,int a,int b,int d,int e,int y){return add(atrie,&st.n_atri,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int atri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(atri2,&st.n_atri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int atri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(atriac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int atri_market(int t,int c,int a,int b,int d,int e,int y){return add(atriam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void atri_report(void){ps("[ATRI] Trip: ");pi(st.n_atrip);ps(" PCS=");pi(st.t_f1);ps("\nTrie: ");pi(st.n_atri);ps(" PCS=");pi(st.t_f2);ps("\nTri2: ");pi(st.n_atri2);ps(" PCS=");pi(st.t_f3);ps("\nTriac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void atri_state(void){ps("[ATRI] Trip=");pi(st.n_atrip);ps(" Trie=");pi(st.n_atri);ps(" Tri2=");pi(st.n_atri2);ps(" Triac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Atriplex Admin Demo ===\n\n");atri_init();
ps("Atriplex planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;atri_planning(t,c,1420+(i*17),1409+(i*14),1389+(i*10),1371+(i*6),2020+(i%5));}
ps("\nAtriplex execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;atri_execution(t,c,1409+(i*15),1398+(i*12),1380+(i*8),1367+(i*5),2021+(i%4));}
ps("\nAtriplex evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;atri_evaluation(t,c,1401+(i*13),1390+(i*10),1374+(i*7),1363+(i*4),2022+(i%3));}
ps("\nAtriplex accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atri_accessory(t,c,1393+(i*11),1384+(i*9),1370+(i*6),1360+(i*3),2023+(i%2));}
ps("\nAtriplex marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atri_market(t,c,1387+(i*9),1378+(i*7),1365+(i*5),1357+(i*3),2024);}
ps("\n");atri_report();atri_state();ps("\n=== Demo Complete ===\n");return 0;}
