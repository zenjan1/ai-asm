/* colocasia_admin: Colocasia management technology administration (v1.0)
 * Colocasia planning, colocasia execution, colocasia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} loc_t;
typedef struct{int n_locp,n_loce,n_loc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} loc_state_t;
static loc_t locps[N],locss[N-2],locvss[N-4],locas[N-6],locmks[N-6]; static loc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(loc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;loc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int loc_init(void){if(init)return -1;st.n_locp=0;st.n_loce=0;st.n_loc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)locps[i].active=0;for(int i=0;i<N-2;i++)locss[i].active=0;for(int i=0;i<N-4;i++)locvss[i].active=0;for(int i=0;i<N-6;i++)locas[i].active=0;for(int i=0;i<N-6;i++)locmks[i].active=0;init=1;ps("[LOC] Colocasia initialized\n");return 0;}
int loc_planning(int t,int c,int a,int b,int d,int e,int y){return add(locps,&st.n_locp,&st.t_f1,N,t,c,a,b,d,e,y);}
int loc_execution(int t,int c,int a,int b,int d,int e,int y){return add(locss,&st.n_loce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int loc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(locvss,&st.n_loc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int loc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(locas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int loc_market(int t,int c,int a,int b,int d,int e,int y){return add(locmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void loc_report(void){ps("[LOC] Colpp: ");pi(st.n_locp);ps(" PCS=");pi(st.t_f1);ps("\nColpe: ");pi(st.n_loce);ps(" PCS=");pi(st.t_f2);ps("\nColv: ");pi(st.n_loc2);ps(" PCS=");pi(st.t_f3);ps("\nColc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void loc_state(void){ps("[LOC] Colpp=");pi(st.n_locp);ps(" Colpe=");pi(st.n_loce);ps(" Colv=");pi(st.n_loc2);ps(" Colc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Colocasia Admin Demo ===\n\n");loc_init();
ps("Colocasia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;loc_planning(t,c,1242+(i*17),1231+(i*14),1211+(i*10),1193+(i*6),2020+(i%5));}
ps("\nColocasia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;loc_execution(t,c,1231+(i*15),1220+(i*12),1202+(i*8),1189+(i*5),2021+(i%4));}
ps("\nColocasia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;loc_evaluation(t,c,1223+(i*13),1212+(i*10),1196+(i*7),1185+(i*4),2022+(i%3));}
ps("\nColocasia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;loc_accessory(t,c,1215+(i*11),1206+(i*9),1192+(i*6),1182+(i*3),2023+(i%2));}
ps("\nColocasia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;loc_market(t,c,1209+(i*9),1200+(i*7),1187+(i*5),1179+(i*3),2024);}
ps("\n");loc_report();loc_state();ps("\n=== Demo Complete ===\n");return 0;}
