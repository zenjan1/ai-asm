/* reducegeartech_admin: Reducer gear technology administration (v1.0)
 * Gear reducers, worm reducers, planetary reducers, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rdg_t;
typedef struct{int n_gr,n_wr,n_pl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rdg_state_t;
static rdg_t grs[N],wrs[N-2],pls[N-4],acs[N-6],mks[N-6]; static rdg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rdg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rdg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RDG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rdg_init(void){if(init)return -1;st.n_gr=0;st.n_wr=0;st.n_pl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)grs[i].active=0;for(int i=0;i<N-2;i++)wrs[i].active=0;for(int i=0;i<N-4;i++)pls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RDG] Reducegeartech initialized\n");return 0;}
int rdg_gear(int t,int c,int a,int b,int d,int e,int y){return add(grs,&st.n_gr,&st.t_f1,N,t,c,a,b,d,e,y);}
int rdg_worm(int t,int c,int a,int b,int d,int e,int y){return add(wrs,&st.n_wr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rdg_planetary(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rdg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rdg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rdg_report(void){ps("[RDG] Gr: ");pi(st.n_gr);ps(" PCS=");pi(st.t_f1);ps("\nWr: ");pi(st.n_wr);ps(" PCS=");pi(st.t_f2);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rdg_state(void){ps("[RDG] Gr=");pi(st.n_gr);ps(" Wr=");pi(st.n_wr);ps(" Pl=");pi(st.n_pl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Reduce Gear Tech Admin Demo ===\n\n");rdg_init();
ps("Gear reducers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rdg_gear(t,c,352+(i*17),337+(i*14),317+(i*10),299+(i*6),2020+(i%5));}
ps("\nWorm reducers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rdg_worm(t,c,341+(i*15),327+(i*12),309+(i*8),296+(i*5),2021+(i%4));}
ps("\nPlanetary reducers...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rdg_planetary(t,c,333+(i*13),319+(i*10),303+(i*7),292+(i*4),2022+(i%3));}
ps("\nReducer accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdg_accessory(t,c,325+(i*11),313+(i*9),299+(i*6),289+(i*3),2023+(i%2));}
ps("\nReducer marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdg_market(t,c,319+(i*9),308+(i*7),295+(i*5),287+(i*3),2024);}
ps("\n");rdg_report();rdg_state();ps("\n=== Demo Complete ===\n");return 0;}
