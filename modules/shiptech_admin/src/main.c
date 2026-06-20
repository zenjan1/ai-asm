/* shiptech_admin: Ship technology administration (v1.0)
 * Shipbuilding, marine equipment, port equipment, marine services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sht_t;
typedef struct{int n_sb,n_me,n_pe,n_ms,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sht_state_t;
static sht_t sbs[N],mes[N-2],pes[N-4],mss[N-6],mks[N-6]; static sht_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sht_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sht_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SHT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sht_init(void){if(init)return -1;st.n_sb=0;st.n_me=0;st.n_pe=0;st.n_ms=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sbs[i].active=0;for(int i=0;i<N-2;i++)mes[i].active=0;for(int i=0;i<N-4;i++)pes[i].active=0;for(int i=0;i<N-6;i++)mss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SHT] Shiptech initialized\n");return 0;}
int sht_ship(int t,int c,int a,int b,int d,int e,int y){return add(sbs,&st.n_sb,&st.t_f1,N,t,c,a,b,d,e,y);}
int sht_marine(int t,int c,int a,int b,int d,int e,int y){return add(mes,&st.n_me,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sht_port(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sht_service(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sht_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sht_report(void){ps("[SHT] Ship: ");pi(st.n_sb);ps(" DWT=");pi(st.t_f1);ps("\nMarine: ");pi(st.n_me);ps(" PCS=");pi(st.t_f2);ps("\nPort: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f3);ps("\nServ: ");pi(st.n_ms);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sht_state(void){ps("[SHT] Sb=");pi(st.n_sb);ps(" Me=");pi(st.n_me);ps(" Pe=");pi(st.n_pe);ps(" Ms=");pi(st.n_ms);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ship Tech Admin Demo ===\n\n");sht_init();
ps("Shipbuilding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sht_ship(t,c,216+(i*17),201+(i*14),181+(i*10),163+(i*6),2020+(i%5));}
ps("\nMarine equipment...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sht_marine(t,c,205+(i*15),191+(i*12),173+(i*8),160+(i*5),2021+(i%4));}
ps("\nPort equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sht_port(t,c,197+(i*13),183+(i*10),167+(i*7),156+(i*4),2022+(i%3));}
ps("\nMarine services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sht_service(t,c,189+(i*11),177+(i*9),163+(i*6),153+(i*3),2023+(i%2));}
ps("\nShip marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sht_market(t,c,183+(i*9),172+(i*7),159+(i*5),151+(i*3),2024);}
ps("\n");sht_report();sht_state();ps("\n=== Demo Complete ===\n");return 0;}
