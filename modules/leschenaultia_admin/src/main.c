/* leschenaultia_admin: Leschenaultia management technology administration (v1.0)
 * Leschenaultia planning, leschenaultia execution, leschenaultia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} les_t;
typedef struct{int n_lesp,n_lese,n_les2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} les_state_t;
static les_t lesps[N],lesss[N-2],lesvss[N-4],lesas[N-6],lesmks[N-6]; static les_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(les_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;les_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int les_init(void){if(init)return -1;st.n_lesp=0;st.n_lese=0;st.n_les2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lesps[i].active=0;for(int i=0;i<N-2;i++)lesss[i].active=0;for(int i=0;i<N-4;i++)lesvss[i].active=0;for(int i=0;i<N-6;i++)lesas[i].active=0;for(int i=0;i<N-6;i++)lesmks[i].active=0;init=1;ps("[LES] Leschenaultia initialized\n");return 0;}
int les_planning(int t,int c,int a,int b,int d,int e,int y){return add(lesps,&st.n_lesp,&st.t_f1,N,t,c,a,b,d,e,y);}
int les_execution(int t,int c,int a,int b,int d,int e,int y){return add(lesss,&st.n_lese,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int les_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lesvss,&st.n_les2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int les_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lesas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int les_market(int t,int c,int a,int b,int d,int e,int y){return add(lesmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void les_report(void){ps("[LES] Lespp: ");pi(st.n_lesp);ps(" PCS=");pi(st.t_f1);ps("\nLese: ");pi(st.n_lese);ps(" PCS=");pi(st.t_f2);ps("\nLesv: ");pi(st.n_les2);ps(" PCS=");pi(st.t_f3);ps("\nLesc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void les_state(void){ps("[LES] Lespp=");pi(st.n_lesp);ps(" Lese=");pi(st.n_lese);ps(" Lesv=");pi(st.n_les2);ps(" Lesc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leschenaultia Admin Demo ===\n\n");les_init();
ps("Leschenaultia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;les_planning(t,c,1154+(i*17),1143+(i*14),1123+(i*10),1105+(i*6),2020+(i%5));}
ps("\nLeschenaultia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;les_execution(t,c,1143+(i*15),1132+(i*12),1114+(i*8),1101+(i*5),2021+(i%4));}
ps("\nLeschenaultia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;les_evaluation(t,c,1135+(i*13),1124+(i*10),1108+(i*7),1097+(i*4),2022+(i%3));}
ps("\nLeschenaultia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;les_accessory(t,c,1127+(i*11),1118+(i*9),1104+(i*6),1094+(i*3),2023+(i%2));}
ps("\nLeschenaultia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;les_market(t,c,1121+(i*9),1112+(i*7),1099+(i*5),1091+(i*3),2024);}
ps("\n");les_report();les_state();ps("\n=== Demo Complete ===\n");return 0;}
