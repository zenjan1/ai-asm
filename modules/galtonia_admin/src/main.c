/* galtonia_admin: Galtonia management technology administration (v1.0)
 * Galtonia planning, galtonia execution, galtonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ga_t;
typedef struct{int n_glp,n_gle,n_glv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ga_state_t;
static ga_t glps[N],glss[N-2],glvss[N-4],glas[N-6],glmks[N-6]; static ga_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ga_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ga_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int glt_init(void){if(init)return -1;st.n_glp=0;st.n_gle=0;st.n_glv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)glps[i].active=0;for(int i=0;i<N-2;i++)glss[i].active=0;for(int i=0;i<N-4;i++)glvss[i].active=0;for(int i=0;i<N-6;i++)glas[i].active=0;for(int i=0;i<N-6;i++)glmks[i].active=0;init=1;ps("[GLT] Galtonia initialized\n");return 0;}
int glt_planning(int t,int c,int a,int b,int d,int e,int y){return add(glps,&st.n_glp,&st.t_f1,N,t,c,a,b,d,e,y);}
int glt_execution(int t,int c,int a,int b,int d,int e,int y){return add(glss,&st.n_gle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int glt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(glvss,&st.n_glv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int glt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(glas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int glt_market(int t,int c,int a,int b,int d,int e,int y){return add(glmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void glt_report(void){ps("[GLT] Glp: ");pi(st.n_glp);ps(" PCS=");pi(st.t_f1);ps("\nGle: ");pi(st.n_gle);ps(" PCS=");pi(st.t_f2);ps("\nGlv: ");pi(st.n_glv);ps(" PCS=");pi(st.t_f3);ps("\nGlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void glt_state(void){ps("[GLT] Glp=");pi(st.n_glp);ps(" Gle=");pi(st.n_gle);ps(" Glv=");pi(st.n_glv);ps(" Glc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Galtonia Admin Demo ===\n\n");glt_init();
ps("Galtonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;glt_planning(t,c,776+(i*17),765+(i*14),745+(i*10),727+(i*6),2020+(i%5));}
ps("\nGaltonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;glt_execution(t,c,765+(i*15),754+(i*12),736+(i*8),723+(i*5),2021+(i%4));}
ps("\nGaltonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;glt_evaluation(t,c,757+(i*13),746+(i*10),732+(i*7),721+(i*4),2022+(i%3));}
ps("\nGaltonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glt_accessory(t,c,749+(i*11),740+(i*9),726+(i*6),716+(i*3),2023+(i%2));}
ps("\nGaltonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glt_market(t,c,743+(i*9),734+(i*7),721+(i*5),713+(i*3),2024);}
ps("\n");glt_report();glt_state();ps("\n=== Demo Complete ===\n");return 0;}
