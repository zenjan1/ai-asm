/* vitex_admin: Vitex management technology administration (v1.0)
 * Vitex planning, vitex execution, vitex evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vb_t;
typedef struct{int n_vbp,n_vbe,n_vbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vb_state_t;
static vb_t vbps[N],vbss[N-2],vbvss[N-4],vbas[N-6],vbmks[N-6]; static vb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vbn_init(void){if(init)return -1;st.n_vbp=0;st.n_vbe=0;st.n_vbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vbps[i].active=0;for(int i=0;i<N-2;i++)vbss[i].active=0;for(int i=0;i<N-4;i++)vbvss[i].active=0;for(int i=0;i<N-6;i++)vbas[i].active=0;for(int i=0;i<N-6;i++)vbmks[i].active=0;init=1;ps("[VBN] Vitex initialized\n");return 0;}
int vbn_planning(int t,int c,int a,int b,int d,int e,int y){return add(vbps,&st.n_vbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vbn_execution(int t,int c,int a,int b,int d,int e,int y){return add(vbss,&st.n_vbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vbn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vbvss,&st.n_vbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vbn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vbn_market(int t,int c,int a,int b,int d,int e,int y){return add(vbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vbn_report(void){ps("[VBN] Vbp: ");pi(st.n_vbp);ps(" PCS=");pi(st.t_f1);ps("\nVbe: ");pi(st.n_vbe);ps(" PCS=");pi(st.t_f2);ps("\nVbv: ");pi(st.n_vbv);ps(" PCS=");pi(st.t_f3);ps("\nVbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vbn_state(void){ps("[VBN] Vbp=");pi(st.n_vbp);ps(" Vbe=");pi(st.n_vbe);ps(" Vbv=");pi(st.n_vbv);ps(" Vbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vitex Admin Demo ===\n\n");vbn_init();
ps("Vitex planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vbn_planning(t,c,752+(i*17),741+(i*14),721+(i*10),703+(i*6),2020+(i%5));}
ps("\nVitex execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vbn_execution(t,c,741+(i*15),730+(i*12),712+(i*8),699+(i*5),2021+(i%4));}
ps("\nVitex evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vbn_evaluation(t,c,733+(i*13),722+(i*10),706+(i*7),695+(i*4),2022+(i%3));}
ps("\nVitex accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vbn_accessory(t,c,725+(i*11),716+(i*9),702+(i*6),692+(i*3),2023+(i%2));}
ps("\nVitex marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vbn_market(t,c,719+(i*9),710+(i*7),697+(i*5),689+(i*3),2024);}
ps("\n");vbn_report();vbn_state();ps("\n=== Demo Complete ===\n");return 0;}
