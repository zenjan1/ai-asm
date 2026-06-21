/* vinca_admin: Vinca management technology administration (v1.0)
 * Vinca planning, vinca execution, vinca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vi_t;
typedef struct{int n_vip,n_vie,n_viv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vi_state_t;
static vi_t vips[N],vies[N-2],vivs[N-4],viacs[N-6],vmks[N-6]; static vi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vi_init(void){if(init)return -1;st.n_vip=0;st.n_vie=0;st.n_viv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vips[i].active=0;for(int i=0;i<N-2;i++)vies[i].active=0;for(int i=0;i<N-4;i++)vivs[i].active=0;for(int i=0;i<N-6;i++)viacs[i].active=0;for(int i=0;i<N-6;i++)vmks[i].active=0;init=1;ps("[VNC] Vinca initialized\n");return 0;}
int vi_planning(int t,int c,int a,int b,int d,int e,int y){return add(vips,&st.n_vip,&st.t_f1,N,t,c,a,b,d,e,y);}
int vi_execution(int t,int c,int a,int b,int d,int e,int y){return add(vies,&st.n_vie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vivs,&st.n_viv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(viacs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vi_market(int t,int c,int a,int b,int d,int e,int y){return add(vmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vi_report(void){ps("[VNC] Vip: ");pi(st.n_vip);ps(" PCS=");pi(st.t_f1);ps("\nVie: ");pi(st.n_vie);ps(" PCS=");pi(st.t_f2);ps("\nViv: ");pi(st.n_viv);ps(" PCS=");pi(st.t_f3);ps("\nVc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vi_state(void){ps("[VNC] Vip=");pi(st.n_vip);ps(" Vie=");pi(st.n_vie);ps(" Viv=");pi(st.n_viv);ps(" Vc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vinca Admin Demo ===\n\n");vi_init();
ps("Vinca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vi_planning(t,c,637+(i*17),626+(i*14),606+(i*10),588+(i*6),2020+(i%5));}
ps("\nVinca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vi_execution(t,c,626+(i*15),615+(i*12),597+(i*8),584+(i*5),2021+(i%4));}
ps("\nVinca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vi_evaluation(t,c,618+(i*13),607+(i*10),591+(i*7),580+(i*4),2022+(i%3));}
ps("\nVinca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vi_accessory(t,c,610+(i*11),601+(i*9),587+(i*6),577+(i*3),2023+(i%2));}
ps("\nVinca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vi_market(t,c,604+(i*9),595+(i*7),582+(i*5),574+(i*3),2024);}
ps("\n");vi_report();vi_state();ps("\n=== Demo Complete ===\n");return 0;}
