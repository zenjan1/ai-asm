/* amaryllis_admin: Amaryllis management technology administration (v1.0)
 * Amaryllis planning, amaryllis execution, amaryllis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ama_t;
typedef struct{int n_amap,n_amae,n_ama2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ama_state_t;
static ama_t amaps[N],amass[N-2],amavss[N-4],amaas[N-6],amamks[N-6]; static ama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ama_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ama_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ama_init(void){if(init)return -1;st.n_amap=0;st.n_amae=0;st.n_ama2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)amaps[i].active=0;for(int i=0;i<N-2;i++)amass[i].active=0;for(int i=0;i<N-4;i++)amavss[i].active=0;for(int i=0;i<N-6;i++)amaas[i].active=0;for(int i=0;i<N-6;i++)amamks[i].active=0;init=1;ps("[AMA] Amaryllis initialized\n");return 0;}
int ama_planning(int t,int c,int a,int b,int d,int e,int y){return add(amaps,&st.n_amap,&st.t_f1,N,t,c,a,b,d,e,y);}
int ama_execution(int t,int c,int a,int b,int d,int e,int y){return add(amass,&st.n_amae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ama_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(amavss,&st.n_ama2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ama_accessory(int t,int c,int a,int b,int d,int e,int y){return add(amaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ama_market(int t,int c,int a,int b,int d,int e,int y){return add(amamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ama_report(void){ps("[AMA] Amap: ");pi(st.n_amap);ps(" PCS=");pi(st.t_f1);ps("\nAmae: ");pi(st.n_amae);ps(" PCS=");pi(st.t_f2);ps("\nAmav: ");pi(st.n_ama2);ps(" PCS=");pi(st.t_f3);ps("\nAmac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ama_state(void){ps("[AMA] Amap=");pi(st.n_amap);ps(" Amae=");pi(st.n_amae);ps(" Amav=");pi(st.n_ama2);ps(" Amac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Amaryllis Admin Demo ===\n\n");ama_init();
ps("Amaryllis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ama_planning(t,c,972+(i*17),961+(i*14),941+(i*10),923+(i*6),2020+(i%5));}
ps("\nAmaryllis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ama_execution(t,c,961+(i*15),950+(i*12),932+(i*8),919+(i*5),2021+(i%4));}
ps("\nAmaryllis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ama_evaluation(t,c,953+(i*13),942+(i*10),926+(i*7),915+(i*4),2022+(i%3));}
ps("\nAmaryllis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ama_accessory(t,c,945+(i*11),936+(i*9),922+(i*6),912+(i*3),2023+(i%2));}
ps("\nAmaryllis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ama_market(t,c,939+(i*9),930+(i*7),917+(i*5),909+(i*3),2024);}
ps("\n");ama_report();ama_state();ps("\n=== Demo Complete ===\n");return 0;}
