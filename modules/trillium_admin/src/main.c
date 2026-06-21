/* trillium_admin: Trillium management technology administration (v1.0)
 * Trillium planning, trillium execution, trillium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tri_t;
typedef struct{int n_trip,n_trie,n_tri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tri_state_t;
static tri_t trips[N],triss[N-2],trivss[N-4],triac[N-6],trimks[N-6]; static tri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tri_init(void){if(init)return -1;st.n_trip=0;st.n_trie=0;st.n_tri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trips[i].active=0;for(int i=0;i<N-2;i++)triss[i].active=0;for(int i=0;i<N-4;i++)trivss[i].active=0;for(int i=0;i<N-6;i++)triac[i].active=0;for(int i=0;i<N-6;i++)trimks[i].active=0;init=1;ps("[TRI] Trillium initialized\n");return 0;}
int tri_planning(int t,int c,int a,int b,int d,int e,int y){return add(trips,&st.n_trip,&st.t_f1,N,t,c,a,b,d,e,y);}
int tri_execution(int t,int c,int a,int b,int d,int e,int y){return add(triss,&st.n_trie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(trivss,&st.n_tri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(triac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tri_market(int t,int c,int a,int b,int d,int e,int y){return add(trimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tri_report(void){ps("[TRI] Trip: ");pi(st.n_trip);ps(" PCS=");pi(st.t_f1);ps("\nTrie: ");pi(st.n_trie);ps(" PCS=");pi(st.t_f2);ps("\nTriv: ");pi(st.n_tri2);ps(" PCS=");pi(st.t_f3);ps("\nTric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tri_state(void){ps("[TRI] Trip=");pi(st.n_trip);ps(" Trie=");pi(st.n_trie);ps(" Triv=");pi(st.n_tri2);ps(" Tric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Trillium Admin Demo ===\n\n");tri_init();
ps("Trillium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tri_planning(t,c,948+(i*17),937+(i*14),917+(i*10),899+(i*6),2020+(i%5));}
ps("\nTrillium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tri_execution(t,c,937+(i*15),926+(i*12),908+(i*8),895+(i*5),2021+(i%4));}
ps("\nTrillium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tri_evaluation(t,c,929+(i*13),918+(i*10),902+(i*7),891+(i*4),2022+(i%3));}
ps("\nTrillium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tri_accessory(t,c,921+(i*11),912+(i*9),898+(i*6),888+(i*3),2023+(i%2));}
ps("\nTrillium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tri_market(t,c,915+(i*9),906+(i*7),893+(i*5),885+(i*3),2024);}
ps("\n");tri_report();tri_state();ps("\n=== Demo Complete ===\n");return 0;}
