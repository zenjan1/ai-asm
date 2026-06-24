/* monarda_admin: Monarda bee balm management (v1.0)
 * Monarda didyma (scarlet bee balm), M. fistulosa (wild bergamot) cultivation
 * Essential oil extraction, pollinator habitat, medicinal respiratory use
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mon_t;
typedef struct{int n_monc,n_mone,n_mon2,n_mp,n_ar,t_f1,t_f2,t_f3,t_f4,t_f5;} mon_state_t;
static mon_t moncs[N],mones[N-2],mon2s[N-4],mps[N-6],ars[N-6]; static mon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mon_init(void){if(init)return -1;st.n_monc=0;st.n_mone=0;st.n_mon2=0;st.n_mp=0;st.n_ar=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)moncs[i].active=0;for(int i=0;i<N-2;i++)mones[i].active=0;for(int i=0;i<N-4;i++)mon2s[i].active=0;for(int i=0;i<N-6;i++)mps[i].active=0;for(int i=0;i<N-6;i++)ars[i].active=0;init=1;ps("[MON] Monarda initialized\n");return 0;}
int mon_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(moncs,&st.n_monc,&st.t_f1,N,t,c,a,b,d,e,y);}
int mon_extraction(int t,int c,int a,int b,int d,int e,int y){return add(mones,&st.n_mone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mon_pollinator(int t,int c,int a,int b,int d,int e,int y){return add(mon2s,&st.n_mon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mon_medicinal(int t,int c,int a,int b,int d,int e,int y){return add(mps,&st.n_mp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mon_aromatic(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mon_report(void){ps("[MON] Cultivation: ");pi(st.n_monc);ps(" PCS=");pi(st.t_f1);ps("\nExtraction: ");pi(st.n_mone);ps(" PCS=");pi(st.t_f2);ps("\nPollinator: ");pi(st.n_mon2);ps(" PCS=");pi(st.t_f3);ps("\nMedicinal: ");pi(st.n_mp);ps(" PCS=");pi(st.t_f4);ps("\nAromatic: ");pi(st.n_ar);ps(" USD=");pi(st.t_f5);ps("\n");}
void mon_state(void){ps("[MON] Cul=");pi(st.n_monc);ps(" Ext=");pi(st.n_mone);ps(" Pol=");pi(st.n_mon2);ps(" Med=");pi(st.n_mp);ps(" Aro=");pi(st.n_ar);ps("\n");}
int main(void){
ps("=== Monarda Admin Demo ===\n\n");mon_init();
ps("Monarda cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mon_cultivation(t,c,937+(i*17),926+(i*14),906+(i*10),888+(i*6),2020+(i%5));}
ps("\nEssential oil extraction...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mon_extraction(t,c,926+(i*15),915+(i*12),897+(i*8),884+(i*5),2021+(i%4));}
ps("\nPollinator habitat...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mon_pollinator(t,c,918+(i*13),907+(i*10),891+(i*7),880+(i*4),2022+(i%3));}
ps("\nMedicinal uses...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_medicinal(t,c,910+(i*11),901+(i*9),887+(i*6),877+(i*3),2023+(i%2));}
ps("\nAromatic properties...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_aromatic(t,c,904+(i*9),895+(i*7),882+(i*5),874+(i*3),2024);}
ps("\n");mon_report();mon_state();ps("\n=== Demo Complete ===\n");return 0;}
