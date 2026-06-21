/* rinorea_admin: Rinorea management technology administration (v1.0)
 * Rinorea planning, rinorea execution, rinorea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rin_t;
typedef struct{int n_rinp,n_rine,n_rin2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rin_state_t;
static rin_t rinps[N],rinss[N-2],rinvss[N-4],rinas[N-6],rinmks[N-6]; static rin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rin_init(void){if(init)return -1;st.n_rinp=0;st.n_rine=0;st.n_rin2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rinps[i].active=0;for(int i=0;i<N-2;i++)rinss[i].active=0;for(int i=0;i<N-4;i++)rinvss[i].active=0;for(int i=0;i<N-6;i++)rinas[i].active=0;for(int i=0;i<N-6;i++)rinmks[i].active=0;init=1;ps("[RIN] Rinorea initialized\n");return 0;}
int rin_planning(int t,int c,int a,int b,int d,int e,int y){return add(rinps,&st.n_rinp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rin_execution(int t,int c,int a,int b,int d,int e,int y){return add(rinss,&st.n_rine,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rinvss,&st.n_rin2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rinas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rin_market(int t,int c,int a,int b,int d,int e,int y){return add(rinmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rin_report(void){ps("[RIN] rinp: ");pi(st.n_rinp);ps(" PCS=");pi(st.t_f1);ps("\nrine: ");pi(st.n_rine);ps(" PCS=");pi(st.t_f2);ps("\nrinv: ");pi(st.n_rin2);ps(" PCS=");pi(st.t_f3);ps("\nrinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rin_state(void){ps("[RIN] rinp=");pi(st.n_rinp);ps(" rine=");pi(st.n_rine);ps(" rinv=");pi(st.n_rin2);ps(" rinc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rinorea Admin Demo ===\n\n");rin_init();
ps("Rinorea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rin_planning(t,c,1094+(i*17),1083+(i*14),1063+(i*10),1045+(i*6),2020+(i%5));}
ps("\nRinorea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rin_execution(t,c,1083+(i*15),1072+(i*12),1054+(i*8),1041+(i*5),2021+(i%4));}
ps("\nRinorea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rin_evaluation(t,c,1075+(i*13),1064+(i*10),1048+(i*7),1037+(i*4),2022+(i%3));}
ps("\nRinorea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rin_accessory(t,c,1067+(i*11),1058+(i*9),1044+(i*6),1034+(i*3),2023+(i%2));}
ps("\nRinorea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rin_market(t,c,1061+(i*9),1052+(i*7),1039+(i*5),1031+(i*3),2024);}
ps("\n");rin_report();rin_state();ps("\n=== Demo Complete ===\n");return 0;}
