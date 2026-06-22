/* wedelia_admin: Wedelia management technology administration (v1.0)
 * Wedelia planning, wedelia execution, wedelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wed_t;
typedef struct{int n_wedp,n_wede,n_wed2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wed_state_t;
static wed_t wedps[N],wedss[N-2],wedvss[N-4],wedas[N-6],wedmks[N-6]; static wed_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wed_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wed_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wed_init(void){if(init)return -1;st.n_wedp=0;st.n_wede=0;st.n_wed2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wedps[i].active=0;for(int i=0;i<N-2;i++)wedss[i].active=0;for(int i=0;i<N-4;i++)wedvss[i].active=0;for(int i=0;i<N-6;i++)wedas[i].active=0;for(int i=0;i<N-6;i++)wedmks[i].active=0;init=1;ps("[WED] Wedelia initialized\n");return 0;}
int wed_planning(int t,int c,int a,int b,int d,int e,int y){return add(wedps,&st.n_wedp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wed_execution(int t,int c,int a,int b,int d,int e,int y){return add(wedss,&st.n_wede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wed_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wedvss,&st.n_wed2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wed_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wedas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wed_market(int t,int c,int a,int b,int d,int e,int y){return add(wedmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wed_report(void){ps("[WED] Wedpp: ");pi(st.n_wedp);ps(" PCS=");pi(st.t_f1);ps("\nWede: ");pi(st.n_wede);ps(" PCS=");pi(st.t_f2);ps("\nWedv: ");pi(st.n_wed2);ps(" PCS=");pi(st.t_f3);ps("\nWedc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wed_state(void){ps("[WED] Wedpp=");pi(st.n_wedp);ps(" Wede=");pi(st.n_wede);ps(" Wedv=");pi(st.n_wed2);ps(" Wedc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wedelia Admin Demo ===\n\n");wed_init();
ps("Wedelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wed_planning(t,c,1110+(i*17),1099+(i*14),1079+(i*10),1061+(i*6),2020+(i%5));}
ps("\nWedelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wed_execution(t,c,1099+(i*15),1088+(i*12),1070+(i*8),1057+(i*5),2021+(i%4));}
ps("\nWedelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wed_evaluation(t,c,1091+(i*13),1080+(i*10),1064+(i*7),1053+(i*4),2022+(i%3));}
ps("\nWedelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wed_accessory(t,c,1083+(i*11),1074+(i*9),1060+(i*6),1050+(i*3),2023+(i%2));}
ps("\nWedelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wed_market(t,c,1077+(i*9),1068+(i*7),1055+(i*5),1047+(i*3),2024);}
ps("\n");wed_report();wed_state();ps("\n=== Demo Complete ===\n");return 0;}
