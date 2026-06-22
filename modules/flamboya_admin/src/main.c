/* flamboya_admin: Flamboya management technology administration (v1.0)
 * Flamboya planning, flamboya execution, flamboya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flam_t;
typedef struct{int n_flamp,n_flam,n_flam2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flam_state_t;
static flam_t flams[N],flame[N-2],flam2[N-4],flamac[N-6],flamam[N-6]; static flam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(flam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flam_init(void){if(init)return -1;st.n_flamp=0;st.n_flam=0;st.n_flam2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)flams[i].active=0;for(int i=0;i<N-2;i++)flame[i].active=0;for(int i=0;i<N-4;i++)flam2[i].active=0;for(int i=0;i<N-6;i++)flamac[i].active=0;for(int i=0;i<N-6;i++)flamam[i].active=0;init=1;ps("[FLAM] Flamboya initialized\n");return 0;}
int flam_planning(int t,int c,int a,int b,int d,int e,int y){return add(flams,&st.n_flamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int flam_execution(int t,int c,int a,int b,int d,int e,int y){return add(flame,&st.n_flam,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flam_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(flam2,&st.n_flam2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flam_accessory(int t,int c,int a,int b,int d,int e,int y){return add(flamac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flam_market(int t,int c,int a,int b,int d,int e,int y){return add(flamam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flam_report(void){ps("[FLAM] Amp: ");pi(st.n_flamp);ps(" PCS=");pi(st.t_f1);ps("\nAme: ");pi(st.n_flam);ps(" PCS=");pi(st.t_f2);ps("\nAm2: ");pi(st.n_flam2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flam_state(void){ps("[FLAM] Amp=");pi(st.n_flamp);ps(" Am=");pi(st.n_flam);ps(" Am2=");pi(st.n_flam2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flamboya Admin Demo ===\n\n");flam_init();
ps("Flamboya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flam_planning(t,c,1514+(i*17),1503+(i*14),1483+(i*10),1465+(i*6),2020+(i%5));}
ps("\nFlamboya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flam_execution(t,c,1503+(i*15),1492+(i*12),1474+(i*8),1461+(i*5),2021+(i%4));}
ps("\nFlamboya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flam_evaluation(t,c,1495+(i*13),1484+(i*10),1468+(i*7),1457+(i*4),2022+(i%3));}
ps("\nFlamboya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flam_accessory(t,c,1487+(i*11),1478+(i*9),1464+(i*6),1454+(i*3),2023+(i%2));}
ps("\nFlamboya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flam_market(t,c,1481+(i*9),1472+(i*7),1459+(i*5),1451+(i*3),2024);}
ps("\n");flam_report();flam_state();ps("\n=== Demo Complete ===\n");return 0;}
