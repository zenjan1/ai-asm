/* avicennia_admin: Avicennia management technology administration (v1.0)
 * Avicennia planning, avicennia execution, avicennia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} avic_t;
typedef struct{int n_avicp,n_avic,n_avic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} avic_state_t;
static avic_t avics[N],avice[N-2],avic2[N-4],avicac[N-6],avicam[N-6]; static avic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(avic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;avic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AVIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int avic_init(void){if(init)return -1;st.n_avicp=0;st.n_avic=0;st.n_avic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)avics[i].active=0;for(int i=0;i<N-2;i++)avice[i].active=0;for(int i=0;i<N-4;i++)avic2[i].active=0;for(int i=0;i<N-6;i++)avicac[i].active=0;for(int i=0;i<N-6;i++)avicam[i].active=0;init=1;ps("[AVIC] Avicennia initialized\n");return 0;}
int avic_planning(int t,int c,int a,int b,int d,int e,int y){return add(avics,&st.n_avicp,&st.t_f1,N,t,c,a,b,d,e,y);}
int avic_execution(int t,int c,int a,int b,int d,int e,int y){return add(avice,&st.n_avic,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int avic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(avic2,&st.n_avic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int avic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(avicac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int avic_market(int t,int c,int a,int b,int d,int e,int y){return add(avicam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void avic_report(void){ps("[AVIC] Vicp: ");pi(st.n_avicp);ps(" PCS=");pi(st.t_f1);ps("\nVices: ");pi(st.n_avic);ps(" PCS=");pi(st.t_f2);ps("\nVic2: ");pi(st.n_avic2);ps(" PCS=");pi(st.t_f3);ps("\nVicac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void avic_state(void){ps("[AVIC] Vicp=");pi(st.n_avicp);ps(" Vices=");pi(st.n_avic);ps(" Vic2=");pi(st.n_avic2);ps(" Vicac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Avicennia Admin Demo ===\n\n");avic_init();
ps("Avicennia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;avic_planning(t,c,1424+(i*17),1413+(i*14),1393+(i*10),1375+(i*6),2020+(i%5));}
ps("\nAvicennia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;avic_execution(t,c,1413+(i*15),1402+(i*12),1384+(i*8),1371+(i*5),2021+(i%4));}
ps("\nAvicennia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;avic_evaluation(t,c,1405+(i*13),1394+(i*10),1378+(i*7),1367+(i*4),2022+(i%3));}
ps("\nAvicennia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;avic_accessory(t,c,1397+(i*11),1388+(i*9),1374+(i*6),1364+(i*3),2023+(i%2));}
ps("\nAvicennia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;avic_market(t,c,1391+(i*9),1382+(i*7),1369+(i*5),1361+(i*3),2024);}
ps("\n");avic_report();avic_state();ps("\n=== Demo Complete ===\n");return 0;}
