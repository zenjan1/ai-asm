/* agritech_admin: Agritech administration (v1.0)
 * Precision agriculture, smart breeding, agriculture platform, food traceability, agriculture drone
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} agt_t;
typedef struct{int n_pa,n_sb,n_ap,n_ft,n_ad,t_f1,t_f2,t_f3,t_f4,t_f5;} agt_state_t;
static agt_t pas[N],sbs[N-2],aps[N-4],fts[N-6],ads[N-6]; static agt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(agt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;agt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int agt_init(void){if(init)return -1;st.n_pa=0;st.n_sb=0;st.n_ap=0;st.n_ft=0;st.n_ad=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pas[i].active=0;for(int i=0;i<N-2;i++)sbs[i].active=0;for(int i=0;i<N-4;i++)aps[i].active=0;for(int i=0;i<N-6;i++)fts[i].active=0;for(int i=0;i<N-6;i++)ads[i].active=0;init=1;ps("[AGT] Agritech initialized\n");return 0;}
int agt_precision(int t,int c,int a,int b,int d,int e,int y){return add(pas,&st.n_pa,&st.t_f1,N,t,c,a,b,d,e,y);}
int agt_breeding(int t,int c,int a,int b,int d,int e,int y){return add(sbs,&st.n_sb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int agt_platform(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int agt_trace(int t,int c,int a,int b,int d,int e,int y){return add(fts,&st.n_ft,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int agt_drone(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void agt_report(void){ps("[AGT] Prec: ");pi(st.n_pa);ps(" IoT=");pi(st.t_f1);ps("\nBreed: ");pi(st.n_sb);ps(" AI=");pi(st.t_f2);ps("\nPlat: ");pi(st.n_ap);ps(" eCom=");pi(st.t_f3);ps("\nTrace: ");pi(st.n_ft);ps(" BC=");pi(st.t_f4);ps("\nDrone: ");pi(st.n_ad);ps(" UAV=");pi(st.t_f5);ps("\n");}
void agt_state(void){ps("[AGT] Pa=");pi(st.n_pa);ps(" Sb=");pi(st.n_sb);ps(" Ap=");pi(st.n_ap);ps(" Ft=");pi(st.n_ft);ps(" Ad=");pi(st.n_ad);ps("\n");}
int main(void){
ps("=== Agritech Admin Demo ===\n\n");agt_init();
ps("Precision agriculture...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;agt_precision(t,c,141+(i*17),126+(i*14),106+(i*10),89+(i*6),2020+(i%5));}
ps("\nSmart breeding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;agt_breeding(t,c,130+(i*15),116+(i*12),98+(i*8),85+(i*5),2021+(i%4));}
ps("\nAgriculture platform...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;agt_platform(t,c,122+(i*13),108+(i*10),92+(i*7),81+(i*4),2022+(i%3));}
ps("\nFood traceability...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agt_trace(t,c,114+(i*11),102+(i*9),88+(i*6),78+(i*3),2023+(i%2));}
ps("\nAgriculture drone...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agt_drone(t,c,108+(i*9),97+(i*7),84+(i*5),76+(i*3),2024);}
ps("\n");agt_report();agt_state();ps("\n=== Demo Complete ===\n");return 0;}
