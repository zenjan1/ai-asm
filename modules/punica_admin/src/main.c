/* punica_admin: Punica management technology administration (v1.0)
 * Punica planning, punica execution, punica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pun_t;
typedef struct{int n_punp,n_pune,n_pun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pun_state_t;
static pun_t punps[N],punss[N-2],punvss[N-4],punas[N-6],punmks[N-6]; static pun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pun_init(void){if(init)return -1;st.n_punp=0;st.n_pune=0;st.n_pun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)punps[i].active=0;for(int i=0;i<N-2;i++)punss[i].active=0;for(int i=0;i<N-4;i++)punvss[i].active=0;for(int i=0;i<N-6;i++)punas[i].active=0;for(int i=0;i<N-6;i++)punmks[i].active=0;init=1;ps("[PUN] Punica initialized\n");return 0;}
int pun_planning(int t,int c,int a,int b,int d,int e,int y){return add(punps,&st.n_punp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pun_execution(int t,int c,int a,int b,int d,int e,int y){return add(punss,&st.n_pune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(punvss,&st.n_pun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(punas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pun_market(int t,int c,int a,int b,int d,int e,int y){return add(punmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pun_report(void){ps("[PUN] punp: ");pi(st.n_punp);ps(" PCS=");pi(st.t_f1);ps("\npune: ");pi(st.n_pune);ps(" PCS=");pi(st.t_f2);ps("\npunv: ");pi(st.n_pun2);ps(" PCS=");pi(st.t_f3);ps("\npunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pun_state(void){ps("[PUN] punp=");pi(st.n_punp);ps(" pune=");pi(st.n_pune);ps(" punv=");pi(st.n_pun2);ps(" punc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Punica Admin Demo ===\n\n");pun_init();
ps("Punica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pun_planning(t,c,1089+(i*17),1078+(i*14),1058+(i*10),1040+(i*6),2020+(i%5));}
ps("\nPunica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pun_execution(t,c,1078+(i*15),1067+(i*12),1049+(i*8),1036+(i*5),2021+(i%4));}
ps("\nPunica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pun_evaluation(t,c,1070+(i*13),1059+(i*10),1043+(i*7),1032+(i*4),2022+(i%3));}
ps("\nPunica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pun_accessory(t,c,1062+(i*11),1053+(i*9),1039+(i*6),1029+(i*3),2023+(i%2));}
ps("\nPunica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pun_market(t,c,1056+(i*9),1047+(i*7),1034+(i*5),1026+(i*3),2024);}
ps("\n");pun_report();pun_state();ps("\n=== Demo Complete ===\n");return 0;}
