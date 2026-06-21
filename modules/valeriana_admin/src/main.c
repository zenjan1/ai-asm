/* valeriana_admin: Valeriana management technology administration (v1.0)
 * Valeriana planning, valeriana execution, valeriana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vlr_t;
typedef struct{int n_vlp,n_vle,n_vlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vlr_state_t;
static vlr_t vlrps[N],vlrss[N-2],vlrvss[N-4],vlras[N-6],vlrmks[N-6]; static vlr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vlr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vlr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VLR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vlr_init(void){if(init)return -1;st.n_vlp=0;st.n_vle=0;st.n_vlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vlrps[i].active=0;for(int i=0;i<N-2;i++)vlrss[i].active=0;for(int i=0;i<N-4;i++)vlrvss[i].active=0;for(int i=0;i<N-6;i++)vlras[i].active=0;for(int i=0;i<N-6;i++)vlrmks[i].active=0;init=1;ps("[VLR] Valeriana initialized\n");return 0;}
int vlr_planning(int t,int c,int a,int b,int d,int e,int y){return add(vlrps,&st.n_vlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vlr_execution(int t,int c,int a,int b,int d,int e,int y){return add(vlrss,&st.n_vle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vlr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vlrvss,&st.n_vlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vlr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vlras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vlr_market(int t,int c,int a,int b,int d,int e,int y){return add(vlrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vlr_report(void){ps("[VLR] Vlp: ");pi(st.n_vlp);ps(" PCS=");pi(st.t_f1);ps("\nVle: ");pi(st.n_vle);ps(" PCS=");pi(st.t_f2);ps("\nVlv: ");pi(st.n_vlv);ps(" PCS=");pi(st.t_f3);ps("\nVlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vlr_state(void){ps("[VLR] Vlp=");pi(st.n_vlp);ps(" Vle=");pi(st.n_vle);ps(" Vlv=");pi(st.n_vlv);ps(" Vlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Valeriana Admin Demo ===\n\n");vlr_init();
ps("Valeriana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vlr_planning(t,c,875+(i*17),864+(i*14),844+(i*10),826+(i*6),2020+(i%5));}
ps("\nValeriana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vlr_execution(t,c,864+(i*15),853+(i*12),835+(i*8),822+(i*5),2021+(i%4));}
ps("\nValeriana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vlr_evaluation(t,c,856+(i*13),845+(i*10),829+(i*7),818+(i*4),2022+(i%3));}
ps("\nValeriana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vlr_accessory(t,c,848+(i*11),839+(i*9),825+(i*6),815+(i*3),2023+(i%2));}
ps("\nValeriana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vlr_market(t,c,842+(i*9),833+(i*7),820+(i*5),812+(i*3),2024);}
ps("\n");vlr_report();vlr_state();ps("\n=== Demo Complete ===\n");return 0;}
