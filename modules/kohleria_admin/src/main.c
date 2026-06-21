/* kohleria_admin: Kohleria management technology administration (v1.0)
 * Kohleria planning, kohleria execution, kohleria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} koh_t;
typedef struct{int n_kohp,n_kohe,n_koh2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} koh_state_t;
static koh_t kohps[N],kohss[N-2],kohvss[N-4],kohas[N-6],kohmks[N-6]; static koh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(koh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;koh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KOH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int koh_init(void){if(init)return -1;st.n_kohp=0;st.n_kohe=0;st.n_koh2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kohps[i].active=0;for(int i=0;i<N-2;i++)kohss[i].active=0;for(int i=0;i<N-4;i++)kohvss[i].active=0;for(int i=0;i<N-6;i++)kohas[i].active=0;for(int i=0;i<N-6;i++)kohmks[i].active=0;init=1;ps("[KOH] Kohleria initialized\n");return 0;}
int koh_planning(int t,int c,int a,int b,int d,int e,int y){return add(kohps,&st.n_kohp,&st.t_f1,N,t,c,a,b,d,e,y);}
int koh_execution(int t,int c,int a,int b,int d,int e,int y){return add(kohss,&st.n_kohe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int koh_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kohvss,&st.n_koh2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int koh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kohas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int koh_market(int t,int c,int a,int b,int d,int e,int y){return add(kohmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void koh_report(void){ps("[KOH] Kohp: ");pi(st.n_kohp);ps(" PCS=");pi(st.t_f1);ps("\nKohe: ");pi(st.n_kohe);ps(" PCS=");pi(st.t_f2);ps("\nKohv: ");pi(st.n_koh2);ps(" PCS=");pi(st.t_f3);ps("\nKohc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void koh_state(void){ps("[KOH] Kohp=");pi(st.n_kohp);ps(" Kohe=");pi(st.n_kohe);ps(" Kohv=");pi(st.n_koh2);ps(" Kohc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kohleria Admin Demo ===\n\n");koh_init();
ps("Kohleria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;koh_planning(t,c,933+(i*17),922+(i*14),902+(i*10),884+(i*6),2020+(i%5));}
ps("\nKohleria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;koh_execution(t,c,922+(i*15),911+(i*12),893+(i*8),880+(i*5),2021+(i%4));}
ps("\nKohleria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;koh_evaluation(t,c,914+(i*13),903+(i*10),887+(i*7),876+(i*4),2022+(i%3));}
ps("\nKohleria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koh_accessory(t,c,906+(i*11),897+(i*9),883+(i*6),873+(i*3),2023+(i%2));}
ps("\nKohleria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koh_market(t,c,900+(i*9),891+(i*7),878+(i*5),870+(i*3),2024);}
ps("\n");koh_report();koh_state();ps("\n=== Demo Complete ===\n");return 0;}
