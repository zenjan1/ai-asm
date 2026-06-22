/* alpae_admin: Alpae management technology administration (v1.0)
 * Alpae planning, alpae execution, alpae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alpa_t;
typedef struct{int n_alpap,n_alpa,n_alpa2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alpa_state_t;
static alpa_t alpas[N],alpae[N-2],alpa2[N-4],alpaac[N-6],alpam[N-6]; static alpa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(alpa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alpa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALPA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alpa_init(void){if(init)return -1;st.n_alpap=0;st.n_alpa=0;st.n_alpa2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)alpas[i].active=0;for(int i=0;i<N-2;i++)alpae[i].active=0;for(int i=0;i<N-4;i++)alpa2[i].active=0;for(int i=0;i<N-6;i++)alpaac[i].active=0;for(int i=0;i<N-6;i++)alpam[i].active=0;init=1;ps("[ALPA] Alpae initialized\n");return 0;}
int alpa_planning(int t,int c,int a,int b,int d,int e,int y){return add(alpas,&st.n_alpap,&st.t_f1,N,t,c,a,b,d,e,y);}
int alpa_execution(int t,int c,int a,int b,int d,int e,int y){return add(alpae,&st.n_alpa,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alpa_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alpa2,&st.n_alpa2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alpa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(alpaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alpa_market(int t,int c,int a,int b,int d,int e,int y){return add(alpam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alpa_report(void){ps("[ALPA] Pap: ");pi(st.n_alpap);ps(" PCS=");pi(st.t_f1);ps("\nPae: ");pi(st.n_alpa);ps(" PCS=");pi(st.t_f2);ps("\nPa2: ");pi(st.n_alpa2);ps(" PCS=");pi(st.t_f3);ps("\nPac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alpa_state(void){ps("[ALPA] Pap=");pi(st.n_alpap);ps(" Pae=");pi(st.n_alpa);ps(" Pa2=");pi(st.n_alpa2);ps(" Pac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alpae Admin Demo ===\n\n");alpa_init();
ps("Alpae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alpa_planning(t,c,1399+(i*17),1388+(i*14),1368+(i*10),1350+(i*6),2020+(i%5));}
ps("\nAlpae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alpa_execution(t,c,1388+(i*15),1377+(i*12),1359+(i*8),1346+(i*5),2021+(i%4));}
ps("\nAlpae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alpa_evaluation(t,c,1380+(i*13),1369+(i*10),1353+(i*7),1342+(i*4),2022+(i%3));}
ps("\nAlpae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alpa_accessory(t,c,1372+(i*11),1363+(i*9),1349+(i*6),1339+(i*3),2023+(i%2));}
ps("\nAlpae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alpa_market(t,c,1366+(i*9),1357+(i*7),1344+(i*5),1336+(i*3),2024);}
ps("\n");alpa_report();alpa_state();ps("\n=== Demo Complete ===\n");return 0;}
