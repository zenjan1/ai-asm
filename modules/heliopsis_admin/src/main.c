/* heliopsis_admin: Heliopsis management technology administration (v1.0)
 * Heliopsis planning, heliopsis execution, heliopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hel_t;
typedef struct{int n_help,n_hele,n_hel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hel_state_t;
static hel_t helps[N],helss[N-2],helvss[N-4],helas[N-6],helmks[N-6]; static hel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hel_init(void){if(init)return -1;st.n_help=0;st.n_hele=0;st.n_hel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)helps[i].active=0;for(int i=0;i<N-2;i++)helss[i].active=0;for(int i=0;i<N-4;i++)helvss[i].active=0;for(int i=0;i<N-6;i++)helas[i].active=0;for(int i=0;i<N-6;i++)helmks[i].active=0;init=1;ps("[HEL] Heliopsis initialized\n");return 0;}
int hel_planning(int t,int c,int a,int b,int d,int e,int y){return add(helps,&st.n_help,&st.t_f1,N,t,c,a,b,d,e,y);}
int hel_execution(int t,int c,int a,int b,int d,int e,int y){return add(helss,&st.n_hele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(helvss,&st.n_hel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(helas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hel_market(int t,int c,int a,int b,int d,int e,int y){return add(helmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hel_report(void){ps("[HEL] Helpp: ");pi(st.n_help);ps(" PCS=");pi(st.t_f1);ps("\nHele: ");pi(st.n_hele);ps(" PCS=");pi(st.t_f2);ps("\nHelv: ");pi(st.n_hel2);ps(" PCS=");pi(st.t_f3);ps("\nHelc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hel_state(void){ps("[HEL] Helpp=");pi(st.n_help);ps(" Hele=");pi(st.n_hele);ps(" Helv=");pi(st.n_hel2);ps(" Helc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heliopsis Admin Demo ===\n\n");hel_init();
ps("Heliopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hel_planning(t,c,1148+(i*17),1137+(i*14),1117+(i*10),1099+(i*6),2020+(i%5));}
ps("\nHeliopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hel_execution(t,c,1137+(i*15),1126+(i*12),1108+(i*8),1095+(i*5),2021+(i%4));}
ps("\nHeliopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hel_evaluation(t,c,1129+(i*13),1118+(i*10),1102+(i*7),1091+(i*4),2022+(i%3));}
ps("\nHeliopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hel_accessory(t,c,1121+(i*11),1112+(i*9),1098+(i*6),1088+(i*3),2023+(i%2));}
ps("\nHeliopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hel_market(t,c,1115+(i*9),1106+(i*7),1093+(i*5),1085+(i*3),2024);}
ps("\n");hel_report();hel_state();ps("\n=== Demo Complete ===\n");return 0;}
