/* krameria_admin: Krameria management technology administration (v1.0)
 * Krameria planning, krameria execution, krameria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kra_t;
typedef struct{int n_krap,n_krae,n_kra2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kra_state_t;
static kra_t kras[N],krass[N-2],kravss[N-4],kraas[N-6],kramks[N-6]; static kra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kra_init(void){if(init)return -1;st.n_krap=0;st.n_krae=0;st.n_kra2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kras[i].active=0;for(int i=0;i<N-2;i++)krass[i].active=0;for(int i=0;i<N-4;i++)kravss[i].active=0;for(int i=0;i<N-6;i++)kraas[i].active=0;for(int i=0;i<N-6;i++)kramks[i].active=0;init=1;ps("[KRA] Krameria initialized\n");return 0;}
int kra_planning(int t,int c,int a,int b,int d,int e,int y){return add(kras,&st.n_krap,&st.t_f1,N,t,c,a,b,d,e,y);}
int kra_execution(int t,int c,int a,int b,int d,int e,int y){return add(krass,&st.n_krae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kra_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kravss,&st.n_kra2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kra_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kraas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kra_market(int t,int c,int a,int b,int d,int e,int y){return add(kramks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kra_report(void){ps("[KRA] Krap: ");pi(st.n_krap);ps(" PCS=");pi(st.t_f1);ps("\nKrae: ");pi(st.n_krae);ps(" PCS=");pi(st.t_f2);ps("\nKrav: ");pi(st.n_kra2);ps(" PCS=");pi(st.t_f3);ps("\nKrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kra_state(void){ps("[KRA] Krap=");pi(st.n_krap);ps(" Krae=");pi(st.n_krae);ps(" Krav=");pi(st.n_kra2);ps(" Krac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Krameria Admin Demo ===\n\n");kra_init();
ps("Krameria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kra_planning(t,c,1036+(i*17),1025+(i*14),1005+(i*10),987+(i*6),2020+(i%5));}
ps("\nKrameria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kra_execution(t,c,1025+(i*15),1014+(i*12),996+(i*8),983+(i*5),2021+(i%4));}
ps("\nKrameria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kra_evaluation(t,c,1017+(i*13),1006+(i*10),990+(i*7),979+(i*4),2022+(i%3));}
ps("\nKrameria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kra_accessory(t,c,1009+(i*11),1000+(i*9),986+(i*6),976+(i*3),2023+(i%2));}
ps("\nKrameria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kra_market(t,c,1003+(i*9),994+(i*7),981+(i*5),973+(i*3),2024);}
ps("\n");kra_report();kra_state();ps("\n=== Demo Complete ===\n");return 0;}
