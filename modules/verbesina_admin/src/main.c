/* verbesina_admin: Verbesina management technology administration (v1.0)
 * Verbesina planning, verbesina execution, verbesina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} verb_t;
typedef struct{int n_verbp,n_verbe,n_verb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} verb_state_t;
static verb_t verbs[N],verbess[N-2],verbvss[N-4],verbas[N-6],verbmks[N-6]; static verb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(verb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;verb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VERB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int verb_init(void){if(init)return -1;st.n_verbp=0;st.n_verbe=0;st.n_verb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)verbs[i].active=0;for(int i=0;i<N-2;i++)verbess[i].active=0;for(int i=0;i<N-4;i++)verbvss[i].active=0;for(int i=0;i<N-6;i++)verbas[i].active=0;for(int i=0;i<N-6;i++)verbmks[i].active=0;init=1;ps("[VERB] Verbesina initialized\n");return 0;}
int verb_planning(int t,int c,int a,int b,int d,int e,int y){return add(verbs,&st.n_verbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int verb_execution(int t,int c,int a,int b,int d,int e,int y){return add(verbess,&st.n_verbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int verb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(verbvss,&st.n_verb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int verb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(verbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int verb_market(int t,int c,int a,int b,int d,int e,int y){return add(verbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void verb_report(void){ps("[VERB] Verbp: ");pi(st.n_verbp);ps(" PCS=");pi(st.t_f1);ps("\nVerbe: ");pi(st.n_verbe);ps(" PCS=");pi(st.t_f2);ps("\nVerb2: ");pi(st.n_verb2);ps(" PCS=");pi(st.t_f3);ps("\nVerbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void verb_state(void){ps("[VERB] Verbp=");pi(st.n_verbp);ps(" Verbe=");pi(st.n_verbe);ps(" Verb2=");pi(st.n_verb2);ps(" Verbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Verbesina Admin Demo ===\n\n");verb_init();
ps("Verbesina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;verb_planning(t,c,1355+(i*17),1344+(i*14),1324+(i*10),1306+(i*6),2020+(i%5));}
ps("\nVerbesina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;verb_execution(t,c,1344+(i*15),1333+(i*12),1315+(i*8),1302+(i*5),2021+(i%4));}
ps("\nVerbesina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;verb_evaluation(t,c,1336+(i*13),1325+(i*10),1309+(i*7),1298+(i*4),2022+(i%3));}
ps("\nVerbesina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;verb_accessory(t,c,1328+(i*11),1319+(i*9),1305+(i*6),1295+(i*3),2023+(i%2));}
ps("\nVerbesina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;verb_market(t,c,1322+(i*9),1313+(i*7),1300+(i*5),1292+(i*3),2024);}
ps("\n");verb_report();verb_state();ps("\n=== Demo Complete ===\n");return 0;}
