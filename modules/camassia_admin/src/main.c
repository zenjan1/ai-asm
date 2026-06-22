/* camassia_admin: Camassia management technology administration (v1.0)
 * Camassia planning, camassia execution, camassia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cms_t;
typedef struct{int n_cmsp,n_cmse,n_cms2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cms_state_t;
static cms_t cmsps[N],cmsss[N-2],cmsvss[N-4],cmsas[N-6],cmsmks[N-6]; static cms_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cms_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cms_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cms_init(void){if(init)return -1;st.n_cmsp=0;st.n_cmse=0;st.n_cms2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cmsps[i].active=0;for(int i=0;i<N-2;i++)cmsss[i].active=0;for(int i=0;i<N-4;i++)cmsvss[i].active=0;for(int i=0;i<N-6;i++)cmsas[i].active=0;for(int i=0;i<N-6;i++)cmsmks[i].active=0;init=1;ps("[CMS] Camassia initialized\n");return 0;}
int cms_planning(int t,int c,int a,int b,int d,int e,int y){return add(cmsps,&st.n_cmsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cms_execution(int t,int c,int a,int b,int d,int e,int y){return add(cmsss,&st.n_cmse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cms_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cmsvss,&st.n_cms2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cms_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cmsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cms_market(int t,int c,int a,int b,int d,int e,int y){return add(cmsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cms_report(void){ps("[CMS] Campp: ");pi(st.n_cmsp);ps(" PCS=");pi(st.t_f1);ps("\nCampe: ");pi(st.n_cmse);ps(" PCS=");pi(st.t_f2);ps("\nCamv: ");pi(st.n_cms2);ps(" PCS=");pi(st.t_f3);ps("\nCamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cms_state(void){ps("[CMS] Campp=");pi(st.n_cmsp);ps(" Campe=");pi(st.n_cmse);ps(" Camv=");pi(st.n_cms2);ps(" Camc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Camassia Admin Demo ===\n\n");cms_init();
ps("Camassia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cms_planning(t,c,1229+(i*17),1218+(i*14),1198+(i*10),1180+(i*6),2020+(i%5));}
ps("\nCamassia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cms_execution(t,c,1218+(i*15),1207+(i*12),1189+(i*8),1176+(i*5),2021+(i%4));}
ps("\nCamassia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cms_evaluation(t,c,1210+(i*13),1199+(i*10),1183+(i*7),1172+(i*4),2022+(i%3));}
ps("\nCamassia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cms_accessory(t,c,1202+(i*11),1193+(i*9),1179+(i*6),1169+(i*3),2023+(i%2));}
ps("\nCamassia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cms_market(t,c,1196+(i*9),1187+(i*7),1174+(i*5),1166+(i*3),2024);}
ps("\n");cms_report();cms_state();ps("\n=== Demo Complete ===\n");return 0;}
