/* tuberose_admin: Tuberose management technology administration (v1.0)
 * Tuberose planning, tuberose execution, tuberose evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tub_t;
typedef struct{int n_tbp,n_tbe,n_tbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tub_state_t;
static tub_t tubps[N],tubss[N-2],tubvss[N-4],tubas[N-6],tubmks[N-6]; static tub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TUB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tub_init(void){if(init)return -1;st.n_tbp=0;st.n_tbe=0;st.n_tbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tubps[i].active=0;for(int i=0;i<N-2;i++)tubss[i].active=0;for(int i=0;i<N-4;i++)tubvss[i].active=0;for(int i=0;i<N-6;i++)tubas[i].active=0;for(int i=0;i<N-6;i++)tubmks[i].active=0;init=1;ps("[TUB] Tuberose initialized\n");return 0;}
int tub_planning(int t,int c,int a,int b,int d,int e,int y){return add(tubps,&st.n_tbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tub_execution(int t,int c,int a,int b,int d,int e,int y){return add(tubss,&st.n_tbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tub_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tubvss,&st.n_tbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tub_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tubas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tub_market(int t,int c,int a,int b,int d,int e,int y){return add(tubmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tub_report(void){ps("[TUB] Tbp: ");pi(st.n_tbp);ps(" PCS=");pi(st.t_f1);ps("\nTbe: ");pi(st.n_tbe);ps(" PCS=");pi(st.t_f2);ps("\nTbv: ");pi(st.n_tbv);ps(" PCS=");pi(st.t_f3);ps("\nTbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tub_state(void){ps("[TUB] Tbp=");pi(st.n_tbp);ps(" Tbe=");pi(st.n_tbe);ps(" Tbv=");pi(st.n_tbv);ps(" Tbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tuberose Admin Demo ===\n\n");tub_init();
ps("Tuberose planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tub_planning(t,c,870+(i*17),859+(i*14),839+(i*10),821+(i*6),2020+(i%5));}
ps("\nTuberose execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tub_execution(t,c,859+(i*15),848+(i*12),830+(i*8),817+(i*5),2021+(i%4));}
ps("\nTuberose evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tub_evaluation(t,c,851+(i*13),840+(i*10),824+(i*7),813+(i*4),2022+(i%3));}
ps("\nTuberose accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tub_accessory(t,c,843+(i*11),834+(i*9),820+(i*6),810+(i*3),2023+(i%2));}
ps("\nTuberose marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tub_market(t,c,837+(i*9),828+(i*7),815+(i*5),807+(i*3),2024);}
ps("\n");tub_report();tub_state();ps("\n=== Demo Complete ===\n");return 0;}
