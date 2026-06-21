/* paprika_admin: Paprika management technology administration (v1.0)
 * Paprika planning, paprika execution, paprika evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pp_t;
typedef struct{int n_ppp,n_ppe,n_ppv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pp_state_t;
static pp_t ppps[N],ppss[N-2],ppvss[N-4],ppas[N-6],ppmks[N-6]; static pp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PPK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pp_init(void){if(init)return -1;st.n_ppp=0;st.n_ppe=0;st.n_ppv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ppps[i].active=0;for(int i=0;i<N-2;i++)ppss[i].active=0;for(int i=0;i<N-4;i++)ppvss[i].active=0;for(int i=0;i<N-6;i++)ppas[i].active=0;for(int i=0;i<N-6;i++)ppmks[i].active=0;init=1;ps("[PPK] Paprika initialized\n");return 0;}
int pp_planning(int t,int c,int a,int b,int d,int e,int y){return add(ppps,&st.n_ppp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pp_execution(int t,int c,int a,int b,int d,int e,int y){return add(ppss,&st.n_ppe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ppvss,&st.n_ppv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ppas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pp_market(int t,int c,int a,int b,int d,int e,int y){return add(ppmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pp_report(void){ps("[PPK] Ppp: ");pi(st.n_ppp);ps(" PCS=");pi(st.t_f1);ps("\nPpe: ");pi(st.n_ppe);ps(" PCS=");pi(st.t_f2);ps("\nPpv: ");pi(st.n_ppv);ps(" PCS=");pi(st.t_f3);ps("\nPpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pp_state(void){ps("[PPK] Ppp=");pi(st.n_ppp);ps(" Ppe=");pi(st.n_ppe);ps(" Ppv=");pi(st.n_ppv);ps(" Ppc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Paprika Admin Demo ===\n\n");pp_init();
ps("Paprika planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pp_planning(t,c,673+(i*17),662+(i*14),642+(i*10),624+(i*6),2020+(i%5));}
ps("\nPaprika execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pp_execution(t,c,662+(i*15),651+(i*12),633+(i*8),620+(i*5),2021+(i%4));}
ps("\nPaprika evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pp_evaluation(t,c,654+(i*13),643+(i*10),627+(i*7),616+(i*4),2022+(i%3));}
ps("\nPaprika accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pp_accessory(t,c,646+(i*11),637+(i*9),623+(i*6),613+(i*3),2023+(i%2));}
ps("\nPaprika marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pp_market(t,c,640+(i*9),631+(i*7),618+(i*5),610+(i*3),2024);}
ps("\n");pp_report();pp_state();ps("\n=== Demo Complete ===\n");return 0;}
