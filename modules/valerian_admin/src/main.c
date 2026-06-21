/* valerian_admin: Valerian management technology administration (v1.0)
 * Valerian planning, valerian execution, valerian evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vl_t;
typedef struct{int n_vlp,n_vle,n_vlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vl_state_t;
static vl_t vlps[N],vlss[N-2],vlvss[N-4],vlas[N-6],vlmks[N-6]; static vl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VLN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vl_init(void){if(init)return -1;st.n_vlp=0;st.n_vle=0;st.n_vlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vlps[i].active=0;for(int i=0;i<N-2;i++)vlss[i].active=0;for(int i=0;i<N-4;i++)vlvss[i].active=0;for(int i=0;i<N-6;i++)vlas[i].active=0;for(int i=0;i<N-6;i++)vlmks[i].active=0;init=1;ps("[VLN] Valerian initialized\n");return 0;}
int vl_planning(int t,int c,int a,int b,int d,int e,int y){return add(vlps,&st.n_vlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vl_execution(int t,int c,int a,int b,int d,int e,int y){return add(vlss,&st.n_vle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vlvss,&st.n_vlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vl_market(int t,int c,int a,int b,int d,int e,int y){return add(vlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vl_report(void){ps("[VLN] Vlp: ");pi(st.n_vlp);ps(" PCS=");pi(st.t_f1);ps("\nVle: ");pi(st.n_vle);ps(" PCS=");pi(st.t_f2);ps("\nVlv: ");pi(st.n_vlv);ps(" PCS=");pi(st.t_f3);ps("\nVlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vl_state(void){ps("[VLN] Vlp=");pi(st.n_vlp);ps(" Vle=");pi(st.n_vle);ps(" Vlv=");pi(st.n_vlv);ps(" Vlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Valerian Admin Demo ===\n\n");vl_init();
ps("Valerian planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vl_planning(t,c,699+(i*17),688+(i*14),668+(i*10),650+(i*6),2020+(i%5));}
ps("\nValerian execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vl_execution(t,c,688+(i*15),677+(i*12),659+(i*8),646+(i*5),2021+(i%4));}
ps("\nValerian evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vl_evaluation(t,c,680+(i*13),669+(i*10),653+(i*7),642+(i*4),2022+(i%3));}
ps("\nValerian accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vl_accessory(t,c,672+(i*11),663+(i*9),649+(i*6),639+(i*3),2023+(i%2));}
ps("\nValerian marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vl_market(t,c,666+(i*9),657+(i*7),644+(i*5),636+(i*3),2024);}
ps("\n");vl_report();vl_state();ps("\n=== Demo Complete ===\n");return 0;}
