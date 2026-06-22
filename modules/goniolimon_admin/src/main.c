/* goniolimon_admin: Goniolimon management technology administration (v1.0)
 * Goniolimon planning, goniolimon execution, goniolimon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gon_t;
typedef struct{int n_gonp,n_gone,n_gon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gon_state_t;
static gon_t gonps[N],gonss[N-2],gonvss[N-4],gonas[N-6],gonmks[N-6]; static gon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gon_init(void){if(init)return -1;st.n_gonp=0;st.n_gone=0;st.n_gon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gonps[i].active=0;for(int i=0;i<N-2;i++)gonss[i].active=0;for(int i=0;i<N-4;i++)gonvss[i].active=0;for(int i=0;i<N-6;i++)gonas[i].active=0;for(int i=0;i<N-6;i++)gonmks[i].active=0;init=1;ps("[GON] Goniolimon initialized\n");return 0;}
int gon_planning(int t,int c,int a,int b,int d,int e,int y){return add(gonps,&st.n_gonp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gon_execution(int t,int c,int a,int b,int d,int e,int y){return add(gonss,&st.n_gone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gonvss,&st.n_gon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gonas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gon_market(int t,int c,int a,int b,int d,int e,int y){return add(gonmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gon_report(void){ps("[GON] Gonp: ");pi(st.n_gonp);ps(" PCS=");pi(st.t_f1);ps("\nGone: ");pi(st.n_gone);ps(" PCS=");pi(st.t_f2);ps("\nGonv: ");pi(st.n_gon2);ps(" PCS=");pi(st.t_f3);ps("\nGonc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gon_state(void){ps("[GON] Gonp=");pi(st.n_gonp);ps(" Gone=");pi(st.n_gone);ps(" Gonv=");pi(st.n_gon2);ps(" Gonc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Goniolimon Admin Demo ===\n\n");gon_init();
ps("Goniolimon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gon_planning(t,c,1288+(i*17),1277+(i*14),1257+(i*10),1239+(i*6),2020+(i%5));}
ps("\nGoniolimon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gon_execution(t,c,1277+(i*15),1266+(i*12),1248+(i*8),1235+(i*5),2021+(i%4));}
ps("\nGoniolimon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gon_evaluation(t,c,1269+(i*13),1258+(i*10),1242+(i*7),1231+(i*4),2022+(i%3));}
ps("\nGoniolimon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gon_accessory(t,c,1261+(i*11),1252+(i*9),1238+(i*6),1228+(i*3),2023+(i%2));}
ps("\nGoniolimon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gon_market(t,c,1255+(i*9),1246+(i*7),1233+(i*5),1225+(i*3),2024);}
ps("\n");gon_report();gon_state();ps("\n=== Demo Complete ===\n");return 0;}
