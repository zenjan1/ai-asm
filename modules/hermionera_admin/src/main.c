/* hermionera_admin: Hermionera management technology administration (v1.0)
 * Hermionera planning, hermionera execution, hermionera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} herm_t;
typedef struct{int n_hermp,n_herm,n_herm2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} herm_state_t;
static herm_t herms[N],herme[N-2],herm2[N-4],hermac[N-6],hermam[N-6]; static herm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(herm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;herm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HERM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int herm_init(void){if(init)return -1;st.n_hermp=0;st.n_herm=0;st.n_herm2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)herms[i].active=0;for(int i=0;i<N-2;i++)herme[i].active=0;for(int i=0;i<N-4;i++)herm2[i].active=0;for(int i=0;i<N-6;i++)hermac[i].active=0;for(int i=0;i<N-6;i++)hermam[i].active=0;init=1;ps("[HERM] Hermionera initialized\n");return 0;}
int herm_planning(int t,int c,int a,int b,int d,int e,int y){return add(herms,&st.n_hermp,&st.t_f1,N,t,c,a,b,d,e,y);}
int herm_execution(int t,int c,int a,int b,int d,int e,int y){return add(herme,&st.n_herm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int herm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(herm2,&st.n_herm2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int herm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hermac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int herm_market(int t,int c,int a,int b,int d,int e,int y){return add(hermam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void herm_report(void){ps("[HERM] Hermp: ");pi(st.n_hermp);ps(" PCS=");pi(st.t_f1);ps("\nHerme: ");pi(st.n_herm);ps(" PCS=");pi(st.t_f2);ps("\nHerm2: ");pi(st.n_herm2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void herm_state(void){ps("[HERM] Hermp=");pi(st.n_hermp);ps(" Herm=");pi(st.n_herm);ps(" Herm2=");pi(st.n_herm2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hermionera Admin Demo ===\n\n");herm_init();
ps("Hermionera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;herm_planning(t,c,1530+(i*17),1519+(i*14),1499+(i*10),1481+(i*6),2020+(i%5));}
ps("\nHermionera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;herm_execution(t,c,1519+(i*15),1508+(i*12),1490+(i*8),1477+(i*5),2021+(i%4));}
ps("\nHermionera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;herm_evaluation(t,c,1511+(i*13),1500+(i*10),1484+(i*7),1473+(i*4),2022+(i%3));}
ps("\nHermionera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;herm_accessory(t,c,1503+(i*11),1494+(i*9),1480+(i*6),1470+(i*3),2023+(i%2));}
ps("\nHermionera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;herm_market(t,c,1497+(i*9),1488+(i*7),1475+(i*5),1467+(i*3),2024);}
ps("\n");herm_report();herm_state();ps("\n=== Demo Complete ===\n");return 0;}
