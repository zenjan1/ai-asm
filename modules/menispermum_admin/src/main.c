/* menispermum_admin: Menispermum management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} meni_t;
typedef struct{int n_meniop,n_meni,n_meni2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} meni_state_t;
static meni_t menios[N],menie[N-2],meni2[N-4],meniac[N-6],meniam[N-6]; static meni_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(meni_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;meni_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MENI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int meni_init(void){if(init)return -1;st.n_meniop=0;st.n_meni=0;st.n_meni2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)menios[i].active=0;for(int i=0;i<N-2;i++)menie[i].active=0;for(int i=0;i<N-4;i++)meni2[i].active=0;for(int i=0;i<N-6;i++)meniac[i].active=0;for(int i=0;i<N-6;i++)meniam[i].active=0;init=1;ps("[MENI] Menispermum initialized\n");return 0;}
int meni_planning(int t,int c,int a,int b,int d,int e,int y){return add(menios,&st.n_meniop,&st.t_f1,N,t,c,a,b,d,e,y);}
int meni_execution(int t,int c,int a,int b,int d,int e,int y){return add(menie,&st.n_meni,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int meni_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(meni2,&st.n_meni2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int meni_accessory(int t,int c,int a,int b,int d,int e,int y){return add(meniac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int meni_market(int t,int c,int a,int b,int d,int e,int y){return add(meniam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void meni_report(void){ps("[MENI] Menip: ");pi(st.n_meniop);ps(" PCS=");pi(st.t_f1);ps("\nMenie: ");pi(st.n_meni);ps(" PCS=");pi(st.t_f2);ps("\nMeni2: ");pi(st.n_meni2);ps(" PCS=");pi(st.t_f3);ps("\nMni: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void meni_state(void){ps("[MENI] Menip=");pi(st.n_meniop);ps(" Meni=");pi(st.n_meni);ps(" Ni2=");pi(st.n_meni2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Menispermum Admin Demo ===\n\n");meni_init();
ps("Menispermum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;meni_planning(t,c,1569-(i*17),1558-(i*14),1538-(i*10),1520-(i*6),2020+(i%5));}
ps("\nMenispermum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;meni_execution(t,c,1558-(i*15),1547-(i*12),1529-(i*8),1516-(i*5),2021+(i%4));}
ps("\nMenispermum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;meni_evaluation(t,c,1550-(i*13),1539-(i*10),1523-(i*7),1512-(i*4),2022+(i%3));}
ps("\nMenispermum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;meni_accessory(t,c,1542-(i*11),1533-(i*9),1519-(i*6),1509-(i*3),2023+(i%2));}
ps("\nMenispermum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;meni_market(t,c,1536-(i*9),1527-(i*7),1514-(i*5),1506-(i*3),2024);}
ps("\n");meni_report();meni_state();ps("\n=== Demo Complete ===\n");return 0;}
