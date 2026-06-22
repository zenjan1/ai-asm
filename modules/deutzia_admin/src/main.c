/* deutzia_admin: Deutzia management technology administration (v1.0)
 * Deutzia planning, deutzia execution, deutzia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} deu_t;
typedef struct{int n_deup,n_deue,n_deu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} deu_state_t;
static deu_t deups[N],deuss[N-2],deuvss[N-4],deuas[N-6],deumks[N-6]; static deu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(deu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;deu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DEU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int deu_init(void){if(init)return -1;st.n_deup=0;st.n_deue=0;st.n_deu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)deups[i].active=0;for(int i=0;i<N-2;i++)deuss[i].active=0;for(int i=0;i<N-4;i++)deuvss[i].active=0;for(int i=0;i<N-6;i++)deuas[i].active=0;for(int i=0;i<N-6;i++)deumks[i].active=0;init=1;ps("[DEU] Deutzia initialized\n");return 0;}
int deu_planning(int t,int c,int a,int b,int d,int e,int y){return add(deups,&st.n_deup,&st.t_f1,N,t,c,a,b,d,e,y);}
int deu_execution(int t,int c,int a,int b,int d,int e,int y){return add(deuss,&st.n_deue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int deu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(deuvss,&st.n_deu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int deu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(deuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int deu_market(int t,int c,int a,int b,int d,int e,int y){return add(deumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void deu_report(void){ps("[DEU] Deup: ");pi(st.n_deup);ps(" PCS=");pi(st.t_f1);ps("\nDeue: ");pi(st.n_deue);ps(" PCS=");pi(st.t_f2);ps("\nDeuv: ");pi(st.n_deu2);ps(" PCS=");pi(st.t_f3);ps("\nDeuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void deu_state(void){ps("[DEU] Deup=");pi(st.n_deup);ps(" Deue=");pi(st.n_deue);ps(" Deuv=");pi(st.n_deu2);ps(" Deuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Deutzia Admin Demo ===\n\n");deu_init();
ps("Deutzia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;deu_planning(t,c,1261+(i*17),1250+(i*14),1230+(i*10),1212+(i*6),2020+(i%5));}
ps("\nDeutzia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;deu_execution(t,c,1250+(i*15),1239+(i*12),1221+(i*8),1208+(i*5),2021+(i%4));}
ps("\nDeutzia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;deu_evaluation(t,c,1242+(i*13),1231+(i*10),1215+(i*7),1204+(i*4),2022+(i%3));}
ps("\nDeutzia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;deu_accessory(t,c,1234+(i*11),1225+(i*9),1211+(i*6),1201+(i*3),2023+(i%2));}
ps("\nDeutzia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;deu_market(t,c,1228+(i*9),1219+(i*7),1206+(i*5),1198+(i*3),2024);}
ps("\n");deu_report();deu_state();ps("\n=== Demo Complete ===\n");return 0;}
