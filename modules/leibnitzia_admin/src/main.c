/* leibnitzia_admin: Leibnitzia management technology administration (v1.0)
 * Leibnitzia planning, leibnitzia execution, leibnitzia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lei_t;
typedef struct{int n_leip,n_leie,n_lei2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lei_state_t;
static lei_t leips[N],leiss[N-2],leivss[N-4],leias[N-6],leimks[N-6]; static lei_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lei_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lei_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lei_init(void){if(init)return -1;st.n_leip=0;st.n_leie=0;st.n_lei2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)leips[i].active=0;for(int i=0;i<N-2;i++)leiss[i].active=0;for(int i=0;i<N-4;i++)leivss[i].active=0;for(int i=0;i<N-6;i++)leias[i].active=0;for(int i=0;i<N-6;i++)leimks[i].active=0;init=1;ps("[LEI] Leibnitzia initialized\n");return 0;}
int lei_planning(int t,int c,int a,int b,int d,int e,int y){return add(leips,&st.n_leip,&st.t_f1,N,t,c,a,b,d,e,y);}
int lei_execution(int t,int c,int a,int b,int d,int e,int y){return add(leiss,&st.n_leie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lei_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(leivss,&st.n_lei2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lei_accessory(int t,int c,int a,int b,int d,int e,int y){return add(leias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lei_market(int t,int c,int a,int b,int d,int e,int y){return add(leimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lei_report(void){ps("[LEI] Leip: ");pi(st.n_leip);ps(" PCS=");pi(st.t_f1);ps("\nLeie: ");pi(st.n_leie);ps(" PCS=");pi(st.t_f2);ps("\nLeiv: ");pi(st.n_lei2);ps(" PCS=");pi(st.t_f3);ps("\nLeic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lei_state(void){ps("[LEI] Leip=");pi(st.n_leip);ps(" Leie=");pi(st.n_leie);ps(" Leiv=");pi(st.n_lei2);ps(" Leic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leibnitzia Admin Demo ===\n\n");lei_init();
ps("Leibnitzia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lei_planning(t,c,1050+(i*17),1039+(i*14),1019+(i*10),1001+(i*6),2020+(i%5));}
ps("\nLeibnitzia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lei_execution(t,c,1039+(i*15),1028+(i*12),1010+(i*8),997+(i*5),2021+(i%4));}
ps("\nLeibnitzia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lei_evaluation(t,c,1031+(i*13),1020+(i*10),1004+(i*7),993+(i*4),2022+(i%3));}
ps("\nLeibnitzia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lei_accessory(t,c,1023+(i*11),1014+(i*9),1000+(i*6),990+(i*3),2023+(i%2));}
ps("\nLeibnitzia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lei_market(t,c,1017+(i*9),1008+(i*7),995+(i*5),987+(i*3),2024);}
ps("\n");lei_report();lei_state();ps("\n=== Demo Complete ===\n");return 0;}
