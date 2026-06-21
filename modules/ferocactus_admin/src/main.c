/* ferocactus_admin: Ferocactus management technology administration (v1.0)
 * Ferocactus planning, ferocactus execution, ferocactus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fer_t;
typedef struct{int n_ferp,n_fere,n_fer2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fer_state_t;
static fer_t ferps[N],ferss[N-2],fervss[N-4],feras[N-6],fermks[N-6]; static fer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fer_init(void){if(init)return -1;st.n_ferp=0;st.n_fere=0;st.n_fer2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ferps[i].active=0;for(int i=0;i<N-2;i++)ferss[i].active=0;for(int i=0;i<N-4;i++)fervss[i].active=0;for(int i=0;i<N-6;i++)feras[i].active=0;for(int i=0;i<N-6;i++)fermks[i].active=0;init=1;ps("[FER] Ferocactus initialized\n");return 0;}
int fer_planning(int t,int c,int a,int b,int d,int e,int y){return add(ferps,&st.n_ferp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fer_execution(int t,int c,int a,int b,int d,int e,int y){return add(ferss,&st.n_fere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fervss,&st.n_fer2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(feras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fer_market(int t,int c,int a,int b,int d,int e,int y){return add(fermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fer_report(void){ps("[FER] Ferrp: ");pi(st.n_ferp);ps(" PCS=");pi(st.t_f1);ps("\nFere: ");pi(st.n_fere);ps(" PCS=");pi(st.t_f2);ps("\nFerv: ");pi(st.n_fer2);ps(" PCS=");pi(st.t_f3);ps("\nFerc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fer_state(void){ps("[FER] Ferrp=");pi(st.n_ferp);ps(" Fere=");pi(st.n_fere);ps(" Ferv=");pi(st.n_fer2);ps(" Ferc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ferocactus Admin Demo ===\n\n");fer_init();
ps("Ferocactus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fer_planning(t,c,999+(i*17),988+(i*14),968+(i*10),950+(i*6),2020+(i%5));}
ps("\nFerocactus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fer_execution(t,c,988+(i*15),977+(i*12),959+(i*8),946+(i*5),2021+(i%4));}
ps("\nFerocactus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fer_evaluation(t,c,980+(i*13),969+(i*10),953+(i*7),942+(i*4),2022+(i%3));}
ps("\nFerocactus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fer_accessory(t,c,972+(i*11),963+(i*9),949+(i*6),939+(i*3),2023+(i%2));}
ps("\nFerocactus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fer_market(t,c,966+(i*9),957+(i*7),944+(i*5),936+(i*3),2024);}
ps("\n");fer_report();fer_state();ps("\n=== Demo Complete ===\n");return 0;}
