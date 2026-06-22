/* cicuta_admin: Cicuta management technology administration (v1.0)
 * Cicuta planning, cicuta execution, cicuta evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} icu_t;
typedef struct{int n_icup,n_icue,n_icu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} icu_state_t;
static icu_t icups[N],icuss[N-2],icuvss[N-4],icuas[N-6],icumks[N-6]; static icu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(icu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;icu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ICU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int icu_init(void){if(init)return -1;st.n_icup=0;st.n_icue=0;st.n_icu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)icups[i].active=0;for(int i=0;i<N-2;i++)icuss[i].active=0;for(int i=0;i<N-4;i++)icuvss[i].active=0;for(int i=0;i<N-6;i++)icuas[i].active=0;for(int i=0;i<N-6;i++)icumks[i].active=0;init=1;ps("[ICU] Cicuta initialized\n");return 0;}
int icu_planning(int t,int c,int a,int b,int d,int e,int y){return add(icups,&st.n_icup,&st.t_f1,N,t,c,a,b,d,e,y);}
int icu_execution(int t,int c,int a,int b,int d,int e,int y){return add(icuss,&st.n_icue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int icu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(icuvss,&st.n_icu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int icu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(icuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int icu_market(int t,int c,int a,int b,int d,int e,int y){return add(icumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void icu_report(void){ps("[ICU] Cicpp: ");pi(st.n_icup);ps(" PCS=");pi(st.t_f1);ps("\nCicpe: ");pi(st.n_icue);ps(" PCS=");pi(st.t_f2);ps("\nCicv: ");pi(st.n_icu2);ps(" PCS=");pi(st.t_f3);ps("\nCicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void icu_state(void){ps("[ICU] Cicpp=");pi(st.n_icup);ps(" Cicpe=");pi(st.n_icue);ps(" Cicv=");pi(st.n_icu2);ps(" Cicc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cicuta Admin Demo ===\n\n");icu_init();
ps("Cicuta planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;icu_planning(t,c,1237+(i*17),1226+(i*14),1206+(i*10),1188+(i*6),2020+(i%5));}
ps("\nCicuta execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;icu_execution(t,c,1226+(i*15),1215+(i*12),1197+(i*8),1184+(i*5),2021+(i%4));}
ps("\nCicuta evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;icu_evaluation(t,c,1218+(i*13),1207+(i*10),1191+(i*7),1180+(i*4),2022+(i%3));}
ps("\nCicuta accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;icu_accessory(t,c,1210+(i*11),1201+(i*9),1187+(i*6),1177+(i*3),2023+(i%2));}
ps("\nCicuta marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;icu_market(t,c,1204+(i*9),1195+(i*7),1182+(i*5),1174+(i*3),2024);}
ps("\n");icu_report();icu_state();ps("\n=== Demo Complete ===\n");return 0;}
