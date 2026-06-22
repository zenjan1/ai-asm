/* notonia_admin: Notonia management technology administration (v1.0)
 * Notonia planning, notonia execution, notonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} not_t;
typedef struct{int n_notp,n_note,n_not2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} not_state_t;
static not_t notps[N],notess[N-2],notvss[N-4],notas[N-6],notmks[N-6]; static not_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(not_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;not_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int not_init(void){if(init)return -1;st.n_notp=0;st.n_note=0;st.n_not2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)notps[i].active=0;for(int i=0;i<N-2;i++)notess[i].active=0;for(int i=0;i<N-4;i++)notvss[i].active=0;for(int i=0;i<N-6;i++)notas[i].active=0;for(int i=0;i<N-6;i++)notmks[i].active=0;init=1;ps("[NOT] Notonia initialized\n");return 0;}
int not_planning(int t,int c,int a,int b,int d,int e,int y){return add(notps,&st.n_notp,&st.t_f1,N,t,c,a,b,d,e,y);}
int not_execution(int t,int c,int a,int b,int d,int e,int y){return add(notess,&st.n_note,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int not_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(notvss,&st.n_not2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int not_accessory(int t,int c,int a,int b,int d,int e,int y){return add(notas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int not_market(int t,int c,int a,int b,int d,int e,int y){return add(notmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void not_report(void){ps("[NOT] Notp: ");pi(st.n_notp);ps(" PCS=");pi(st.t_f1);ps("\nNote: ");pi(st.n_note);ps(" PCS=");pi(st.t_f2);ps("\nNotv: ");pi(st.n_not2);ps(" PCS=");pi(st.t_f3);ps("\nNotc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void not_state(void){ps("[NOT] Notp=");pi(st.n_notp);ps(" Note=");pi(st.n_note);ps(" Notv=");pi(st.n_not2);ps(" Notc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Notonia Admin Demo ===\n\n");not_init();
ps("Notonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;not_planning(t,c,1318+(i*17),1307+(i*14),1287+(i*10),1269+(i*6),2020+(i%5));}
ps("\nNotonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;not_execution(t,c,1307+(i*15),1296+(i*12),1278+(i*8),1265+(i*5),2021+(i%4));}
ps("\nNotonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;not_evaluation(t,c,1299+(i*13),1288+(i*10),1272+(i*7),1261+(i*4),2022+(i%3));}
ps("\nNotonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;not_accessory(t,c,1291+(i*11),1282+(i*9),1268+(i*6),1258+(i*3),2023+(i%2));}
ps("\nNotonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;not_market(t,c,1285+(i*9),1276+(i*7),1263+(i*5),1255+(i*3),2024);}
ps("\n");not_report();not_state();ps("\n=== Demo Complete ===\n");return 0;}
