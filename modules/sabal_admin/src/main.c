/* sabal_admin: Sabal management technology administration (v1.0)
 * Sabal planning, sabal execution, sabal evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sab_t;
typedef struct{int n_sabp,n_sabe,n_sab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sab_state_t;
static sab_t sabps[N],sabss[N-2],sabvss[N-4],sabas[N-6],sabmks[N-6]; static sab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sab_init(void){if(init)return -1;st.n_sabp=0;st.n_sabe=0;st.n_sab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sabps[i].active=0;for(int i=0;i<N-2;i++)sabss[i].active=0;for(int i=0;i<N-4;i++)sabvss[i].active=0;for(int i=0;i<N-6;i++)sabas[i].active=0;for(int i=0;i<N-6;i++)sabmks[i].active=0;init=1;ps("[SAB] Sabal initialized\n");return 0;}
int sab_planning(int t,int c,int a,int b,int d,int e,int y){return add(sabps,&st.n_sabp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sab_execution(int t,int c,int a,int b,int d,int e,int y){return add(sabss,&st.n_sabe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sabvss,&st.n_sab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sabas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sab_market(int t,int c,int a,int b,int d,int e,int y){return add(sabmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sab_report(void){ps("[SAB] sabp: ");pi(st.n_sabp);ps(" PCS=");pi(st.t_f1);ps("\nsabe: ");pi(st.n_sabe);ps(" PCS=");pi(st.t_f2);ps("\nsabv: ");pi(st.n_sab2);ps(" PCS=");pi(st.t_f3);ps("\nsabc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sab_state(void){ps("[SAB] sabp=");pi(st.n_sabp);ps(" sabe=");pi(st.n_sabe);ps(" sabv=");pi(st.n_sab2);ps(" sabc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sabal Admin Demo ===\n\n");sab_init();
ps("Sabal planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sab_planning(t,c,1098+(i*17),1087+(i*14),1067+(i*10),1049+(i*6),2020+(i%5));}
ps("\nSabal execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sab_execution(t,c,1087+(i*15),1076+(i*12),1058+(i*8),1045+(i*5),2021+(i%4));}
ps("\nSabal evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sab_evaluation(t,c,1079+(i*13),1068+(i*10),1052+(i*7),1041+(i*4),2022+(i%3));}
ps("\nSabal accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sab_accessory(t,c,1071+(i*11),1062+(i*9),1048+(i*6),1038+(i*3),2023+(i%2));}
ps("\nSabal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sab_market(t,c,1065+(i*9),1056+(i*7),1043+(i*5),1035+(i*3),2024);}
ps("\n");sab_report();sab_state();ps("\n=== Demo Complete ===\n");return 0;}
