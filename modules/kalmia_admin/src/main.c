/* kalmia_admin: Kalmia management technology administration (v1.0)
 * Kalmia planning, kalmia execution, kalmia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kal_t;
typedef struct{int n_kalp,n_kale,n_kal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kal_state_t;
static kal_t kalps[N],kalss[N-2],kalvss[N-4],kalas[N-6],kalmks[N-6]; static kal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kal_init(void){if(init)return -1;st.n_kalp=0;st.n_kale=0;st.n_kal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kalps[i].active=0;for(int i=0;i<N-2;i++)kalss[i].active=0;for(int i=0;i<N-4;i++)kalvss[i].active=0;for(int i=0;i<N-6;i++)kalas[i].active=0;for(int i=0;i<N-6;i++)kalmks[i].active=0;init=1;ps("[KAL] Kalmia initialized\n");return 0;}
int kal_planning(int t,int c,int a,int b,int d,int e,int y){return add(kalps,&st.n_kalp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kal_execution(int t,int c,int a,int b,int d,int e,int y){return add(kalss,&st.n_kale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kalvss,&st.n_kal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kalas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kal_market(int t,int c,int a,int b,int d,int e,int y){return add(kalmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kal_report(void){ps("[KAL] Kalpp: ");pi(st.n_kalp);ps(" PCS=");pi(st.t_f1);ps("\nKale: ");pi(st.n_kale);ps(" PCS=");pi(st.t_f2);ps("\nKalv: ");pi(st.n_kal2);ps(" PCS=");pi(st.t_f3);ps("\nKalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kal_state(void){ps("[KAL] Kalpp=");pi(st.n_kalp);ps(" Kale=");pi(st.n_kale);ps(" Kalv=");pi(st.n_kal2);ps(" Kalc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kalmia Admin Demo ===\n\n");kal_init();
ps("Kalmia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kal_planning(t,c,1153+(i*17),1142+(i*14),1122+(i*10),1104+(i*6),2020+(i%5));}
ps("\nKalmia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kal_execution(t,c,1142+(i*15),1131+(i*12),1113+(i*8),1100+(i*5),2021+(i%4));}
ps("\nKalmia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kal_evaluation(t,c,1134+(i*13),1123+(i*10),1107+(i*7),1096+(i*4),2022+(i%3));}
ps("\nKalmia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kal_accessory(t,c,1126+(i*11),1117+(i*9),1103+(i*6),1093+(i*3),2023+(i%2));}
ps("\nKalmia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kal_market(t,c,1120+(i*9),1111+(i*7),1098+(i*5),1090+(i*3),2024);}
ps("\n");kal_report();kal_state();ps("\n=== Demo Complete ===\n");return 0;}
