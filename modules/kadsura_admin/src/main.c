/* kadsura_admin: Kadsura management technology administration (v1.0)
 * Kadsura planning, kadsura execution, kadsura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kad_t;
typedef struct{int n_kadp,n_kade,n_kad2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kad_state_t;
static kad_t kadps[N],kadss[N-2],kadvss[N-4],kadas[N-6],kadkmks[N-6]; static kad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kad_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kad_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KAD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kad_init(void){if(init)return -1;st.n_kadp=0;st.n_kade=0;st.n_kad2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kadps[i].active=0;for(int i=0;i<N-2;i++)kadss[i].active=0;for(int i=0;i<N-4;i++)kadvss[i].active=0;for(int i=0;i<N-6;i++)kadas[i].active=0;for(int i=0;i<N-6;i++)kadkmks[i].active=0;init=1;ps("[KAD] Kadsura initialized\n");return 0;}
int kad_planning(int t,int c,int a,int b,int d,int e,int y){return add(kadps,&st.n_kadp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kad_execution(int t,int c,int a,int b,int d,int e,int y){return add(kadss,&st.n_kade,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kad_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kadvss,&st.n_kad2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kad_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kadas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kad_market(int t,int c,int a,int b,int d,int e,int y){return add(kadkmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kad_report(void){ps("[KAD] Kadp: ");pi(st.n_kadp);ps(" PCS=");pi(st.t_f1);ps("\nKade: ");pi(st.n_kade);ps(" PCS=");pi(st.t_f2);ps("\nKadv: ");pi(st.n_kad2);ps(" PCS=");pi(st.t_f3);ps("\nKadc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kad_state(void){ps("[KAD] Kadp=");pi(st.n_kadp);ps(" Kade=");pi(st.n_kade);ps(" Kadv=");pi(st.n_kad2);ps(" Kadc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kadsura Admin Demo ===\n\n");kad_init();
ps("Kadsura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kad_planning(t,c,1034+(i*17),1023+(i*14),1003+(i*10),985+(i*6),2020+(i%5));}
ps("\nKadsura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kad_execution(t,c,1023+(i*15),1012+(i*12),994+(i*8),981+(i*5),2021+(i%4));}
ps("\nKadsura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kad_evaluation(t,c,1015+(i*13),1004+(i*10),988+(i*7),977+(i*4),2022+(i%3));}
ps("\nKadsura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kad_accessory(t,c,1007+(i*11),998+(i*9),984+(i*6),974+(i*3),2023+(i%2));}
ps("\nKadsura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kad_market(t,c,1001+(i*9),992+(i*7),979+(i*5),971+(i*3),2024);}
ps("\n");kad_report();kad_state();ps("\n=== Demo Complete ===\n");return 0;}
