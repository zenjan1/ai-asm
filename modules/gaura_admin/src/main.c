/* gaura_admin: Gaura management technology administration (v1.0)
 * Gaura planning, gaura execution, gaura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gu_t;
typedef struct{int n_gup,n_gue,n_guv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gu_state_t;
static gu_t gups[N],guss[N-2],guvss[N-4],guas[N-6],gumks[N-6]; static gu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gur_init(void){if(init)return -1;st.n_gup=0;st.n_gue=0;st.n_guv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gups[i].active=0;for(int i=0;i<N-2;i++)guss[i].active=0;for(int i=0;i<N-4;i++)guvss[i].active=0;for(int i=0;i<N-6;i++)guas[i].active=0;for(int i=0;i<N-6;i++)gumks[i].active=0;init=1;ps("[GUR] Gaura initialized\n");return 0;}
int gur_planning(int t,int c,int a,int b,int d,int e,int y){return add(gups,&st.n_gup,&st.t_f1,N,t,c,a,b,d,e,y);}
int gur_execution(int t,int c,int a,int b,int d,int e,int y){return add(guss,&st.n_gue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(guvss,&st.n_guv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(guas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gur_market(int t,int c,int a,int b,int d,int e,int y){return add(gumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gur_report(void){ps("[GUR] Gup: ");pi(st.n_gup);ps(" PCS=");pi(st.t_f1);ps("\nGue: ");pi(st.n_gue);ps(" PCS=");pi(st.t_f2);ps("\nGuv: ");pi(st.n_guv);ps(" PCS=");pi(st.t_f3);ps("\nGuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gur_state(void){ps("[GUR] Gup=");pi(st.n_gup);ps(" Gue=");pi(st.n_gue);ps(" Guv=");pi(st.n_guv);ps(" Guc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gaura Admin Demo ===\n\n");gur_init();
ps("Gaura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gur_planning(t,c,728+(i*17),717+(i*14),697+(i*10),679+(i*6),2020+(i%5));}
ps("\nGaura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gur_execution(t,c,717+(i*15),706+(i*12),688+(i*8),675+(i*5),2021+(i%4));}
ps("\nGaura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gur_evaluation(t,c,709+(i*13),698+(i*10),682+(i*7),671+(i*4),2022+(i%3));}
ps("\nGaura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gur_accessory(t,c,701+(i*11),692+(i*9),678+(i*6),668+(i*3),2023+(i%2));}
ps("\nGaura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gur_market(t,c,695+(i*9),686+(i*7),673+(i*5),665+(i*3),2024);}
ps("\n");gur_report();gur_state();ps("\n=== Demo Complete ===\n");return 0;}
