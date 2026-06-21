/* lobelia_admin: Lobelia management technology administration (v1.0)
 * Lobelia planning, lobelia execution, lobelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lob_t;
typedef struct{int n_lobp,n_lobe,n_lob2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lob_state_t;
static lob_t lobps[N],lobss[N-2],lobvss[N-4],lobas[N-6],lobmks[N-6]; static lob_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lob_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lob_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lob_init(void){if(init)return -1;st.n_lobp=0;st.n_lobe=0;st.n_lob2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lobps[i].active=0;for(int i=0;i<N-2;i++)lobss[i].active=0;for(int i=0;i<N-4;i++)lobvss[i].active=0;for(int i=0;i<N-6;i++)lobas[i].active=0;for(int i=0;i<N-6;i++)lobmks[i].active=0;init=1;ps("[LOB] Lobelia initialized\n");return 0;}
int lob_planning(int t,int c,int a,int b,int d,int e,int y){return add(lobps,&st.n_lobp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lob_execution(int t,int c,int a,int b,int d,int e,int y){return add(lobss,&st.n_lobe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lob_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lobvss,&st.n_lob2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lob_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lobas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lob_market(int t,int c,int a,int b,int d,int e,int y){return add(lobmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lob_report(void){ps("[LOB] Lobp: ");pi(st.n_lobp);ps(" PCS=");pi(st.t_f1);ps("\nLobe: ");pi(st.n_lobe);ps(" PCS=");pi(st.t_f2);ps("\nLobv: ");pi(st.n_lob2);ps(" PCS=");pi(st.t_f3);ps("\nLobc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lob_state(void){ps("[LOB] Lobp=");pi(st.n_lobp);ps(" Lobe=");pi(st.n_lobe);ps(" Lobv=");pi(st.n_lob2);ps(" Lobc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lobelia Admin Demo ===\n\n");lob_init();
ps("Lobelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lob_planning(t,c,936+(i*17),925+(i*14),905+(i*10),887+(i*6),2020+(i%5));}
ps("\nLobelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lob_execution(t,c,925+(i*15),914+(i*12),896+(i*8),883+(i*5),2021+(i%4));}
ps("\nLobelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lob_evaluation(t,c,917+(i*13),906+(i*10),890+(i*7),879+(i*4),2022+(i%3));}
ps("\nLobelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lob_accessory(t,c,909+(i*11),900+(i*9),886+(i*6),876+(i*3),2023+(i%2));}
ps("\nLobelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lob_market(t,c,903+(i*9),894+(i*7),881+(i*5),873+(i*3),2024);}
ps("\n");lob_report();lob_state();ps("\n=== Demo Complete ===\n");return 0;}
