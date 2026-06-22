/* jamesia_admin: Jamesia management technology administration (v1.0)
 * Jamesia planning, jamesia execution, jamesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jam_t;
typedef struct{int n_jamp,n_jame,n_jam2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jam_state_t;
static jam_t jamps[N],jamss[N-2],jamvss[N-4],jamas[N-6],jammks[N-6]; static jam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jam_init(void){if(init)return -1;st.n_jamp=0;st.n_jame=0;st.n_jam2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jamps[i].active=0;for(int i=0;i<N-2;i++)jamss[i].active=0;for(int i=0;i<N-4;i++)jamvss[i].active=0;for(int i=0;i<N-6;i++)jamas[i].active=0;for(int i=0;i<N-6;i++)jammks[i].active=0;init=1;ps("[JAM] Jamesia initialized\n");return 0;}
int jam_planning(int t,int c,int a,int b,int d,int e,int y){return add(jamps,&st.n_jamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jam_execution(int t,int c,int a,int b,int d,int e,int y){return add(jamss,&st.n_jame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jam_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jamvss,&st.n_jam2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jam_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jamas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jam_market(int t,int c,int a,int b,int d,int e,int y){return add(jammks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jam_report(void){ps("[JAM] Jampp: ");pi(st.n_jamp);ps(" PCS=");pi(st.t_f1);ps("\nJame: ");pi(st.n_jame);ps(" PCS=");pi(st.t_f2);ps("\nJamv: ");pi(st.n_jam2);ps(" PCS=");pi(st.t_f3);ps("\nJamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jam_state(void){ps("[JAM] Jampp=");pi(st.n_jamp);ps(" Jame=");pi(st.n_jame);ps(" Jamv=");pi(st.n_jam2);ps(" Jamc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jamesia Admin Demo ===\n\n");jam_init();
ps("Jamesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jam_planning(t,c,1152+(i*17),1141+(i*14),1121+(i*10),1103+(i*6),2020+(i%5));}
ps("\nJamesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jam_execution(t,c,1141+(i*15),1130+(i*12),1112+(i*8),1099+(i*5),2021+(i%4));}
ps("\nJamesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jam_evaluation(t,c,1133+(i*13),1122+(i*10),1106+(i*7),1095+(i*4),2022+(i%3));}
ps("\nJamesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jam_accessory(t,c,1125+(i*11),1116+(i*9),1102+(i*6),1092+(i*3),2023+(i%2));}
ps("\nJamesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jam_market(t,c,1119+(i*9),1110+(i*7),1097+(i*5),1089+(i*3),2024);}
ps("\n");jam_report();jam_state();ps("\n=== Demo Complete ===\n");return 0;}
