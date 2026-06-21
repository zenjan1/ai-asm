/* spathiphyllum_admin: Spathiphyllum management technology administration (v1.0)
 * Spathiphyllum planning, spathiphyllum execution, spathiphyllum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spa_t;
typedef struct{int n_shp,n_she,n_shv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spa_state_t;
static spa_t shps[N],shss[N-2],shvss[N-4],shas[N-6],shmks[N-6]; static spa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sph_init(void){if(init)return -1;st.n_shp=0;st.n_she=0;st.n_shv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)shps[i].active=0;for(int i=0;i<N-2;i++)shss[i].active=0;for(int i=0;i<N-4;i++)shvss[i].active=0;for(int i=0;i<N-6;i++)shas[i].active=0;for(int i=0;i<N-6;i++)shmks[i].active=0;init=1;ps("[SPH] Spathiphyllum initialized\n");return 0;}
int sph_planning(int t,int c,int a,int b,int d,int e,int y){return add(shps,&st.n_shp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sph_execution(int t,int c,int a,int b,int d,int e,int y){return add(shss,&st.n_she,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(shvss,&st.n_shv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(shas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sph_market(int t,int c,int a,int b,int d,int e,int y){return add(shmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sph_report(void){ps("[SPH] Shp: ");pi(st.n_shp);ps(" PCS=");pi(st.t_f1);ps("\nShe: ");pi(st.n_she);ps(" PCS=");pi(st.t_f2);ps("\nShv: ");pi(st.n_shv);ps(" PCS=");pi(st.t_f3);ps("\nShc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sph_state(void){ps("[SPH] Shp=");pi(st.n_shp);ps(" She=");pi(st.n_she);ps(" Shv=");pi(st.n_shv);ps(" Shc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spathiphyllum Admin Demo ===\n\n");sph_init();
ps("Spathiphyllum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sph_planning(t,c,803+(i*17),792+(i*14),772+(i*10),754+(i*6),2020+(i%5));}
ps("\nSpathiphyllum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sph_execution(t,c,792+(i*15),781+(i*12),763+(i*8),750+(i*5),2021+(i%4));}
ps("\nSpathiphyllum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sph_evaluation(t,c,784+(i*13),773+(i*10),759+(i*7),748+(i*4),2022+(i%3));}
ps("\nSpathiphyllum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sph_accessory(t,c,776+(i*11),767+(i*9),753+(i*6),743+(i*3),2023+(i%2));}
ps("\nSpathiphyllum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sph_market(t,c,770+(i*9),761+(i*7),748+(i*5),740+(i*3),2024);}
ps("\n");sph_report();sph_state();ps("\n=== Demo Complete ===\n");return 0;}
