/* actaeae_admin: Actaeae management technology administration (v1.0)
 * Actaeae planning, actaeae execution, actaeae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acta_t;
typedef struct{int n_actap,n_acta,n_acta2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acta_state_t;
static acta_t actas[N],actae[N-2],actav[N-4],actaac[N-6],actam[N-6]; static acta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acta_init(void){if(init)return -1;st.n_actap=0;st.n_acta=0;st.n_acta2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)actas[i].active=0;for(int i=0;i<N-2;i++)actae[i].active=0;for(int i=0;i<N-4;i++)actav[i].active=0;for(int i=0;i<N-6;i++)actaac[i].active=0;for(int i=0;i<N-6;i++)actam[i].active=0;init=1;ps("[ACTA] Actaeae initialized\n");return 0;}
int acta_planning(int t,int c,int a,int b,int d,int e,int y){return add(actas,&st.n_actap,&st.t_f1,N,t,c,a,b,d,e,y);}
int acta_execution(int t,int c,int a,int b,int d,int e,int y){return add(actae,&st.n_acta,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acta_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(actav,&st.n_acta2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acta_accessory(int t,int c,int a,int b,int d,int e,int y){return add(actaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acta_market(int t,int c,int a,int b,int d,int e,int y){return add(actam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acta_report(void){ps("[ACTA] Actap: ");pi(st.n_actap);ps(" PCS=");pi(st.t_f1);ps("\nActae: ");pi(st.n_acta);ps(" PCS=");pi(st.t_f2);ps("\nActav: ");pi(st.n_acta2);ps(" PCS=");pi(st.t_f3);ps("\nActac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acta_state(void){ps("[ACTA] Actap=");pi(st.n_actap);ps(" Actae=");pi(st.n_acta);ps(" Actav=");pi(st.n_acta2);ps(" Actac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Actaeae Admin Demo ===\n\n");acta_init();
ps("Actaeae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acta_planning(t,c,1392+(i*17),1381+(i*14),1361+(i*10),1343+(i*6),2020+(i%5));}
ps("\nActaeae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acta_execution(t,c,1381+(i*15),1370+(i*12),1352+(i*8),1339+(i*5),2021+(i%4));}
ps("\nActaeae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acta_evaluation(t,c,1373+(i*13),1362+(i*10),1346+(i*7),1335+(i*4),2022+(i%3));}
ps("\nActaeae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acta_accessory(t,c,1365+(i*11),1356+(i*9),1342+(i*6),1332+(i*3),2023+(i%2));}
ps("\nActaeae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acta_market(t,c,1359+(i*9),1350+(i*7),1337+(i*5),1329+(i*3),2024);}
ps("\n");acta_report();acta_state();ps("\n=== Demo Complete ===\n");return 0;}
