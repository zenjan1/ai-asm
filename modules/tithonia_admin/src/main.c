/* tithonia_admin: Tithonia management technology administration (v1.0)
 * Tithonia planning, tithonia execution, tithonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tit_t;
typedef struct{int n_titp,n_tite,n_tit2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tit_state_t;
static tit_t titps[N],titss[N-2],titvss[N-4],titas[N-6],titmks[N-6]; static tit_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tit_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tit_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TIT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tit_init(void){if(init)return -1;st.n_titp=0;st.n_tite=0;st.n_tit2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)titps[i].active=0;for(int i=0;i<N-2;i++)titss[i].active=0;for(int i=0;i<N-4;i++)titvss[i].active=0;for(int i=0;i<N-6;i++)titas[i].active=0;for(int i=0;i<N-6;i++)titmks[i].active=0;init=1;ps("[TIT] Tithonia initialized\n");return 0;}
int tit_planning(int t,int c,int a,int b,int d,int e,int y){return add(titps,&st.n_titp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tit_execution(int t,int c,int a,int b,int d,int e,int y){return add(titss,&st.n_tite,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tit_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(titvss,&st.n_tit2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tit_accessory(int t,int c,int a,int b,int d,int e,int y){return add(titas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tit_market(int t,int c,int a,int b,int d,int e,int y){return add(titmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tit_report(void){ps("[TIT] Titpp: ");pi(st.n_titp);ps(" PCS=");pi(st.t_f1);ps("\nTite: ");pi(st.n_tite);ps(" PCS=");pi(st.t_f2);ps("\nTitv: ");pi(st.n_tit2);ps(" PCS=");pi(st.t_f3);ps("\nTitc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tit_state(void){ps("[TIT] Titpp=");pi(st.n_titp);ps(" Tite=");pi(st.n_tite);ps(" Titv=");pi(st.n_tit2);ps(" Titc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tithonia Admin Demo ===\n\n");tit_init();
ps("Tithonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tit_planning(t,c,1106+(i*17),1095+(i*14),1075+(i*10),1057+(i*6),2020+(i%5));}
ps("\nTithonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tit_execution(t,c,1095+(i*15),1084+(i*12),1066+(i*8),1053+(i*5),2021+(i%4));}
ps("\nTithonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tit_evaluation(t,c,1087+(i*13),1076+(i*10),1060+(i*7),1049+(i*4),2022+(i%3));}
ps("\nTithonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tit_accessory(t,c,1079+(i*11),1070+(i*9),1056+(i*6),1046+(i*3),2023+(i%2));}
ps("\nTithonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tit_market(t,c,1073+(i*9),1064+(i*7),1051+(i*5),1043+(i*3),2024);}
ps("\n");tit_report();tit_state();ps("\n=== Demo Complete ===\n");return 0;}
