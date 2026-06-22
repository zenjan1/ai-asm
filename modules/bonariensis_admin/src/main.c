/* bonariensis_admin: Bonariensis management technology administration (v1.0)
 * Bonariensis planning, bonariensis execution, bonariensis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bon_t;
typedef struct{int n_bonp,n_bone,n_bon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bon_state_t;
static bon_t bonps[N],bonss[N-2],bonvss[N-4],bonas[N-6],bonmks[N-6]; static bon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bon_init(void){if(init)return -1;st.n_bonp=0;st.n_bone=0;st.n_bon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bonps[i].active=0;for(int i=0;i<N-2;i++)bonss[i].active=0;for(int i=0;i<N-4;i++)bonvss[i].active=0;for(int i=0;i<N-6;i++)bonas[i].active=0;for(int i=0;i<N-6;i++)bonmks[i].active=0;init=1;ps("[BON] Bonariensis initialized\n");return 0;}
int bon_planning(int t,int c,int a,int b,int d,int e,int y){return add(bonps,&st.n_bonp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bon_execution(int t,int c,int a,int b,int d,int e,int y){return add(bonss,&st.n_bone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bonvss,&st.n_bon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bonas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bon_market(int t,int c,int a,int b,int d,int e,int y){return add(bonmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bon_report(void){ps("[BON] Bonpp: ");pi(st.n_bonp);ps(" PCS=");pi(st.t_f1);ps("\nBonpe: ");pi(st.n_bone);ps(" PCS=");pi(st.t_f2);ps("\nBonv: ");pi(st.n_bon2);ps(" PCS=");pi(st.t_f3);ps("\nBonc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bon_state(void){ps("[BON] Bonpp=");pi(st.n_bonp);ps(" Bonpe=");pi(st.n_bone);ps(" Bonv=");pi(st.n_bon2);ps(" Bonc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bonariensis Admin Demo ===\n\n");bon_init();
ps("Bonariensis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bon_planning(t,c,1222+(i*17),1211+(i*14),1191+(i*10),1173+(i*6),2020+(i%5));}
ps("\nBonariensis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bon_execution(t,c,1211+(i*15),1200+(i*12),1182+(i*8),1169+(i*5),2021+(i%4));}
ps("\nBonariensis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bon_evaluation(t,c,1203+(i*13),1192+(i*10),1176+(i*7),1165+(i*4),2022+(i%3));}
ps("\nBonariensis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bon_accessory(t,c,1195+(i*11),1186+(i*9),1172+(i*6),1162+(i*3),2023+(i%2));}
ps("\nBonariensis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bon_market(t,c,1189+(i*9),1180+(i*7),1167+(i*5),1159+(i*3),2024);}
ps("\n");bon_report();bon_state();ps("\n=== Demo Complete ===\n");return 0;}
