/* dierama_admin: Dierama management technology administration (v1.0)
 * Dierama planning, dierama execution, dierama evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drm_t;
typedef struct{int n_drmp,n_drme,n_drm2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drm_state_t;
static drm_t drmps[N],drmss[N-2],drmvss[N-4],drmas[N-6],drmmks[N-6]; static drm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(drm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drm_init(void){if(init)return -1;st.n_drmp=0;st.n_drme=0;st.n_drm2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drmps[i].active=0;for(int i=0;i<N-2;i++)drmss[i].active=0;for(int i=0;i<N-4;i++)drmvss[i].active=0;for(int i=0;i<N-6;i++)drmas[i].active=0;for(int i=0;i<N-6;i++)drmmks[i].active=0;init=1;ps("[DRM] Dierama initialized\n");return 0;}
int drm_planning(int t,int c,int a,int b,int d,int e,int y){return add(drmps,&st.n_drmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int drm_execution(int t,int c,int a,int b,int d,int e,int y){return add(drmss,&st.n_drme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(drmvss,&st.n_drm2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(drmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drm_market(int t,int c,int a,int b,int d,int e,int y){return add(drmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drm_report(void){ps("[DRM] Diep: ");pi(st.n_drmp);ps(" PCS=");pi(st.t_f1);ps("\nDiee: ");pi(st.n_drme);ps(" PCS=");pi(st.t_f2);ps("\nDiev: ");pi(st.n_drm2);ps(" PCS=");pi(st.t_f3);ps("\nDiec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drm_state(void){ps("[DRM] Diep=");pi(st.n_drmp);ps(" Diee=");pi(st.n_drme);ps(" Diev=");pi(st.n_drm2);ps(" Diec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dierama Admin Demo ===\n\n");drm_init();
ps("Dierama planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drm_planning(t,c,1265+(i*17),1254+(i*14),1234+(i*10),1216+(i*6),2020+(i%5));}
ps("\nDierama execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drm_execution(t,c,1254+(i*15),1243+(i*12),1225+(i*8),1212+(i*5),2021+(i%4));}
ps("\nDierama evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drm_evaluation(t,c,1246+(i*13),1235+(i*10),1219+(i*7),1208+(i*4),2022+(i%3));}
ps("\nDierama accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drm_accessory(t,c,1238+(i*11),1229+(i*9),1215+(i*6),1205+(i*3),2023+(i%2));}
ps("\nDierama marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drm_market(t,c,1232+(i*9),1223+(i*7),1210+(i*5),1202+(i*3),2024);}
ps("\n");drm_report();drm_state();ps("\n=== Demo Complete ===\n");return 0;}
