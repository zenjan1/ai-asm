/* asphodeline_admin: Asphodeline management technology administration (v1.0)
 * Asphodeline planning, asphodeline execution, asphodeline evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asph_t;
typedef struct{int n_asphp,n_asphe,n_asph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asph_state_t;
static asph_t asphps[N],asphss[N-2],asphvss[N-4],asphas[N-6],asphmks[N-6]; static asph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asph_init(void){if(init)return -1;st.n_asphp=0;st.n_asphe=0;st.n_asph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)asphps[i].active=0;for(int i=0;i<N-2;i++)asphss[i].active=0;for(int i=0;i<N-4;i++)asphvss[i].active=0;for(int i=0;i<N-6;i++)asphas[i].active=0;for(int i=0;i<N-6;i++)asphmks[i].active=0;init=1;ps("[ASP] Asphodeline initialized\n");return 0;}
int asph_planning(int t,int c,int a,int b,int d,int e,int y){return add(asphps,&st.n_asphp,&st.t_f1,N,t,c,a,b,d,e,y);}
int asph_execution(int t,int c,int a,int b,int d,int e,int y){return add(asphss,&st.n_asphe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(asphvss,&st.n_asph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(asphas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asph_market(int t,int c,int a,int b,int d,int e,int y){return add(asphmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asph_report(void){ps("[ASP] Asppp: ");pi(st.n_asphp);ps(" PCS=");pi(st.t_f1);ps("\nAspe: ");pi(st.n_asphe);ps(" PCS=");pi(st.t_f2);ps("\nAspv: ");pi(st.n_asph2);ps(" PCS=");pi(st.t_f3);ps("\nAspc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asph_state(void){ps("[ASP] Asppp=");pi(st.n_asphp);ps(" Aspe=");pi(st.n_asphe);ps(" Aspv=");pi(st.n_asph2);ps(" Aspc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Asphodeline Admin Demo ===\n\n");asph_init();
ps("Asphodeline planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asph_planning(t,c,1122+(i*17),1111+(i*14),1091+(i*10),1073+(i*6),2020+(i%5));}
ps("\nAsphodeline execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asph_execution(t,c,1111+(i*15),1100+(i*12),1082+(i*8),1069+(i*5),2021+(i%4));}
ps("\nAsphodeline evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asph_evaluation(t,c,1103+(i*13),1092+(i*10),1076+(i*7),1065+(i*4),2022+(i%3));}
ps("\nAsphodeline accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asph_accessory(t,c,1095+(i*11),1086+(i*9),1072+(i*6),1062+(i*3),2023+(i%2));}
ps("\nAsphodeline marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asph_market(t,c,1089+(i*9),1080+(i*7),1067+(i*5),1059+(i*3),2024);}
ps("\n");asph_report();asph_state();ps("\n=== Demo Complete ===\n");return 0;}
