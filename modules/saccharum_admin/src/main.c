/* saccharum_admin: Saccharum management technology administration (v1.0)
 * Saccharum planning, saccharum execution, saccharum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sac_t;
typedef struct{int n_sacp,n_sace,n_sac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sac_state_t;
static sac_t sacps[N],sacss[N-2],sacvss[N-4],sacas[N-6],sacmks[N-6]; static sac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sac_init(void){if(init)return -1;st.n_sacp=0;st.n_sace=0;st.n_sac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sacps[i].active=0;for(int i=0;i<N-2;i++)sacss[i].active=0;for(int i=0;i<N-4;i++)sacvss[i].active=0;for(int i=0;i<N-6;i++)sacas[i].active=0;for(int i=0;i<N-6;i++)sacmks[i].active=0;init=1;ps("[SAC] Saccharum initialized\n");return 0;}
int sac_planning(int t,int c,int a,int b,int d,int e,int y){return add(sacps,&st.n_sacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sac_execution(int t,int c,int a,int b,int d,int e,int y){return add(sacss,&st.n_sace,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sacvss,&st.n_sac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sacas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sac_market(int t,int c,int a,int b,int d,int e,int y){return add(sacmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sac_report(void){ps("[SAC] Sacpp: ");pi(st.n_sacp);ps(" PCS=");pi(st.t_f1);ps("\nSace: ");pi(st.n_sace);ps(" PCS=");pi(st.t_f2);ps("\nSacv: ");pi(st.n_sac2);ps(" PCS=");pi(st.t_f3);ps("\nSacc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sac_state(void){ps("[SAC] Sacpp=");pi(st.n_sacp);ps(" Sace=");pi(st.n_sace);ps(" Sacv=");pi(st.n_sac2);ps(" Sacc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Saccharum Admin Demo ===\n\n");sac_init();
ps("Saccharum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sac_planning(t,c,1099+(i*17),1088+(i*14),1068+(i*10),1050+(i*6),2020+(i%5));}
ps("\nSaccharum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sac_execution(t,c,1088+(i*15),1077+(i*12),1059+(i*8),1046+(i*5),2021+(i%4));}
ps("\nSaccharum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sac_evaluation(t,c,1080+(i*13),1069+(i*10),1053+(i*7),1042+(i*4),2022+(i%3));}
ps("\nSaccharum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sac_accessory(t,c,1072+(i*11),1063+(i*9),1049+(i*6),1039+(i*3),2023+(i%2));}
ps("\nSaccharum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sac_market(t,c,1066+(i*9),1057+(i*7),1044+(i*5),1036+(i*3),2024);}
ps("\n");sac_report();sac_state();ps("\n=== Demo Complete ===\n");return 0;}
