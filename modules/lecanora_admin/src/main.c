/* lecanora_admin: Lecanora management technology administration (v1.0)
 * Lecanora planning, lecanora execution, lecanora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lec_t;
typedef struct{int n_lecp,n_lece,n_lec2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lec_state_t;
static lec_t lecps[N],lecss[N-2],lecvss[N-4],lecas[N-6],lecmks[N-6]; static lec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lec_init(void){if(init)return -1;st.n_lecp=0;st.n_lece=0;st.n_lec2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lecps[i].active=0;for(int i=0;i<N-2;i++)lecss[i].active=0;for(int i=0;i<N-4;i++)lecvss[i].active=0;for(int i=0;i<N-6;i++)lecas[i].active=0;for(int i=0;i<N-6;i++)lecmks[i].active=0;init=1;ps("[LEC] Lecanora initialized\n");return 0;}
int lec_planning(int t,int c,int a,int b,int d,int e,int y){return add(lecps,&st.n_lecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lec_execution(int t,int c,int a,int b,int d,int e,int y){return add(lecss,&st.n_lece,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lec_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lecvss,&st.n_lec2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lec_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lecas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lec_market(int t,int c,int a,int b,int d,int e,int y){return add(lecmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lec_report(void){ps("[LEC] Lecp: ");pi(st.n_lecp);ps(" PCS=");pi(st.t_f1);ps("\nLece: ");pi(st.n_lece);ps(" PCS=");pi(st.t_f2);ps("\nLecv: ");pi(st.n_lec2);ps(" PCS=");pi(st.t_f3);ps("\nLecc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lec_state(void){ps("[LEC] Lecp=");pi(st.n_lecp);ps(" Lece=");pi(st.n_lece);ps(" Lecv=");pi(st.n_lec2);ps(" Lecc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lecanora Admin Demo ===\n\n");lec_init();
ps("Lecanora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lec_planning(t,c,1048+(i*17),1037+(i*14),1017+(i*10),999+(i*6),2020+(i%5));}
ps("\nLecanora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lec_execution(t,c,1037+(i*15),1026+(i*12),1008+(i*8),995+(i*5),2021+(i%4));}
ps("\nLecanora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lec_evaluation(t,c,1029+(i*13),1018+(i*10),1002+(i*7),991+(i*4),2022+(i%3));}
ps("\nLecanora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lec_accessory(t,c,1021+(i*11),1012+(i*9),998+(i*6),988+(i*3),2023+(i%2));}
ps("\nLecanora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lec_market(t,c,1015+(i*9),1006+(i*7),993+(i*5),985+(i*3),2024);}
ps("\n");lec_report();lec_state();ps("\n=== Demo Complete ===\n");return 0;}
