/* marcetia_admin: Marcetia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} marc_t;
typedef struct{int n_marop,n_marc,n_marc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} marc_state_t;
static marc_t marcos[N],marce[N-2],marc2[N-4],marcac[N-6],marcam[N-6]; static marc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(marc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;marc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MARC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int marc_init(void){if(init)return -1;st.n_marop=0;st.n_marc=0;st.n_marc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)marcos[i].active=0;for(int i=0;i<N-2;i++)marce[i].active=0;for(int i=0;i<N-4;i++)marc2[i].active=0;for(int i=0;i<N-6;i++)marcac[i].active=0;for(int i=0;i<N-6;i++)marcam[i].active=0;init=1;ps("[MARC] Marcetia initialized\n");return 0;}
int marc_planning(int t,int c,int a,int b,int d,int e,int y){return add(marcos,&st.n_marop,&st.t_f1,N,t,c,a,b,d,e,y);}
int marc_execution(int t,int c,int a,int b,int d,int e,int y){return add(marce,&st.n_marc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int marc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(marc2,&st.n_marc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int marc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(marcac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int marc_market(int t,int c,int a,int b,int d,int e,int y){return add(marcam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void marc_report(void){ps("[MARC] Marcp: ");pi(st.n_marop);ps(" PCS=");pi(st.t_f1);ps("\nMarce: ");pi(st.n_marc);ps(" PCS=");pi(st.t_f2);ps("\nMarc2: ");pi(st.n_marc2);ps(" PCS=");pi(st.t_f3);ps("\nMrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void marc_state(void){ps("[MARC] Marcp=");pi(st.n_marop);ps(" Marc=");pi(st.n_marc);ps(" Rcc2=");pi(st.n_marc2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Marcetia Admin Demo ===\n\n");marc_init();
ps("Marcetia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;marc_planning(t,c,1564-(i*17),1553-(i*14),1533-(i*10),1515-(i*6),2020+(i%5));}
ps("\nMarcetia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;marc_execution(t,c,1553-(i*15),1542-(i*12),1524-(i*8),1511-(i*5),2021+(i%4));}
ps("\nMarcetia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;marc_evaluation(t,c,1545-(i*13),1534-(i*10),1518-(i*7),1507-(i*4),2022+(i%3));}
ps("\nMarcetia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;marc_accessory(t,c,1537-(i*11),1528-(i*9),1514-(i*6),1504-(i*3),2023+(i%2));}
ps("\nMarcetia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;marc_market(t,c,1531-(i*9),1522-(i*7),1509-(i*5),1501-(i*3),2024);}
ps("\n");marc_report();marc_state();ps("\n=== Demo Complete ===\n");return 0;}
