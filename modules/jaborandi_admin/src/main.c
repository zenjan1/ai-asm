/* jaborandi_admin: Jaborandi management technology administration (v1.0)
 * Jaborandi planning, jaborandi execution, jaborandi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jab_t;
typedef struct{int n_jabp,n_jabe,n_jab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jab_state_t;
static jab_t jabps[N],jabss[N-2],jabvss[N-4],jabas[N-6],jabmks[N-6]; static jab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jab_init(void){if(init)return -1;st.n_jabp=0;st.n_jabe=0;st.n_jab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jabps[i].active=0;for(int i=0;i<N-2;i++)jabss[i].active=0;for(int i=0;i<N-4;i++)jabvss[i].active=0;for(int i=0;i<N-6;i++)jabas[i].active=0;for(int i=0;i<N-6;i++)jabmks[i].active=0;init=1;ps("[JAB] Jaborandi initialized\n");return 0;}
int jab_planning(int t,int c,int a,int b,int d,int e,int y){return add(jabps,&st.n_jabp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jab_execution(int t,int c,int a,int b,int d,int e,int y){return add(jabss,&st.n_jabe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jabvss,&st.n_jab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jabas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jab_market(int t,int c,int a,int b,int d,int e,int y){return add(jabmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jab_report(void){ps("[JAB] Jabp: ");pi(st.n_jabp);ps(" PCS=");pi(st.t_f1);ps("\nJabe: ");pi(st.n_jabe);ps(" PCS=");pi(st.t_f2);ps("\nJabv: ");pi(st.n_jab2);ps(" PCS=");pi(st.t_f3);ps("\nJabc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jab_state(void){ps("[JAB] Jabp=");pi(st.n_jabp);ps(" Jabe=");pi(st.n_jabe);ps(" Jabv=");pi(st.n_jab2);ps(" Jabc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jaborandi Admin Demo ===\n\n");jab_init();
ps("Jaborandi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jab_planning(t,c,1027+(i*17),1016+(i*14),996+(i*10),978+(i*6),2020+(i%5));}
ps("\nJaborandi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jab_execution(t,c,1016+(i*15),1005+(i*12),987+(i*8),974+(i*5),2021+(i%4));}
ps("\nJaborandi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jab_evaluation(t,c,1008+(i*13),997+(i*10),981+(i*7),970+(i*4),2022+(i%3));}
ps("\nJaborandi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jab_accessory(t,c,1000+(i*11),991+(i*9),977+(i*6),967+(i*3),2023+(i%2));}
ps("\nJaborandi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jab_market(t,c,994+(i*9),985+(i*7),972+(i*5),964+(i*3),2024);}
ps("\n");jab_report();jab_state();ps("\n=== Demo Complete ===\n");return 0;}
