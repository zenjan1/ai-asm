/* burnet_admin: Burnet management technology administration (v1.0)
 * Burnet planning, burnet execution, burnet evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bn_t;
typedef struct{int n_bnp,n_bne,n_bnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bn_state_t;
static bn_t bnps[N],bnss[N-2],bnvss[N-4],bnas[N-6],bnmks[N-6]; static bn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BNT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bn_init(void){if(init)return -1;st.n_bnp=0;st.n_bne=0;st.n_bnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bnps[i].active=0;for(int i=0;i<N-2;i++)bnss[i].active=0;for(int i=0;i<N-4;i++)bnvss[i].active=0;for(int i=0;i<N-6;i++)bnas[i].active=0;for(int i=0;i<N-6;i++)bnmks[i].active=0;init=1;ps("[BNT] Burnet initialized\n");return 0;}
int bn_planning(int t,int c,int a,int b,int d,int e,int y){return add(bnps,&st.n_bnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bn_execution(int t,int c,int a,int b,int d,int e,int y){return add(bnss,&st.n_bne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bnvss,&st.n_bnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bn_market(int t,int c,int a,int b,int d,int e,int y){return add(bnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bn_report(void){ps("[BNT] Bnp: ");pi(st.n_bnp);ps(" PCS=");pi(st.t_f1);ps("\nBne: ");pi(st.n_bne);ps(" PCS=");pi(st.t_f2);ps("\nBnv: ");pi(st.n_bnv);ps(" PCS=");pi(st.t_f3);ps("\nBnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bn_state(void){ps("[BNT] Bnp=");pi(st.n_bnp);ps(" Bne=");pi(st.n_bne);ps(" Bnv=");pi(st.n_bnv);ps(" Bnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Burnet Admin Demo ===\n\n");bn_init();
ps("Burnet planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bn_planning(t,c,696+(i*17),685+(i*14),665+(i*10),647+(i*6),2020+(i%5));}
ps("\nBurnet execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bn_execution(t,c,685+(i*15),674+(i*12),656+(i*8),643+(i*5),2021+(i%4));}
ps("\nBurnet evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bn_evaluation(t,c,677+(i*13),666+(i*10),650+(i*7),639+(i*4),2022+(i%3));}
ps("\nBurnet accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bn_accessory(t,c,669+(i*11),660+(i*9),646+(i*6),636+(i*3),2023+(i%2));}
ps("\nBurnet marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bn_market(t,c,663+(i*9),654+(i*7),641+(i*5),633+(i*3),2024);}
ps("\n");bn_report();bn_state();ps("\n=== Demo Complete ===\n");return 0;}
