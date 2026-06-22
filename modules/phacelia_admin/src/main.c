/* phacelia_admin: Phacelia management technology administration (v1.0)
 * Phacelia planning, phacelia execution, phacelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phac_t;
typedef struct{int n_phacp,n_phace,n_phac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phac_state_t;
static phac_t phacps[N],phacss[N-2],phacvss[N-4],phacas[N-6],phacmks[N-6]; static phac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(phac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phac_init(void){if(init)return -1;st.n_phacp=0;st.n_phace=0;st.n_phac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)phacps[i].active=0;for(int i=0;i<N-2;i++)phacss[i].active=0;for(int i=0;i<N-4;i++)phacvss[i].active=0;for(int i=0;i<N-6;i++)phacas[i].active=0;for(int i=0;i<N-6;i++)phacmks[i].active=0;init=1;ps("[PHAC] Phacelia initialized\n");return 0;}
int phac_planning(int t,int c,int a,int b,int d,int e,int y){return add(phacps,&st.n_phacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int phac_execution(int t,int c,int a,int b,int d,int e,int y){return add(phacss,&st.n_phace,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(phacvss,&st.n_phac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(phacas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phac_market(int t,int c,int a,int b,int d,int e,int y){return add(phacmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phac_report(void){ps("[PHAC] Phapp: ");pi(st.n_phacp);ps(" PCS=");pi(st.t_f1);ps("\nPhae: ");pi(st.n_phace);ps(" PCS=");pi(st.t_f2);ps("\nPhav: ");pi(st.n_phac2);ps(" PCS=");pi(st.t_f3);ps("\nPhac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phac_state(void){ps("[PHAC] Phapp=");pi(st.n_phacp);ps(" Phae=");pi(st.n_phace);ps(" Phav=");pi(st.n_phac2);ps(" Phac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phacelia Admin Demo ===\n\n");phac_init();
ps("Phacelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phac_planning(t,c,1167+(i*17),1156+(i*14),1136+(i*10),1118+(i*6),2020+(i%5));}
ps("\nPhacelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phac_execution(t,c,1156+(i*15),1145+(i*12),1127+(i*8),1114+(i*5),2021+(i%4));}
ps("\nPhacelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phac_evaluation(t,c,1148+(i*13),1137+(i*10),1121+(i*7),1110+(i*4),2022+(i%3));}
ps("\nPhacelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phac_accessory(t,c,1140+(i*11),1131+(i*9),1117+(i*6),1107+(i*3),2023+(i%2));}
ps("\nPhacelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phac_market(t,c,1134+(i*9),1125+(i*7),1112+(i*5),1104+(i*3),2024);}
ps("\n");phac_report();phac_state();ps("\n=== Demo Complete ===\n");return 0;}
