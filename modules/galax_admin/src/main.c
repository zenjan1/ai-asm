/* galax_admin: Galax management technology administration (v1.0)
 * Galax planning, galax execution, galax evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gala_t;
typedef struct{int n_galap,n_gala,n_gala2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gala_state_t;
static gala_t galas[N],galae[N-2],gala2[N-4],galaac[N-6],galaam[N-6]; static gala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gala_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gala_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GALA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gala_init(void){if(init)return -1;st.n_galap=0;st.n_gala=0;st.n_gala2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)galas[i].active=0;for(int i=0;i<N-2;i++)galae[i].active=0;for(int i=0;i<N-4;i++)gala2[i].active=0;for(int i=0;i<N-6;i++)galaac[i].active=0;for(int i=0;i<N-6;i++)galaam[i].active=0;init=1;ps("[GALA] Galax initialized\n");return 0;}
int gala_planning(int t,int c,int a,int b,int d,int e,int y){return add(galas,&st.n_galap,&st.t_f1,N,t,c,a,b,d,e,y);}
int gala_execution(int t,int c,int a,int b,int d,int e,int y){return add(galae,&st.n_gala,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gala_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gala2,&st.n_gala2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gala_accessory(int t,int c,int a,int b,int d,int e,int y){return add(galaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gala_market(int t,int c,int a,int b,int d,int e,int y){return add(galaam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gala_report(void){ps("[GALA] Galp: ");pi(st.n_galap);ps(" PCS=");pi(st.t_f1);ps("\nGale: ");pi(st.n_gala);ps(" PCS=");pi(st.t_f2);ps("\nGal2: ");pi(st.n_gala2);ps(" PCS=");pi(st.t_f3);ps("\nGac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gala_state(void){ps("[GALA] Galp=");pi(st.n_galap);ps(" Gal=");pi(st.n_gala);ps(" Gal2=");pi(st.n_gala2);ps(" Gac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Galax Admin Demo ===\n\n");gala_init();
ps("Galax planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gala_planning(t,c,1516+(i*17),1505+(i*14),1485+(i*10),1467+(i*6),2020+(i%5));}
ps("\nGalax execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gala_execution(t,c,1505+(i*15),1494+(i*12),1476+(i*8),1463+(i*5),2021+(i%4));}
ps("\nGalax evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gala_evaluation(t,c,1497+(i*13),1486+(i*10),1470+(i*7),1459+(i*4),2022+(i%3));}
ps("\nGalax accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gala_accessory(t,c,1489+(i*11),1480+(i*9),1466+(i*6),1456+(i*3),2023+(i%2));}
ps("\nGalax marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gala_market(t,c,1483+(i*9),1474+(i*7),1461+(i*5),1453+(i*3),2024);}
ps("\n");gala_report();gala_state();ps("\n=== Demo Complete ===\n");return 0;}
