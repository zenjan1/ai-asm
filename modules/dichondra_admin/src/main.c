/* dichondra_admin: Dichondra management technology administration (v1.0)
 * Dichondra planning, dichondra execution, dichondra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dch_t;
typedef struct{int n_dchp,n_dche,n_dch2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dch_state_t;
static dch_t dchps[N],dchss[N-2],dchvss[N-4],dchas[N-6],dchmks[N-6]; static dch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dch_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dch_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dch_init(void){if(init)return -1;st.n_dchp=0;st.n_dche=0;st.n_dch2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dchps[i].active=0;for(int i=0;i<N-2;i++)dchss[i].active=0;for(int i=0;i<N-4;i++)dchvss[i].active=0;for(int i=0;i<N-6;i++)dchas[i].active=0;for(int i=0;i<N-6;i++)dchmks[i].active=0;init=1;ps("[DCH] Dichondra initialized\n");return 0;}
int dch_planning(int t,int c,int a,int b,int d,int e,int y){return add(dchps,&st.n_dchp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dch_execution(int t,int c,int a,int b,int d,int e,int y){return add(dchss,&st.n_dche,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dch_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dchvss,&st.n_dch2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dch_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dchas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dch_market(int t,int c,int a,int b,int d,int e,int y){return add(dchmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dch_report(void){ps("[DCH] Dicp: ");pi(st.n_dchp);ps(" PCS=");pi(st.t_f1);ps("\nDice: ");pi(st.n_dche);ps(" PCS=");pi(st.t_f2);ps("\nDicv: ");pi(st.n_dch2);ps(" PCS=");pi(st.t_f3);ps("\nDicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dch_state(void){ps("[DCH] Dicp=");pi(st.n_dchp);ps(" Dice=");pi(st.n_dche);ps(" Dicv=");pi(st.n_dch2);ps(" Dicc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dichondra Admin Demo ===\n\n");dch_init();
ps("Dichondra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dch_planning(t,c,1263+(i*17),1252+(i*14),1232+(i*10),1214+(i*6),2020+(i%5));}
ps("\nDichondra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dch_execution(t,c,1252+(i*15),1241+(i*12),1223+(i*8),1210+(i*5),2021+(i%4));}
ps("\nDichondra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dch_evaluation(t,c,1244+(i*13),1233+(i*10),1217+(i*7),1206+(i*4),2022+(i%3));}
ps("\nDichondra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dch_accessory(t,c,1236+(i*11),1227+(i*9),1213+(i*6),1203+(i*3),2023+(i%2));}
ps("\nDichondra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dch_market(t,c,1230+(i*9),1221+(i*7),1208+(i*5),1200+(i*3),2024);}
ps("\n");dch_report();dch_state();ps("\n=== Demo Complete ===\n");return 0;}
