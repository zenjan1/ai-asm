/* nymphaea_admin: Nymphaea management technology administration (v1.0)
 * Nymphaea planning, nymphaea execution, nymphaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nym_t;
typedef struct{int n_nymp,n_nyme,n_nym2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nym_state_t;
static nym_t nymps[N],nymss[N-2],nymvss[N-4],nymas[N-6],nymmks[N-6]; static nym_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nym_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nym_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NYM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nym_init(void){if(init)return -1;st.n_nymp=0;st.n_nyme=0;st.n_nym2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nymps[i].active=0;for(int i=0;i<N-2;i++)nymss[i].active=0;for(int i=0;i<N-4;i++)nymvss[i].active=0;for(int i=0;i<N-6;i++)nymas[i].active=0;for(int i=0;i<N-6;i++)nymmks[i].active=0;init=1;ps("[NYM] Nymphaea initialized\n");return 0;}
int nym_planning(int t,int c,int a,int b,int d,int e,int y){return add(nymps,&st.n_nymp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nym_execution(int t,int c,int a,int b,int d,int e,int y){return add(nymss,&st.n_nyme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nym_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nymvss,&st.n_nym2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nym_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nymas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nym_market(int t,int c,int a,int b,int d,int e,int y){return add(nymmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nym_report(void){ps("[NYM] Nympp: ");pi(st.n_nymp);ps(" PCS=");pi(st.t_f1);ps("\nNyme: ");pi(st.n_nyme);ps(" PCS=");pi(st.t_f2);ps("\nNymv: ");pi(st.n_nym2);ps(" PCS=");pi(st.t_f3);ps("\nNymc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nym_state(void){ps("[NYM] Nympp=");pi(st.n_nymp);ps(" Nyme=");pi(st.n_nyme);ps(" Nymv=");pi(st.n_nym2);ps(" Nymc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nymphaea Admin Demo ===\n\n");nym_init();
ps("Nymphaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nym_planning(t,c,1070+(i*17),1059+(i*14),1039+(i*10),1021+(i*6),2020+(i%5));}
ps("\nNymphaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nym_execution(t,c,1059+(i*15),1048+(i*12),1030+(i*8),1017+(i*5),2021+(i%4));}
ps("\nNymphaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nym_evaluation(t,c,1051+(i*13),1040+(i*10),1024+(i*7),1013+(i*4),2022+(i%3));}
ps("\nNymphaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nym_accessory(t,c,1043+(i*11),1034+(i*9),1020+(i*6),1010+(i*3),2023+(i%2));}
ps("\nNymphaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nym_market(t,c,1037+(i*9),1028+(i*7),1015+(i*5),1007+(i*3),2024);}
ps("\n");nym_report();nym_state();ps("\n=== Demo Complete ===\n");return 0;}
