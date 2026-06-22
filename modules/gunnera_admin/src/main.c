/* gunnera_admin: Gunnera management technology administration (v1.0)
 * Gunnera planning, gunnera execution, gunnera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gun_t;
typedef struct{int n_gunp,n_gune,n_gun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gun_state_t;
static gun_t gunps[N],gunss[N-2],gunvss[N-4],gunas[N-6],gunmks[N-6]; static gun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gun_init(void){if(init)return -1;st.n_gunp=0;st.n_gune=0;st.n_gun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gunps[i].active=0;for(int i=0;i<N-2;i++)gunss[i].active=0;for(int i=0;i<N-4;i++)gunvss[i].active=0;for(int i=0;i<N-6;i++)gunas[i].active=0;for(int i=0;i<N-6;i++)gunmks[i].active=0;init=1;ps("[GUN] Gunnera initialized\n");return 0;}
int gun_planning(int t,int c,int a,int b,int d,int e,int y){return add(gunps,&st.n_gunp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gun_execution(int t,int c,int a,int b,int d,int e,int y){return add(gunss,&st.n_gune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gunvss,&st.n_gun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gunas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gun_market(int t,int c,int a,int b,int d,int e,int y){return add(gunmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gun_report(void){ps("[GUN] Gunp: ");pi(st.n_gunp);ps(" PCS=");pi(st.t_f1);ps("\nGune: ");pi(st.n_gune);ps(" PCS=");pi(st.t_f2);ps("\nGunv: ");pi(st.n_gun2);ps(" PCS=");pi(st.t_f3);ps("\nGunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gun_state(void){ps("[GUN] Gunp=");pi(st.n_gunp);ps(" Gune=");pi(st.n_gune);ps(" Gunv=");pi(st.n_gun2);ps(" Gunc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gunnera Admin Demo ===\n\n");gun_init();
ps("Gunnera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gun_planning(t,c,1289+(i*17),1278+(i*14),1258+(i*10),1240+(i*6),2020+(i%5));}
ps("\nGunnera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gun_execution(t,c,1278+(i*15),1267+(i*12),1249+(i*8),1236+(i*5),2021+(i%4));}
ps("\nGunnera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gun_evaluation(t,c,1270+(i*13),1259+(i*10),1243+(i*7),1232+(i*4),2022+(i%3));}
ps("\nGunnera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gun_accessory(t,c,1262+(i*11),1253+(i*9),1239+(i*6),1229+(i*3),2023+(i%2));}
ps("\nGunnera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gun_market(t,c,1256+(i*9),1247+(i*7),1234+(i*5),1226+(i*3),2024);}
ps("\n");gun_report();gun_state();ps("\n=== Demo Complete ===\n");return 0;}
