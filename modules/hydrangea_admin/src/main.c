/* hydrangea_admin: Hydrangea management technology administration (v1.0)
 * Hydrangea planning, hydrangea execution, hydrangea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hg_t;
typedef struct{int n_hgp,n_hge,n_hgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hg_state_t;
static hg_t hgps[N],hgss[N-2],hgvss[N-4],hgas[N-6],hgmks[N-6]; static hg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HGA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hg_init(void){if(init)return -1;st.n_hgp=0;st.n_hge=0;st.n_hgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hgps[i].active=0;for(int i=0;i<N-2;i++)hgss[i].active=0;for(int i=0;i<N-4;i++)hgvss[i].active=0;for(int i=0;i<N-6;i++)hgas[i].active=0;for(int i=0;i<N-6;i++)hgmks[i].active=0;init=1;ps("[HGA] Hydrangea initialized\n");return 0;}
int hg_planning(int t,int c,int a,int b,int d,int e,int y){return add(hgps,&st.n_hgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hg_execution(int t,int c,int a,int b,int d,int e,int y){return add(hgss,&st.n_hge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hgvss,&st.n_hgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hg_market(int t,int c,int a,int b,int d,int e,int y){return add(hgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hg_report(void){ps("[HGA] Hgp: ");pi(st.n_hgp);ps(" PCS=");pi(st.t_f1);ps("\nHge: ");pi(st.n_hge);ps(" PCS=");pi(st.t_f2);ps("\nHgv: ");pi(st.n_hgv);ps(" PCS=");pi(st.t_f3);ps("\nHgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hg_state(void){ps("[HGA] Hgp=");pi(st.n_hgp);ps(" Hge=");pi(st.n_hge);ps(" Hgv=");pi(st.n_hgv);ps(" Hgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hydrangea Admin Demo ===\n\n");hg_init();
ps("Hydrangea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hg_planning(t,c,645+(i*17),634+(i*14),614+(i*10),596+(i*6),2020+(i%5));}
ps("\nHydrangea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hg_execution(t,c,634+(i*15),623+(i*12),605+(i*8),592+(i*5),2021+(i%4));}
ps("\nHydrangea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hg_evaluation(t,c,626+(i*13),615+(i*10),599+(i*7),588+(i*4),2022+(i%3));}
ps("\nHydrangea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hg_accessory(t,c,618+(i*11),609+(i*9),595+(i*6),585+(i*3),2023+(i%2));}
ps("\nHydrangea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hg_market(t,c,612+(i*9),603+(i*7),590+(i*5),582+(i*3),2024);}
ps("\n");hg_report();hg_state();ps("\n=== Demo Complete ===\n");return 0;}
