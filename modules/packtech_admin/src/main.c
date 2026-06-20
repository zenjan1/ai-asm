/* packtech_admin: Packaging technology administration (v1.0)
 * Packaging materials, packaging containers, packaging design, packaging equipment, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pkt_t;
typedef struct{int n_pm,n_pc,n_pd,n_pe,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pkt_state_t;
static pkt_t pms[N],pcs[N-2],pds[N-4],pes[N-6],mks[N-6]; static pkt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pkt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pkt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PKT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pkt_init(void){if(init)return -1;st.n_pm=0;st.n_pc=0;st.n_pd=0;st.n_pe=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pms[i].active=0;for(int i=0;i<N-2;i++)pcs[i].active=0;for(int i=0;i<N-4;i++)pds[i].active=0;for(int i=0;i<N-6;i++)pes[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PKT] Packtech initialized\n");return 0;}
int pkt_material(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f1,N,t,c,a,b,d,e,y);}
int pkt_container(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pkt_design(int t,int c,int a,int b,int d,int e,int y){return add(pds,&st.n_pd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pkt_equipment(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pkt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pkt_report(void){ps("[PKT] Pm: ");pi(st.n_pm);ps(" kg=");pi(st.t_f1);ps("\nPc: ");pi(st.n_pc);ps(" PCS=");pi(st.t_f2);ps("\nPd: ");pi(st.n_pd);ps(" PCS=");pi(st.t_f3);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pkt_state(void){ps("[PKT] Pm=");pi(st.n_pm);ps(" Pc=");pi(st.n_pc);ps(" Pd=");pi(st.n_pd);ps(" Pe=");pi(st.n_pe);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Packaging Tech Admin Demo ===\n\n");pkt_init();
ps("Packaging materials...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pkt_material(t,c,228+(i*17),213+(i*14),193+(i*10),175+(i*6),2020+(i%5));}
ps("\nPackaging containers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pkt_container(t,c,217+(i*15),203+(i*12),185+(i*8),172+(i*5),2021+(i%4));}
ps("\nPackaging design...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pkt_design(t,c,209+(i*13),195+(i*10),179+(i*7),168+(i*4),2022+(i%3));}
ps("\nPackaging equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pkt_equipment(t,c,201+(i*11),189+(i*9),175+(i*6),165+(i*3),2023+(i%2));}
ps("\nPackaging marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pkt_market(t,c,195+(i*9),184+(i*7),171+(i*5),163+(i*3),2024);}
ps("\n");pkt_report();pkt_state();ps("\n=== Demo Complete ===\n");return 0;}
