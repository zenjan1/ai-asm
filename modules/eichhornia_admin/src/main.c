/* eichhornia_admin: Eichhornia management technology administration (v1.0)
 * Eichhornia planning, eichhornia execution, eichhornia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eic_t;
typedef struct{int n_eicp,n_eice,n_eic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eic_state_t;
static eic_t eicps[N],eicss[N-2],eicvss[N-4],eicas[N-6],eicmks[N-6]; static eic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eic_init(void){if(init)return -1;st.n_eicp=0;st.n_eice=0;st.n_eic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eicps[i].active=0;for(int i=0;i<N-2;i++)eicss[i].active=0;for(int i=0;i<N-4;i++)eicvss[i].active=0;for(int i=0;i<N-6;i++)eicas[i].active=0;for(int i=0;i<N-6;i++)eicmks[i].active=0;init=1;ps("[EIC] Eichhornia initialized\n");return 0;}
int eic_planning(int t,int c,int a,int b,int d,int e,int y){return add(eicps,&st.n_eicp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eic_execution(int t,int c,int a,int b,int d,int e,int y){return add(eicss,&st.n_eice,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eicvss,&st.n_eic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eicas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eic_market(int t,int c,int a,int b,int d,int e,int y){return add(eicmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eic_report(void){ps("[EIC] Eicpp: ");pi(st.n_eicp);ps(" PCS=");pi(st.t_f1);ps("\nEice: ");pi(st.n_eice);ps(" PCS=");pi(st.t_f2);ps("\nEicv: ");pi(st.n_eic2);ps(" PCS=");pi(st.t_f3);ps("\nEicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eic_state(void){ps("[EIC] Eicpp=");pi(st.n_eicp);ps(" Eice=");pi(st.n_eice);ps(" Eicv=");pi(st.n_eic2);ps(" Eicc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eichhornia Admin Demo ===\n\n");eic_init();
ps("Eichhornia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eic_planning(t,c,1140+(i*17),1129+(i*14),1109+(i*10),1091+(i*6),2020+(i%5));}
ps("\nEichhornia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eic_execution(t,c,1129+(i*15),1118+(i*12),1100+(i*8),1087+(i*5),2021+(i%4));}
ps("\nEichhornia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eic_evaluation(t,c,1121+(i*13),1110+(i*10),1094+(i*7),1083+(i*4),2022+(i%3));}
ps("\nEichhornia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eic_accessory(t,c,1113+(i*11),1104+(i*9),1090+(i*6),1080+(i*3),2023+(i%2));}
ps("\nEichhornia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eic_market(t,c,1107+(i*9),1098+(i*7),1085+(i*5),1077+(i*3),2024);}
ps("\n");eic_report();eic_state();ps("\n=== Demo Complete ===\n");return 0;}
