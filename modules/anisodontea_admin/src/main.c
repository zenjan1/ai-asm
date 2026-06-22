/* anisodontea_admin: Anisodontea management technology administration (v1.0)
 * Anisodontea planning, anisodontea execution, anisodontea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ani_t;
typedef struct{int n_anip,n_anie,n_ani2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ani_state_t;
static ani_t anips[N],aniss[N-2],anivss[N-4],anias[N-6],animks[N-6]; static ani_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ani_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ani_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ani_init(void){if(init)return -1;st.n_anip=0;st.n_anie=0;st.n_ani2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anips[i].active=0;for(int i=0;i<N-2;i++)aniss[i].active=0;for(int i=0;i<N-4;i++)anivss[i].active=0;for(int i=0;i<N-6;i++)anias[i].active=0;for(int i=0;i<N-6;i++)animks[i].active=0;init=1;ps("[ANI] Anisodontea initialized\n");return 0;}
int ani_planning(int t,int c,int a,int b,int d,int e,int y){return add(anips,&st.n_anip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ani_execution(int t,int c,int a,int b,int d,int e,int y){return add(aniss,&st.n_anie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ani_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anivss,&st.n_ani2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ani_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ani_market(int t,int c,int a,int b,int d,int e,int y){return add(animks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ani_report(void){ps("[ANI] Anipp: ");pi(st.n_anip);ps(" PCS=");pi(st.t_f1);ps("\nAnie: ");pi(st.n_anie);ps(" PCS=");pi(st.t_f2);ps("\nAniv: ");pi(st.n_ani2);ps(" PCS=");pi(st.t_f3);ps("\nAnic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ani_state(void){ps("[ANI] Anipp=");pi(st.n_anip);ps(" Anie=");pi(st.n_anie);ps(" Aniv=");pi(st.n_ani2);ps(" Anic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anisodontea Admin Demo ===\n\n");ani_init();
ps("Anisodontea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ani_planning(t,c,1118+(i*17),1107+(i*14),1087+(i*10),1069+(i*6),2020+(i%5));}
ps("\nAnisodontea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ani_execution(t,c,1107+(i*15),1096+(i*12),1078+(i*8),1065+(i*5),2021+(i%4));}
ps("\nAnisodontea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ani_evaluation(t,c,1099+(i*13),1088+(i*10),1072+(i*7),1061+(i*4),2022+(i%3));}
ps("\nAnisodontea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ani_accessory(t,c,1091+(i*11),1082+(i*9),1068+(i*6),1058+(i*3),2023+(i%2));}
ps("\nAnisodontea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ani_market(t,c,1085+(i*9),1076+(i*7),1063+(i*5),1055+(i*3),2024);}
ps("\n");ani_report();ani_state();ps("\n=== Demo Complete ===\n");return 0;}
