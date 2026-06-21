/* lunaria_admin: Lunaria management technology administration (v1.0)
 * Lunaria planning, lunaria execution, lunaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lun_t;
typedef struct{int n_lunp,n_lune,n_lun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lun_state_t;
static lun_t lunps[N],lunss[N-2],lunvss[N-4],lunas[N-6],lunmks[N-6]; static lun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lun_init(void){if(init)return -1;st.n_lunp=0;st.n_lune=0;st.n_lun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lunps[i].active=0;for(int i=0;i<N-2;i++)lunss[i].active=0;for(int i=0;i<N-4;i++)lunvss[i].active=0;for(int i=0;i<N-6;i++)lunas[i].active=0;for(int i=0;i<N-6;i++)lunmks[i].active=0;init=1;ps("[LUN] Lunaria initialized\n");return 0;}
int lun_planning(int t,int c,int a,int b,int d,int e,int y){return add(lunps,&st.n_lunp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lun_execution(int t,int c,int a,int b,int d,int e,int y){return add(lunss,&st.n_lune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lunvss,&st.n_lun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lunas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lun_market(int t,int c,int a,int b,int d,int e,int y){return add(lunmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lun_report(void){ps("[LUN] Lunp: ");pi(st.n_lunp);ps(" PCS=");pi(st.t_f1);ps("\nLune: ");pi(st.n_lune);ps(" PCS=");pi(st.t_f2);ps("\nLunv: ");pi(st.n_lun2);ps(" PCS=");pi(st.t_f3);ps("\nLunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lun_state(void){ps("[LUN] Lunp=");pi(st.n_lunp);ps(" Lune=");pi(st.n_lune);ps(" Lunv=");pi(st.n_lun2);ps(" Lunc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lunaria Admin Demo ===\n\n");lun_init();
ps("Lunaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lun_planning(t,c,1062+(i*17),1051+(i*14),1031+(i*10),1013+(i*6),2020+(i%5));}
ps("\nLunaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lun_execution(t,c,1051+(i*15),1040+(i*12),1022+(i*8),1009+(i*5),2021+(i%4));}
ps("\nLunaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lun_evaluation(t,c,1043+(i*13),1032+(i*10),1016+(i*7),1005+(i*4),2022+(i%3));}
ps("\nLunaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lun_accessory(t,c,1035+(i*11),1026+(i*9),1012+(i*6),1002+(i*3),2023+(i%2));}
ps("\nLunaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lun_market(t,c,1029+(i*9),1020+(i*7),1007+(i*5),999+(i*3),2024);}
ps("\n");lun_report();lun_state();ps("\n=== Demo Complete ===\n");return 0;}
