/* dentaria_admin: Dentaria management technology administration (v1.0)
 * Dentaria planning, dentaria execution, dentaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nta_t;
typedef struct{int n_ntap,n_ntae,n_nta2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nta_state_t;
static nta_t ntaps[N],ntass[N-2],ntavss[N-4],ntaas[N-6],ntamks[N-6]; static nta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nta_init(void){if(init)return -1;st.n_ntap=0;st.n_ntae=0;st.n_nta2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ntaps[i].active=0;for(int i=0;i<N-2;i++)ntass[i].active=0;for(int i=0;i<N-4;i++)ntavss[i].active=0;for(int i=0;i<N-6;i++)ntaas[i].active=0;for(int i=0;i<N-6;i++)ntamks[i].active=0;init=1;ps("[NTA] Dentaria initialized\n");return 0;}
int nta_planning(int t,int c,int a,int b,int d,int e,int y){return add(ntaps,&st.n_ntap,&st.t_f1,N,t,c,a,b,d,e,y);}
int nta_execution(int t,int c,int a,int b,int d,int e,int y){return add(ntass,&st.n_ntae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nta_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ntavss,&st.n_nta2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nta_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ntaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nta_market(int t,int c,int a,int b,int d,int e,int y){return add(ntamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nta_report(void){ps("[NTA] Denp: ");pi(st.n_ntap);ps(" PCS=");pi(st.t_f1);ps("\nDene: ");pi(st.n_ntae);ps(" PCS=");pi(st.t_f2);ps("\nDenv: ");pi(st.n_nta2);ps(" PCS=");pi(st.t_f3);ps("\nDenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nta_state(void){ps("[NTA] Denp=");pi(st.n_ntap);ps(" Dene=");pi(st.n_ntae);ps(" Denv=");pi(st.n_nta2);ps(" Denc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dentaria Admin Demo ===\n\n");nta_init();
ps("Dentaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nta_planning(t,c,1259+(i*17),1248+(i*14),1228+(i*10),1210+(i*6),2020+(i%5));}
ps("\nDentaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nta_execution(t,c,1248+(i*15),1237+(i*12),1219+(i*8),1206+(i*5),2021+(i%4));}
ps("\nDentaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nta_evaluation(t,c,1240+(i*13),1229+(i*10),1213+(i*7),1202+(i*4),2022+(i%3));}
ps("\nDentaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nta_accessory(t,c,1232+(i*11),1223+(i*9),1209+(i*6),1199+(i*3),2023+(i%2));}
ps("\nDentaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nta_market(t,c,1226+(i*9),1217+(i*7),1204+(i*5),1196+(i*3),2024);}
ps("\n");nta_report();nta_state();ps("\n=== Demo Complete ===\n");return 0;}
