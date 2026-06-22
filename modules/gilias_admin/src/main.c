/* gilias_admin: Gilias management technology administration (v1.0)
 * Gilias planning, gilias execution, gilias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gli_t;
typedef struct{int n_glip,n_glie,n_gli2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gli_state_t;
static gli_t glips[N],gliss[N-2],glivss[N-4],glias[N-6],glimks[N-6]; static gli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gli_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gli_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gli_init(void){if(init)return -1;st.n_glip=0;st.n_glie=0;st.n_gli2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)glips[i].active=0;for(int i=0;i<N-2;i++)gliss[i].active=0;for(int i=0;i<N-4;i++)glivss[i].active=0;for(int i=0;i<N-6;i++)glias[i].active=0;for(int i=0;i<N-6;i++)glimks[i].active=0;init=1;ps("[GLI] Gilias initialized\n");return 0;}
int gli_planning(int t,int c,int a,int b,int d,int e,int y){return add(glips,&st.n_glip,&st.t_f1,N,t,c,a,b,d,e,y);}
int gli_execution(int t,int c,int a,int b,int d,int e,int y){return add(gliss,&st.n_glie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(glivss,&st.n_gli2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(glias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gli_market(int t,int c,int a,int b,int d,int e,int y){return add(glimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gli_report(void){ps("[GLI] Gilpp: ");pi(st.n_glip);ps(" PCS=");pi(st.t_f1);ps("\nGile: ");pi(st.n_glie);ps(" PCS=");pi(st.t_f2);ps("\nGilv: ");pi(st.n_gli2);ps(" PCS=");pi(st.t_f3);ps("\nGilc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gli_state(void){ps("[GLI] Gilpp=");pi(st.n_glip);ps(" Gile=");pi(st.n_glie);ps(" Gilv=");pi(st.n_gli2);ps(" Gilc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gilias Admin Demo ===\n\n");gli_init();
ps("Gilias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gli_planning(t,c,1145+(i*17),1134+(i*14),1114+(i*10),1096+(i*6),2020+(i%5));}
ps("\nGilias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gli_execution(t,c,1134+(i*15),1123+(i*12),1105+(i*8),1092+(i*5),2021+(i%4));}
ps("\nGilias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gli_evaluation(t,c,1126+(i*13),1115+(i*10),1099+(i*7),1088+(i*4),2022+(i%3));}
ps("\nGilias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gli_accessory(t,c,1118+(i*11),1109+(i*9),1095+(i*6),1085+(i*3),2023+(i%2));}
ps("\nGilias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gli_market(t,c,1112+(i*9),1103+(i*7),1090+(i*5),1082+(i*3),2024);}
ps("\n");gli_report();gli_state();ps("\n=== Demo Complete ===\n");return 0;}
