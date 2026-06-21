/* gesneria_admin: Gesneria management technology administration (v1.0)
 * Gesneria planning, gesneria execution, gesneria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gs_t;
typedef struct{int n_gnp,n_gne,n_gnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gs_state_t;
static gs_t gnps[N],gnss[N-2],gnvss[N-4],gnas[N-6],gnmks[N-6]; static gs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GSN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gsn_init(void){if(init)return -1;st.n_gnp=0;st.n_gne=0;st.n_gnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gnps[i].active=0;for(int i=0;i<N-2;i++)gnss[i].active=0;for(int i=0;i<N-4;i++)gnvss[i].active=0;for(int i=0;i<N-6;i++)gnas[i].active=0;for(int i=0;i<N-6;i++)gnmks[i].active=0;init=1;ps("[GSN] Gesneria initialized\n");return 0;}
int gsn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gnps,&st.n_gnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gsn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gnss,&st.n_gne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gsn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gnvss,&st.n_gnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gsn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gsn_market(int t,int c,int a,int b,int d,int e,int y){return add(gnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gsn_report(void){ps("[GSN] Gnp: ");pi(st.n_gnp);ps(" PCS=");pi(st.t_f1);ps("\nGne: ");pi(st.n_gne);ps(" PCS=");pi(st.t_f2);ps("\nGnv: ");pi(st.n_gnv);ps(" PCS=");pi(st.t_f3);ps("\nGnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gsn_state(void){ps("[GSN] Gnp=");pi(st.n_gnp);ps(" Gne=");pi(st.n_gne);ps(" Gnv=");pi(st.n_gnv);ps(" Gnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gesneria Admin Demo ===\n\n");gsn_init();
ps("Gesneria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gsn_planning(t,c,778+(i*17),767+(i*14),747+(i*10),729+(i*6),2020+(i%5));}
ps("\nGesneria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gsn_execution(t,c,767+(i*15),756+(i*12),738+(i*8),725+(i*5),2021+(i%4));}
ps("\nGesneria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gsn_evaluation(t,c,759+(i*13),748+(i*10),734+(i*7),723+(i*4),2022+(i%3));}
ps("\nGesneria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gsn_accessory(t,c,751+(i*11),742+(i*9),728+(i*6),718+(i*3),2023+(i%2));}
ps("\nGesneria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gsn_market(t,c,745+(i*9),736+(i*7),723+(i*5),715+(i*3),2024);}
ps("\n");gsn_report();gsn_state();ps("\n=== Demo Complete ===\n");return 0;}
