/* tradescantia_admin: Tradescantia management technology administration (v1.0)
 * Tradescantia planning, tradescantia execution, tradescantia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tl_t;
typedef struct{int n_tlp,n_tle,n_tlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tl_state_t;
static tl_t tlps[N],tlss[N-2],tlvss[N-4],tlas[N-6],tlmks[N-6]; static tl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TLI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tli_init(void){if(init)return -1;st.n_tlp=0;st.n_tle=0;st.n_tlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tlps[i].active=0;for(int i=0;i<N-2;i++)tlss[i].active=0;for(int i=0;i<N-4;i++)tlvss[i].active=0;for(int i=0;i<N-6;i++)tlas[i].active=0;for(int i=0;i<N-6;i++)tlmks[i].active=0;init=1;ps("[TLI] Tradescantia initialized\n");return 0;}
int tli_planning(int t,int c,int a,int b,int d,int e,int y){return add(tlps,&st.n_tlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tli_execution(int t,int c,int a,int b,int d,int e,int y){return add(tlss,&st.n_tle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tlvss,&st.n_tlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tli_market(int t,int c,int a,int b,int d,int e,int y){return add(tlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tli_report(void){ps("[TLI] Tlp: ");pi(st.n_tlp);ps(" PCS=");pi(st.t_f1);ps("\nTle: ");pi(st.n_tle);ps(" PCS=");pi(st.t_f2);ps("\nTlv: ");pi(st.n_tlv);ps(" PCS=");pi(st.t_f3);ps("\nTlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tli_state(void){ps("[TLI] Tlp=");pi(st.n_tlp);ps(" Tle=");pi(st.n_tle);ps(" Tlv=");pi(st.n_tlv);ps(" Tlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tradescantia Admin Demo ===\n\n");tli_init();
ps("Tradescantia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tli_planning(t,c,737+(i*17),726+(i*14),706+(i*10),688+(i*6),2020+(i%5));}
ps("\nTradescantia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tli_execution(t,c,726+(i*15),715+(i*12),697+(i*8),684+(i*5),2021+(i%4));}
ps("\nTradescantia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tli_evaluation(t,c,718+(i*13),707+(i*10),691+(i*7),680+(i*4),2022+(i%3));}
ps("\nTradescantia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tli_accessory(t,c,710+(i*11),701+(i*9),687+(i*6),677+(i*3),2023+(i%2));}
ps("\nTradescantia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tli_market(t,c,704+(i*9),695+(i*7),682+(i*5),674+(i*3),2024);}
ps("\n");tli_report();tli_state();ps("\n=== Demo Complete ===\n");return 0;}
