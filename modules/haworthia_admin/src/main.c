/* haworthia_admin: Haworthia management technology administration (v1.0)
 * Haworthia planning, haworthia execution, haworthia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} haw_t;
typedef struct{int n_hawp,n_hawe,n_haw2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} haw_state_t;
static haw_t hawps[N],hawss[N-2],hawvss[N-4],hawas[N-6],hawmks[N-6]; static haw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(haw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;haw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HAW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int haw_init(void){if(init)return -1;st.n_hawp=0;st.n_hawe=0;st.n_haw2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hawps[i].active=0;for(int i=0;i<N-2;i++)hawss[i].active=0;for(int i=0;i<N-4;i++)hawvss[i].active=0;for(int i=0;i<N-6;i++)hawas[i].active=0;for(int i=0;i<N-6;i++)hawmks[i].active=0;init=1;ps("[HAW] Haworthia initialized\n");return 0;}
int haw_planning(int t,int c,int a,int b,int d,int e,int y){return add(hawps,&st.n_hawp,&st.t_f1,N,t,c,a,b,d,e,y);}
int haw_execution(int t,int c,int a,int b,int d,int e,int y){return add(hawss,&st.n_hawe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int haw_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hawvss,&st.n_haw2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int haw_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hawas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int haw_market(int t,int c,int a,int b,int d,int e,int y){return add(hawmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void haw_report(void){ps("[HAW] Hawp: ");pi(st.n_hawp);ps(" PCS=");pi(st.t_f1);ps("\nHawe: ");pi(st.n_hawe);ps(" PCS=");pi(st.t_f2);ps("\nHawv: ");pi(st.n_haw2);ps(" PCS=");pi(st.t_f3);ps("\nHawc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void haw_state(void){ps("[HAW] Hawp=");pi(st.n_hawp);ps(" Hawe=");pi(st.n_hawe);ps(" Hawv=");pi(st.n_haw2);ps(" Hawc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Haworthia Admin Demo ===\n\n");haw_init();
ps("Haworthia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;haw_planning(t,c,1291+(i*17),1280+(i*14),1260+(i*10),1242+(i*6),2020+(i%5));}
ps("\nHaworthia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;haw_execution(t,c,1280+(i*15),1269+(i*12),1251+(i*8),1238+(i*5),2021+(i%4));}
ps("\nHaworthia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;haw_evaluation(t,c,1272+(i*13),1261+(i*10),1245+(i*7),1234+(i*4),2022+(i%3));}
ps("\nHaworthia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;haw_accessory(t,c,1264+(i*11),1255+(i*9),1241+(i*6),1231+(i*3),2023+(i%2));}
ps("\nHaworthia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;haw_market(t,c,1258+(i*9),1249+(i*7),1236+(i*5),1228+(i*3),2024);}
ps("\n");haw_report();haw_state();ps("\n=== Demo Complete ===\n");return 0;}
