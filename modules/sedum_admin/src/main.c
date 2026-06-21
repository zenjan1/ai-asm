/* sedum_admin: Sedum management technology administration (v1.0)
 * Sedum planning, sedum execution, sedum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sn_t;
typedef struct{int n_sdp,n_sde,n_sdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sn_state_t;
static sn_t sdps[N],sdss[N-2],sdvss[N-4],sdas[N-6],sdmks[N-6]; static sn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SDM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sd_init(void){if(init)return -1;st.n_sdp=0;st.n_sde=0;st.n_sdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sdps[i].active=0;for(int i=0;i<N-2;i++)sdss[i].active=0;for(int i=0;i<N-4;i++)sdvss[i].active=0;for(int i=0;i<N-6;i++)sdas[i].active=0;for(int i=0;i<N-6;i++)sdmks[i].active=0;init=1;ps("[SDM] Sedum initialized\n");return 0;}
int sd_planning(int t,int c,int a,int b,int d,int e,int y){return add(sdps,&st.n_sdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sd_execution(int t,int c,int a,int b,int d,int e,int y){return add(sdss,&st.n_sde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sdvss,&st.n_sdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sd_market(int t,int c,int a,int b,int d,int e,int y){return add(sdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sd_report(void){ps("[SDM] Sdp: ");pi(st.n_sdp);ps(" PCS=");pi(st.t_f1);ps("\nSde: ");pi(st.n_sde);ps(" PCS=");pi(st.t_f2);ps("\nSdv: ");pi(st.n_sdv);ps(" PCS=");pi(st.t_f3);ps("\nSdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sd_state(void){ps("[SDM] Sdp=");pi(st.n_sdp);ps(" Sde=");pi(st.n_sde);ps(" Sdv=");pi(st.n_sdv);ps(" Sdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sedum Admin Demo ===\n\n");sd_init();
ps("Sedum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sd_planning(t,c,722+(i*17),711+(i*14),691+(i*10),673+(i*6),2020+(i%5));}
ps("\nSedum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sd_execution(t,c,711+(i*15),700+(i*12),682+(i*8),669+(i*5),2021+(i%4));}
ps("\nSedum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sd_evaluation(t,c,703+(i*13),692+(i*10),676+(i*7),665+(i*4),2022+(i%3));}
ps("\nSedum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sd_accessory(t,c,695+(i*11),686+(i*9),672+(i*6),662+(i*3),2023+(i%2));}
ps("\nSedum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sd_market(t,c,689+(i*9),680+(i*7),667+(i*5),659+(i*3),2024);}
ps("\n");sd_report();sd_state();ps("\n=== Demo Complete ===\n");return 0;}
