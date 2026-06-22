/* pogonia_admin: Pogonia management technology administration (v1.0)
 * Pogonia planning, pogonia execution, pogonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pog_t;
typedef struct{int n_pogp,n_poge,n_pog2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pog_state_t;
static pog_t pogps[N],pogss[N-2],pogvss[N-4],pogas[N-6],pogmks[N-6]; static pog_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pog_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pog_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[POG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pog_init(void){if(init)return -1;st.n_pogp=0;st.n_poge=0;st.n_pog2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pogps[i].active=0;for(int i=0;i<N-2;i++)pogss[i].active=0;for(int i=0;i<N-4;i++)pogvss[i].active=0;for(int i=0;i<N-6;i++)pogas[i].active=0;for(int i=0;i<N-6;i++)pogmks[i].active=0;init=1;ps("[POG] Pogonia initialized\n");return 0;}
int pog_planning(int t,int c,int a,int b,int d,int e,int y){return add(pogps,&st.n_pogp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pog_execution(int t,int c,int a,int b,int d,int e,int y){return add(pogss,&st.n_poge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pog_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pogvss,&st.n_pog2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pog_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pogas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pog_market(int t,int c,int a,int b,int d,int e,int y){return add(pogmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pog_report(void){ps("[POG] Pogpp: ");pi(st.n_pogp);ps(" PCS=");pi(st.t_f1);ps("\nPoge: ");pi(st.n_poge);ps(" PCS=");pi(st.t_f2);ps("\nPogv: ");pi(st.n_pog2);ps(" PCS=");pi(st.t_f3);ps("\nPogc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pog_state(void){ps("[POG] Pogpp=");pi(st.n_pogp);ps(" Poge=");pi(st.n_poge);ps(" Pogv=");pi(st.n_pog2);ps(" Pogc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pogonia Admin Demo ===\n\n");pog_init();
ps("Pogonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pog_planning(t,c,1171+(i*17),1160+(i*14),1140+(i*10),1122+(i*6),2020+(i%5));}
ps("\nPogonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pog_execution(t,c,1160+(i*15),1149+(i*12),1131+(i*8),1118+(i*5),2021+(i%4));}
ps("\nPogonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pog_evaluation(t,c,1152+(i*13),1141+(i*10),1125+(i*7),1114+(i*4),2022+(i%3));}
ps("\nPogonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pog_accessory(t,c,1144+(i*11),1135+(i*9),1121+(i*6),1111+(i*3),2023+(i%2));}
ps("\nPogonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pog_market(t,c,1138+(i*9),1129+(i*7),1116+(i*5),1108+(i*3),2024);}
ps("\n");pog_report();pog_state();ps("\n=== Demo Complete ===\n");return 0;}
