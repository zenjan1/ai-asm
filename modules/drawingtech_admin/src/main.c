/* drawingtech_admin: Drawing technology administration (v1.0)
 * Bar drawing, tube drawing, wire drawing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drw_t;
typedef struct{int n_br,n_tb,n_wr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drw_state_t;
static drw_t brs[N],tbs[N-2],wrs[N-4],acs[N-6],mks[N-6]; static drw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(drw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drw_init(void){if(init)return -1;st.n_br=0;st.n_tb=0;st.n_wr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brs[i].active=0;for(int i=0;i<N-2;i++)tbs[i].active=0;for(int i=0;i<N-4;i++)wrs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DRW] Drawingtech initialized\n");return 0;}
int drw_bar(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f1,N,t,c,a,b,d,e,y);}
int drw_tube(int t,int c,int a,int b,int d,int e,int y){return add(tbs,&st.n_tb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drw_wire(int t,int c,int a,int b,int d,int e,int y){return add(wrs,&st.n_wr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drw_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drw_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drw_report(void){ps("[DRW] Br: ");pi(st.n_br);ps(" PCS=");pi(st.t_f1);ps("\nTb: ");pi(st.n_tb);ps(" PCS=");pi(st.t_f2);ps("\nWr: ");pi(st.n_wr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drw_state(void){ps("[DRW] Br=");pi(st.n_br);ps(" Tb=");pi(st.n_tb);ps(" Wr=");pi(st.n_wr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Drawing Tech Admin Demo ===\n\n");drw_init();
ps("Bar drawing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drw_bar(t,c,279+(i*17),264+(i*14),244+(i*10),226+(i*6),2020+(i%5));}
ps("\nTube drawing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drw_tube(t,c,268+(i*15),254+(i*12),236+(i*8),223+(i*5),2021+(i%4));}
ps("\nWire drawing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drw_wire(t,c,260+(i*13),246+(i*10),230+(i*7),219+(i*4),2022+(i%3));}
ps("\nDrawing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drw_accessory(t,c,252+(i*11),240+(i*9),226+(i*6),216+(i*3),2023+(i%2));}
ps("\nDrawing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drw_market(t,c,246+(i*9),235+(i*7),222+(i*5),214+(i*3),2024);}
ps("\n");drw_report();drw_state();ps("\n=== Demo Complete ===\n");return 0;}
