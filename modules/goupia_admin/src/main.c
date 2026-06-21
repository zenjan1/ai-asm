/* goupia_admin: Goupia management technology administration (v1.0)
 * Goupia planning, goupia execution, goupia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gou_t;
typedef struct{int n_goup,n_goue,n_gou2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gou_state_t;
static gou_t goups[N],gouss[N-2],gouvss[N-4],gouas[N-6],goumks[N-6]; static gou_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gou_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gou_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GOU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gou_init(void){if(init)return -1;st.n_goup=0;st.n_goue=0;st.n_gou2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)goups[i].active=0;for(int i=0;i<N-2;i++)gouss[i].active=0;for(int i=0;i<N-4;i++)gouvss[i].active=0;for(int i=0;i<N-6;i++)gouas[i].active=0;for(int i=0;i<N-6;i++)goumks[i].active=0;init=1;ps("[GOU] Goupia initialized\n");return 0;}
int gou_planning(int t,int c,int a,int b,int d,int e,int y){return add(goups,&st.n_goup,&st.t_f1,N,t,c,a,b,d,e,y);}
int gou_execution(int t,int c,int a,int b,int d,int e,int y){return add(gouss,&st.n_goue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gou_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gouvss,&st.n_gou2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gou_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gouas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gou_market(int t,int c,int a,int b,int d,int e,int y){return add(goumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gou_report(void){ps("[GOU] Gouup: ");pi(st.n_goup);ps(" PCS=");pi(st.t_f1);ps("\nGoue: ");pi(st.n_goue);ps(" PCS=");pi(st.t_f2);ps("\nGouv: ");pi(st.n_gou2);ps(" PCS=");pi(st.t_f3);ps("\nGouc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gou_state(void){ps("[GOU] Gouup=");pi(st.n_goup);ps(" Goue=");pi(st.n_goue);ps(" Gouv=");pi(st.n_gou2);ps(" Gouc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Goupia Admin Demo ===\n\n");gou_init();
ps("Goupia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gou_planning(t,c,1013+(i*17),1002+(i*14),982+(i*10),964+(i*6),2020+(i%5));}
ps("\nGoupia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gou_execution(t,c,1002+(i*15),991+(i*12),973+(i*8),960+(i*5),2021+(i%4));}
ps("\nGoupia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gou_evaluation(t,c,994+(i*13),983+(i*10),967+(i*7),956+(i*4),2022+(i%3));}
ps("\nGoupia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gou_accessory(t,c,986+(i*11),977+(i*9),963+(i*6),953+(i*3),2023+(i%2));}
ps("\nGoupia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gou_market(t,c,980+(i*9),971+(i*7),958+(i*5),950+(i*3),2024);}
ps("\n");gou_report();gou_state();ps("\n=== Demo Complete ===\n");return 0;}
