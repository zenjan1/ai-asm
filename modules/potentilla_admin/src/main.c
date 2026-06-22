/* potentilla_admin: Potentilla management technology administration (v1.0)
 * Potentilla planning, potentilla execution, potentilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pot_t;
typedef struct{int n_potp,n_pote,n_pot2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pot_state_t;
static pot_t potps[N],potss[N-2],potvss[N-4],potas[N-6],potmks[N-6]; static pot_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pot_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pot_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[POT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pot_init(void){if(init)return -1;st.n_potp=0;st.n_pote=0;st.n_pot2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)potps[i].active=0;for(int i=0;i<N-2;i++)potss[i].active=0;for(int i=0;i<N-4;i++)potvss[i].active=0;for(int i=0;i<N-6;i++)potas[i].active=0;for(int i=0;i<N-6;i++)potmks[i].active=0;init=1;ps("[POT] Potentilla initialized\n");return 0;}
int pot_planning(int t,int c,int a,int b,int d,int e,int y){return add(potps,&st.n_potp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pot_execution(int t,int c,int a,int b,int d,int e,int y){return add(potss,&st.n_pote,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pot_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(potvss,&st.n_pot2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pot_accessory(int t,int c,int a,int b,int d,int e,int y){return add(potas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pot_market(int t,int c,int a,int b,int d,int e,int y){return add(potmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pot_report(void){ps("[POT] Potpp: ");pi(st.n_potp);ps(" PCS=");pi(st.t_f1);ps("\nPote: ");pi(st.n_pote);ps(" PCS=");pi(st.t_f2);ps("\nPotv: ");pi(st.n_pot2);ps(" PCS=");pi(st.t_f3);ps("\nPotc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pot_state(void){ps("[POT] Potpp=");pi(st.n_potp);ps(" Pote=");pi(st.n_pote);ps(" Potv=");pi(st.n_pot2);ps(" Potc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Potentilla Admin Demo ===\n\n");pot_init();
ps("Potentilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pot_planning(t,c,1172+(i*17),1161+(i*14),1141+(i*10),1123+(i*6),2020+(i%5));}
ps("\nPotentilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pot_execution(t,c,1161+(i*15),1150+(i*12),1132+(i*8),1119+(i*5),2021+(i%4));}
ps("\nPotentilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pot_evaluation(t,c,1153+(i*13),1142+(i*10),1126+(i*7),1115+(i*4),2022+(i%3));}
ps("\nPotentilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pot_accessory(t,c,1145+(i*11),1136+(i*9),1122+(i*6),1112+(i*3),2023+(i%2));}
ps("\nPotentilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pot_market(t,c,1139+(i*9),1130+(i*7),1117+(i*5),1109+(i*3),2024);}
ps("\n");pot_report();pot_state();ps("\n=== Demo Complete ===\n");return 0;}
