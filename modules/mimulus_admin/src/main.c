/* mimulus_admin: Mimulus management technology administration (v1.0)
 * Mimulus planning, mimulus execution, mimulus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mml_t;
typedef struct{int n_mup,n_mue,n_muv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mml_state_t;
static mml_t mmlps[N],mmlss[N-2],mmlvss[N-4],mmlas[N-6],mmlmks[N-6]; static mml_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mml_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mml_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MML] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mml_init(void){if(init)return -1;st.n_mup=0;st.n_mue=0;st.n_muv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mmlps[i].active=0;for(int i=0;i<N-2;i++)mmlss[i].active=0;for(int i=0;i<N-4;i++)mmlvss[i].active=0;for(int i=0;i<N-6;i++)mmlas[i].active=0;for(int i=0;i<N-6;i++)mmlmks[i].active=0;init=1;ps("[MML] Mimulus initialized\n");return 0;}
int mml_planning(int t,int c,int a,int b,int d,int e,int y){return add(mmlps,&st.n_mup,&st.t_f1,N,t,c,a,b,d,e,y);}
int mml_execution(int t,int c,int a,int b,int d,int e,int y){return add(mmlss,&st.n_mue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mml_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mmlvss,&st.n_muv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mml_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mmlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mml_market(int t,int c,int a,int b,int d,int e,int y){return add(mmlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mml_report(void){ps("[MML] Mup: ");pi(st.n_mup);ps(" PCS=");pi(st.t_f1);ps("\nMue: ");pi(st.n_mue);ps(" PCS=");pi(st.t_f2);ps("\nMuv: ");pi(st.n_muv);ps(" PCS=");pi(st.t_f3);ps("\nMuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mml_state(void){ps("[MML] Mup=");pi(st.n_mup);ps(" Mue=");pi(st.n_mue);ps(" Muv=");pi(st.n_muv);ps(" Muc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mimulus Admin Demo ===\n\n");mml_init();
ps("Mimulus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mml_planning(t,c,840+(i*17),829+(i*14),809+(i*10),791+(i*6),2020+(i%5));}
ps("\nMimulus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mml_execution(t,c,829+(i*15),818+(i*12),800+(i*8),787+(i*5),2021+(i%4));}
ps("\nMimulus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mml_evaluation(t,c,821+(i*13),810+(i*10),794+(i*7),783+(i*4),2022+(i%3));}
ps("\nMimulus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mml_accessory(t,c,813+(i*11),804+(i*9),790+(i*6),780+(i*3),2023+(i%2));}
ps("\nMimulus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mml_market(t,c,807+(i*9),798+(i*7),785+(i*5),777+(i*3),2024);}
ps("\n");mml_report();mml_state();ps("\n=== Demo Complete ===\n");return 0;}
