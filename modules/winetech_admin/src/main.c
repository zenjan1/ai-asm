/* winetech_admin: Wine technology administration (v1.0)
 * Grape planting, winemaking, cellar management, wine tasting, wine marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wit_t;
typedef struct{int n_gp,n_wm,n_cl,n_ts,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wit_state_t;
static wit_t gps[N],wms[N-2],cls[N-4],tss[N-6],mks[N-6]; static wit_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wit_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wit_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WIT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wit_init(void){if(init)return -1;st.n_gp=0;st.n_wm=0;st.n_cl=0;st.n_ts=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gps[i].active=0;for(int i=0;i<N-2;i++)wms[i].active=0;for(int i=0;i<N-4;i++)cls[i].active=0;for(int i=0;i<N-6;i++)tss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WIT] Winetech initialized\n");return 0;}
int wit_grape(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wit_make(int t,int c,int a,int b,int d,int e,int y){return add(wms,&st.n_wm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wit_cellar(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wit_taste(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wit_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wit_report(void){ps("[WIT] Grape: ");pi(st.n_gp);ps(" ha=");pi(st.t_f1);ps("\nMake: ");pi(st.n_wm);ps(" L=");pi(st.t_f2);ps("\nCellar: ");pi(st.n_cl);ps(" Oak=");pi(st.t_f3);ps("\nTaste: ");pi(st.n_ts);ps(" Pts=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wit_state(void){ps("[WIT] Gp=");pi(st.n_gp);ps(" Wm=");pi(st.n_wm);ps(" Cl=");pi(st.n_cl);ps(" Ts=");pi(st.n_ts);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wine Tech Admin Demo ===\n\n");wit_init();
ps("Grape planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wit_grape(t,c,174+(i*17),159+(i*14),139+(i*10),122+(i*6),2020+(i%5));}
ps("\nWinemaking...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wit_make(t,c,163+(i*15),149+(i*12),131+(i*8),118+(i*5),2021+(i%4));}
ps("\nCellar management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wit_cellar(t,c,155+(i*13),141+(i*10),125+(i*7),114+(i*4),2022+(i%3));}
ps("\nWine tasting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wit_taste(t,c,147+(i*11),135+(i*9),121+(i*6),111+(i*3),2023+(i%2));}
ps("\nWine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wit_market(t,c,141+(i*9),130+(i*7),117+(i*5),109+(i*3),2024);}
ps("\n");wit_report();wit_state();ps("\n=== Demo Complete ===\n");return 0;}
