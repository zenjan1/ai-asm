/* massonia_admin: Massonia management technology administration (v1.0)
 * Massonia planning, massonia execution, massonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mas_t;
typedef struct{int n_masp,n_mase,n_mas2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mas_state_t;
static mas_t masps[N],masses[N-2],masvss[N-4],masas[N-6],masmks[N-6]; static mas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mas_init(void){if(init)return -1;st.n_masp=0;st.n_mase=0;st.n_mas2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)masps[i].active=0;for(int i=0;i<N-2;i++)masses[i].active=0;for(int i=0;i<N-4;i++)masvss[i].active=0;for(int i=0;i<N-6;i++)masas[i].active=0;for(int i=0;i<N-6;i++)masmks[i].active=0;init=1;ps("[MAS] Massonia initialized\n");return 0;}
int mas_planning(int t,int c,int a,int b,int d,int e,int y){return add(masps,&st.n_masp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mas_execution(int t,int c,int a,int b,int d,int e,int y){return add(masses,&st.n_mase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(masvss,&st.n_mas2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(masas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mas_market(int t,int c,int a,int b,int d,int e,int y){return add(masmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mas_report(void){ps("[MAS] Masp: ");pi(st.n_masp);ps(" PCS=");pi(st.t_f1);ps("\nMase: ");pi(st.n_mase);ps(" PCS=");pi(st.t_f2);ps("\nMasv: ");pi(st.n_mas2);ps(" PCS=");pi(st.t_f3);ps("\nMasc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mas_state(void){ps("[MAS] Masp=");pi(st.n_masp);ps(" Mase=");pi(st.n_mase);ps(" Masv=");pi(st.n_mas2);ps(" Masc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Massonia Admin Demo ===\n\n");mas_init();
ps("Massonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mas_planning(t,c,1312+(i*17),1301+(i*14),1281+(i*10),1263+(i*6),2020+(i%5));}
ps("\nMassonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mas_execution(t,c,1301+(i*15),1290+(i*12),1272+(i*8),1259+(i*5),2021+(i%4));}
ps("\nMassonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mas_evaluation(t,c,1293+(i*13),1282+(i*10),1266+(i*7),1255+(i*4),2022+(i%3));}
ps("\nMassonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mas_accessory(t,c,1285+(i*11),1276+(i*9),1262+(i*6),1252+(i*3),2023+(i%2));}
ps("\nMassonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mas_market(t,c,1279+(i*9),1270+(i*7),1257+(i*5),1249+(i*3),2024);}
ps("\n");mas_report();mas_state();ps("\n=== Demo Complete ===\n");return 0;}
