/* diervilla_admin: Diervilla management technology administration (v1.0)
 * Diervilla planning, diervilla execution, diervilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dvl_t;
typedef struct{int n_dvlp,n_dvle,n_dvl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dvl_state_t;
static dvl_t dvlps[N],dvlss[N-2],dvlvss[N-4],dvlas[N-6],dvlmks[N-6]; static dvl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dvl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dvl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DVL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dvl_init(void){if(init)return -1;st.n_dvlp=0;st.n_dvle=0;st.n_dvl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dvlps[i].active=0;for(int i=0;i<N-2;i++)dvlss[i].active=0;for(int i=0;i<N-4;i++)dvlvss[i].active=0;for(int i=0;i<N-6;i++)dvlas[i].active=0;for(int i=0;i<N-6;i++)dvlmks[i].active=0;init=1;ps("[DVL] Diervilla initialized\n");return 0;}
int dvl_planning(int t,int c,int a,int b,int d,int e,int y){return add(dvlps,&st.n_dvlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dvl_execution(int t,int c,int a,int b,int d,int e,int y){return add(dvlss,&st.n_dvle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dvl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dvlvss,&st.n_dvl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dvl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dvlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dvl_market(int t,int c,int a,int b,int d,int e,int y){return add(dvlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dvl_report(void){ps("[DVL] Diep: ");pi(st.n_dvlp);ps(" PCS=");pi(st.t_f1);ps("\nDiee: ");pi(st.n_dvle);ps(" PCS=");pi(st.t_f2);ps("\nDiev: ");pi(st.n_dvl2);ps(" PCS=");pi(st.t_f3);ps("\nDiec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dvl_state(void){ps("[DVL] Diep=");pi(st.n_dvlp);ps(" Diee=");pi(st.n_dvle);ps(" Diev=");pi(st.n_dvl2);ps(" Diec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Diervilla Admin Demo ===\n\n");dvl_init();
ps("Diervilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dvl_planning(t,c,1268+(i*17),1257+(i*14),1237+(i*10),1219+(i*6),2020+(i%5));}
ps("\nDiervilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dvl_execution(t,c,1257+(i*15),1246+(i*12),1228+(i*8),1215+(i*5),2021+(i%4));}
ps("\nDiervilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dvl_evaluation(t,c,1249+(i*13),1238+(i*10),1222+(i*7),1211+(i*4),2022+(i%3));}
ps("\nDiervilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dvl_accessory(t,c,1241+(i*11),1232+(i*9),1218+(i*6),1208+(i*3),2023+(i%2));}
ps("\nDiervilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dvl_market(t,c,1235+(i*9),1226+(i*7),1213+(i*5),1205+(i*3),2024);}
ps("\n");dvl_report();dvl_state();ps("\n=== Demo Complete ===\n");return 0;}
