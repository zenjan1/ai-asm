/* anredera_admin: Anredera management technology administration (v1.0)
 * Anredera planning, anredera execution, anredera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anre_t;
typedef struct{int n_anrep,n_anre,n_anre2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anre_state_t;
static anre_t anres[N],anree[N-2],anre2[N-4],anreac[N-6],anream[N-6]; static anre_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(anre_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anre_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANRE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anre_init(void){if(init)return -1;st.n_anrep=0;st.n_anre=0;st.n_anre2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anres[i].active=0;for(int i=0;i<N-2;i++)anree[i].active=0;for(int i=0;i<N-4;i++)anre2[i].active=0;for(int i=0;i<N-6;i++)anreac[i].active=0;for(int i=0;i<N-6;i++)anream[i].active=0;init=1;ps("[ANRE] Anredera initialized\n");return 0;}
int anre_planning(int t,int c,int a,int b,int d,int e,int y){return add(anres,&st.n_anrep,&st.t_f1,N,t,c,a,b,d,e,y);}
int anre_execution(int t,int c,int a,int b,int d,int e,int y){return add(anree,&st.n_anre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anre_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anre2,&st.n_anre2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anre_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anreac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anre_market(int t,int c,int a,int b,int d,int e,int y){return add(anream,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anre_report(void){ps("[ANRE] Nrep: ");pi(st.n_anrep);ps(" PCS=");pi(st.t_f1);ps("\nNree: ");pi(st.n_anre);ps(" PCS=");pi(st.t_f2);ps("\nNre2: ");pi(st.n_anre2);ps(" PCS=");pi(st.t_f3);ps("\nNreac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anre_state(void){ps("[ANRE] Nrep=");pi(st.n_anrep);ps(" Nree=");pi(st.n_anre);ps(" Nre2=");pi(st.n_anre2);ps(" Nreac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anredera Admin Demo ===\n\n");anre_init();
ps("Anredera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anre_planning(t,c,1409+(i*17),1398+(i*14),1378+(i*10),1360+(i*6),2020+(i%5));}
ps("\nAnredera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anre_execution(t,c,1398+(i*15),1387+(i*12),1369+(i*8),1356+(i*5),2021+(i%4));}
ps("\nAnredera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anre_evaluation(t,c,1390+(i*13),1379+(i*10),1363+(i*7),1352+(i*4),2022+(i%3));}
ps("\nAnredera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anre_accessory(t,c,1382+(i*11),1373+(i*9),1359+(i*6),1349+(i*3),2023+(i%2));}
ps("\nAnredera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anre_market(t,c,1376+(i*9),1367+(i*7),1354+(i*5),1346+(i*3),2024);}
ps("\n");anre_report();anre_state();ps("\n=== Demo Complete ===\n");return 0;}
