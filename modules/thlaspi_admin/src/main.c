/* thlaspi_admin: Thlaspi management technology administration (v1.0)
 * Thlaspi planning, thlaspi execution, thlaspi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} thl_t;
typedef struct{int n_thlp,n_thle,n_thl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} thl_state_t;
static thl_t thlps[N],thlss[N-2],thlvss[N-4],thlas[N-6],thlmks[N-6]; static thl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;thl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int thl_init(void){if(init)return -1;st.n_thlp=0;st.n_thle=0;st.n_thl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)thlps[i].active=0;for(int i=0;i<N-2;i++)thlss[i].active=0;for(int i=0;i<N-4;i++)thlvss[i].active=0;for(int i=0;i<N-6;i++)thlas[i].active=0;for(int i=0;i<N-6;i++)thlmks[i].active=0;init=1;ps("[THL] Thlaspi initialized\n");return 0;}
int thl_planning(int t,int c,int a,int b,int d,int e,int y){return add(thlps,&st.n_thlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int thl_execution(int t,int c,int a,int b,int d,int e,int y){return add(thlss,&st.n_thle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int thl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(thlvss,&st.n_thl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int thl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(thlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int thl_market(int t,int c,int a,int b,int d,int e,int y){return add(thlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void thl_report(void){ps("[THL] Thlpp: ");pi(st.n_thlp);ps(" PCS=");pi(st.t_f1);ps("\nThle: ");pi(st.n_thle);ps(" PCS=");pi(st.t_f2);ps("\nThlv: ");pi(st.n_thl2);ps(" PCS=");pi(st.t_f3);ps("\nThlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void thl_state(void){ps("[THL] Thlpp=");pi(st.n_thlp);ps(" Thle=");pi(st.n_thle);ps(" Thlv=");pi(st.n_thl2);ps(" Thlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thlaspi Admin Demo ===\n\n");thl_init();
ps("Thlaspi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;thl_planning(t,c,1103+(i*17),1092+(i*14),1072+(i*10),1054+(i*6),2020+(i%5));}
ps("\nThlaspi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;thl_execution(t,c,1092+(i*15),1081+(i*12),1063+(i*8),1050+(i*5),2021+(i%4));}
ps("\nThlaspi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;thl_evaluation(t,c,1084+(i*13),1073+(i*10),1057+(i*7),1046+(i*4),2022+(i%3));}
ps("\nThlaspi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thl_accessory(t,c,1076+(i*11),1067+(i*9),1053+(i*6),1043+(i*3),2023+(i%2));}
ps("\nThlaspi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thl_market(t,c,1070+(i*9),1061+(i*7),1048+(i*5),1040+(i*3),2024);}
ps("\n");thl_report();thl_state();ps("\n=== Demo Complete ===\n");return 0;}
