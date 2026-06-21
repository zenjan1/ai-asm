/* dicentra_admin: Dicentra management technology administration (v1.0)
 * Dicentra planning, dicentra execution, dicentra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dic_t;
typedef struct{int n_dicp,n_dice,n_dic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dic_state_t;
static dic_t dicps[N],dicss[N-2],dicvss[N-4],dicas[N-6],dicmks[N-6]; static dic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dic_init(void){if(init)return -1;st.n_dicp=0;st.n_dice=0;st.n_dic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dicps[i].active=0;for(int i=0;i<N-2;i++)dicss[i].active=0;for(int i=0;i<N-4;i++)dicvss[i].active=0;for(int i=0;i<N-6;i++)dicas[i].active=0;for(int i=0;i<N-6;i++)dicmks[i].active=0;init=1;ps("[DIC] Dicentra initialized\n");return 0;}
int dic_planning(int t,int c,int a,int b,int d,int e,int y){return add(dicps,&st.n_dicp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dic_execution(int t,int c,int a,int b,int d,int e,int y){return add(dicss,&st.n_dice,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dicvss,&st.n_dic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dicas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dic_market(int t,int c,int a,int b,int d,int e,int y){return add(dicmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dic_report(void){ps("[DIC] Diccip: ");pi(st.n_dicp);ps(" PCS=");pi(st.t_f1);ps("\nDice: ");pi(st.n_dice);ps(" PCS=");pi(st.t_f2);ps("\nDicv: ");pi(st.n_dic2);ps(" PCS=");pi(st.t_f3);ps("\nDicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dic_state(void){ps("[DIC] Diccip=");pi(st.n_dicp);ps(" Dice=");pi(st.n_dice);ps(" Dicsv=");pi(st.n_dic2);ps(" Dicc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dicentra Admin Demo ===\n\n");dic_init();
ps("Dicentra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dic_planning(t,c,992+(i*17),981+(i*14),961+(i*10),943+(i*6),2020+(i%5));}
ps("\nDicentra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dic_execution(t,c,981+(i*15),970+(i*12),952+(i*8),939+(i*5),2021+(i%4));}
ps("\nDicentra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dic_evaluation(t,c,973+(i*13),962+(i*10),946+(i*7),935+(i*4),2022+(i%3));}
ps("\nDicentra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dic_accessory(t,c,965+(i*11),956+(i*9),942+(i*6),932+(i*3),2023+(i%2));}
ps("\nDicentra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dic_market(t,c,959+(i*9),950+(i*7),937+(i*5),929+(i*3),2024);}
ps("\n");dic_report();dic_state();ps("\n=== Demo Complete ===\n");return 0;}
