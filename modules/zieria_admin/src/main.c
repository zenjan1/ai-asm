/* zieria_admin: Zieria management technology administration (v1.0)
 * Zieria planning, zieria execution, zieria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zie_t;
typedef struct{int n_ziep,n_ziee,n_zie2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zie_state_t;
static zie_t zieps[N],ziess[N-2],zievss[N-4],zieas[N-6],ziemks[N-6]; static zie_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zie_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zie_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZIE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zie_init(void){if(init)return -1;st.n_ziep=0;st.n_ziee=0;st.n_zie2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zieps[i].active=0;for(int i=0;i<N-2;i++)ziess[i].active=0;for(int i=0;i<N-4;i++)zievss[i].active=0;for(int i=0;i<N-6;i++)zieas[i].active=0;for(int i=0;i<N-6;i++)ziemks[i].active=0;init=1;ps("[ZIE] Zieria initialized\n");return 0;}
int zie_planning(int t,int c,int a,int b,int d,int e,int y){return add(zieps,&st.n_ziep,&st.t_f1,N,t,c,a,b,d,e,y);}
int zie_execution(int t,int c,int a,int b,int d,int e,int y){return add(ziess,&st.n_ziee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zie_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zievss,&st.n_zie2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zie_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zieas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zie_market(int t,int c,int a,int b,int d,int e,int y){return add(ziemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zie_report(void){ps("[ZIE] Ziepp: ");pi(st.n_ziep);ps(" PCS=");pi(st.t_f1);ps("\nZiee: ");pi(st.n_ziee);ps(" PCS=");pi(st.t_f2);ps("\nZiev: ");pi(st.n_zie2);ps(" PCS=");pi(st.t_f3);ps("\nZiec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zie_state(void){ps("[ZIE] Ziepp=");pi(st.n_ziep);ps(" Ziee=");pi(st.n_ziee);ps(" Ziev=");pi(st.n_zie2);ps(" Ziec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zieria Admin Demo ===\n\n");zie_init();
ps("Zieria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zie_planning(t,c,1115+(i*17),1104+(i*14),1084+(i*10),1066+(i*6),2020+(i%5));}
ps("\nZieria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zie_execution(t,c,1104+(i*15),1093+(i*12),1075+(i*8),1062+(i*5),2021+(i%4));}
ps("\nZieria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zie_evaluation(t,c,1096+(i*13),1085+(i*10),1069+(i*7),1058+(i*4),2022+(i%3));}
ps("\nZieria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zie_accessory(t,c,1088+(i*11),1079+(i*9),1065+(i*6),1055+(i*3),2023+(i%2));}
ps("\nZieria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zie_market(t,c,1082+(i*9),1073+(i*7),1060+(i*5),1052+(i*3),2024);}
ps("\n");zie_report();zie_state();ps("\n=== Demo Complete ===\n");return 0;}
