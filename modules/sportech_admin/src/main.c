/* sportech_admin: Sports technology administration (v1.0)
 * Sports equipment, athletic wear, venue facilities, sports services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spt_t;
typedef struct{int n_se,n_aw,n_vf,n_ss,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spt_state_t;
static spt_t ses[N],aws[N-2],vfs[N-4],sss[N-6],mks[N-6]; static spt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spt_init(void){if(init)return -1;st.n_se=0;st.n_aw=0;st.n_vf=0;st.n_ss=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ses[i].active=0;for(int i=0;i<N-2;i++)aws[i].active=0;for(int i=0;i<N-4;i++)vfs[i].active=0;for(int i=0;i<N-6;i++)sss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPT] Sportech initialized\n");return 0;}
int spt_equip(int t,int c,int a,int b,int d,int e,int y){return add(ses,&st.n_se,&st.t_f1,N,t,c,a,b,d,e,y);}
int spt_wear(int t,int c,int a,int b,int d,int e,int y){return add(aws,&st.n_aw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spt_venue(int t,int c,int a,int b,int d,int e,int y){return add(vfs,&st.n_vf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spt_service(int t,int c,int a,int b,int d,int e,int y){return add(sss,&st.n_ss,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spt_report(void){ps("[SPT] Se: ");pi(st.n_se);ps(" PCS=");pi(st.t_f1);ps("\nAw: ");pi(st.n_aw);ps(" PCS=");pi(st.t_f2);ps("\nVf: ");pi(st.n_vf);ps(" PCS=");pi(st.t_f3);ps("\nSs: ");pi(st.n_ss);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spt_state(void){ps("[SPT] Se=");pi(st.n_se);ps(" Aw=");pi(st.n_aw);ps(" Vf=");pi(st.n_vf);ps(" Ss=");pi(st.n_ss);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sports Tech Admin Demo ===\n\n");spt_init();
ps("Sports equipment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spt_equip(t,c,231+(i*17),216+(i*14),196+(i*10),178+(i*6),2020+(i%5));}
ps("\nAthletic wear...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spt_wear(t,c,220+(i*15),206+(i*12),188+(i*8),175+(i*5),2021+(i%4));}
ps("\nVenue facilities...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spt_venue(t,c,212+(i*13),198+(i*10),182+(i*7),171+(i*4),2022+(i%3));}
ps("\nSports services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spt_service(t,c,204+(i*11),192+(i*9),178+(i*6),168+(i*3),2023+(i%2));}
ps("\nSports marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spt_market(t,c,198+(i*9),187+(i*7),174+(i*5),166+(i*3),2024);}
ps("\n");spt_report();spt_state();ps("\n=== Demo Complete ===\n");return 0;}
