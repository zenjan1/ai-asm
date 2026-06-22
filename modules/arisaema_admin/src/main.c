/* arisaema_admin: Arisaema management technology administration (v1.0)
 * Arisaema planning, arisaema execution, arisaema evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ari_t;
typedef struct{int n_arip,n_arie,n_ari2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ari_state_t;
static ari_t arips[N],ariss[N-2],arivss[N-4],arias[N-6],arimks[N-6]; static ari_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ari_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ari_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ari_init(void){if(init)return -1;st.n_arip=0;st.n_arie=0;st.n_ari2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arips[i].active=0;for(int i=0;i<N-2;i++)ariss[i].active=0;for(int i=0;i<N-4;i++)arivss[i].active=0;for(int i=0;i<N-6;i++)arias[i].active=0;for(int i=0;i<N-6;i++)arimks[i].active=0;init=1;ps("[ARI] Arisaema initialized\n");return 0;}
int ari_planning(int t,int c,int a,int b,int d,int e,int y){return add(arips,&st.n_arip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ari_execution(int t,int c,int a,int b,int d,int e,int y){return add(ariss,&st.n_arie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ari_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arivss,&st.n_ari2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ari_accessory(int t,int c,int a,int b,int d,int e,int y){return add(arias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ari_market(int t,int c,int a,int b,int d,int e,int y){return add(arimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ari_report(void){ps("[ARI] Aripp: ");pi(st.n_arip);ps(" PCS=");pi(st.t_f1);ps("\nArie: ");pi(st.n_arie);ps(" PCS=");pi(st.t_f2);ps("\nAriv: ");pi(st.n_ari2);ps(" PCS=");pi(st.t_f3);ps("\nAric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ari_state(void){ps("[ARI] Aripp=");pi(st.n_arip);ps(" Arie=");pi(st.n_arie);ps(" Ariv=");pi(st.n_ari2);ps(" Aric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arisaema Admin Demo ===\n\n");ari_init();
ps("Arisaema planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ari_planning(t,c,1120+(i*17),1109+(i*14),1089+(i*10),1071+(i*6),2020+(i%5));}
ps("\nArisaema execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ari_execution(t,c,1109+(i*15),1098+(i*12),1080+(i*8),1067+(i*5),2021+(i%4));}
ps("\nArisaema evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ari_evaluation(t,c,1101+(i*13),1090+(i*10),1074+(i*7),1063+(i*4),2022+(i%3));}
ps("\nArisaema accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ari_accessory(t,c,1093+(i*11),1084+(i*9),1070+(i*6),1060+(i*3),2023+(i%2));}
ps("\nArisaema marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ari_market(t,c,1087+(i*9),1078+(i*7),1065+(i*5),1057+(i*3),2024);}
ps("\n");ari_report();ari_state();ps("\n=== Demo Complete ===\n");return 0;}
