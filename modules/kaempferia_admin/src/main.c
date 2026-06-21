/* kaempferia_admin: Kaempferia management technology administration (v1.0)
 * Kaempferia planning, kaempferia execution, kaempferia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kae_t;
typedef struct{int n_kaep,n_kaee,n_kae2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kae_state_t;
static kae_t kaeps[N],kaess[N-2],kaevss[N-4],kaeas[N-6],kaemks[N-6]; static kae_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kae_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kae_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KAE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kae_init(void){if(init)return -1;st.n_kaep=0;st.n_kaee=0;st.n_kae2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kaeps[i].active=0;for(int i=0;i<N-2;i++)kaess[i].active=0;for(int i=0;i<N-4;i++)kaevss[i].active=0;for(int i=0;i<N-6;i++)kaeas[i].active=0;for(int i=0;i<N-6;i++)kaemks[i].active=0;init=1;ps("[KAE] Kaempferia initialized\n");return 0;}
int kae_planning(int t,int c,int a,int b,int d,int e,int y){return add(kaeps,&st.n_kaep,&st.t_f1,N,t,c,a,b,d,e,y);}
int kae_execution(int t,int c,int a,int b,int d,int e,int y){return add(kaess,&st.n_kaee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kae_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kaevss,&st.n_kae2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kae_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kaeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kae_market(int t,int c,int a,int b,int d,int e,int y){return add(kaemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kae_report(void){ps("[KAE] Kaep: ");pi(st.n_kaep);ps(" PCS=");pi(st.t_f1);ps("\nKaee: ");pi(st.n_kaee);ps(" PCS=");pi(st.t_f2);ps("\nKaev: ");pi(st.n_kae2);ps(" PCS=");pi(st.t_f3);ps("\nKaec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kae_state(void){ps("[KAE] Kaep=");pi(st.n_kaep);ps(" Kaee=");pi(st.n_kaee);ps(" Kaev=");pi(st.n_kae2);ps(" Kaec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kaempferia Admin Demo ===\n\n");kae_init();
ps("Kaempferia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kae_planning(t,c,1035+(i*17),1024+(i*14),1004+(i*10),986+(i*6),2020+(i%5));}
ps("\nKaempferia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kae_execution(t,c,1024+(i*15),1013+(i*12),995+(i*8),982+(i*5),2021+(i%4));}
ps("\nKaempferia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kae_evaluation(t,c,1016+(i*13),1005+(i*10),989+(i*7),978+(i*4),2022+(i%3));}
ps("\nKaempferia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kae_accessory(t,c,1008+(i*11),999+(i*9),985+(i*6),975+(i*3),2023+(i%2));}
ps("\nKaempferia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kae_market(t,c,1002+(i*9),993+(i*7),980+(i*5),972+(i*3),2024);}
ps("\n");kae_report();kae_state();ps("\n=== Demo Complete ===\n");return 0;}
