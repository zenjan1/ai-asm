/* sportstech_admin: Sports technology administration (v1.0)
 * Smart training, event management, sports health, sports venues, sports marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spt_t;
typedef struct{int n_tr,n_ev,n_he,n_ve,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spt_state_t;
static spt_t trs[N],evs[N-2],hes[N-4],ves[N-6],mks[N-6]; static spt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spt_init(void){if(init)return -1;st.n_tr=0;st.n_ev=0;st.n_he=0;st.n_ve=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trs[i].active=0;for(int i=0;i<N-2;i++)evs[i].active=0;for(int i=0;i<N-4;i++)hes[i].active=0;for(int i=0;i<N-6;i++)ves[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPT] Sportstech initialized\n");return 0;}
int spt_training(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f1,N,t,c,a,b,d,e,y);}
int spt_event(int t,int c,int a,int b,int d,int e,int y){return add(evs,&st.n_ev,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spt_health(int t,int c,int a,int b,int d,int e,int y){return add(hes,&st.n_he,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spt_venue(int t,int c,int a,int b,int d,int e,int y){return add(ves,&st.n_ve,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spt_marketing(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spt_report(void){ps("[SPT] Train: ");pi(st.n_tr);ps(" AI=");pi(st.t_f1);ps("\nEvent: ");pi(st.n_ev);ps(" Live=");pi(st.t_f2);ps("\nHealth: ");pi(st.n_he);ps(" Mon=");pi(st.t_f3);ps("\nVenue: ");pi(st.n_ve);ps(" IoT=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" Fan=");pi(st.t_f5);ps("\n");}
void spt_state(void){ps("[SPT] Tr=");pi(st.n_tr);ps(" Ev=");pi(st.n_ev);ps(" He=");pi(st.n_he);ps(" Ve=");pi(st.n_ve);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sports Tech Admin Demo ===\n\n");spt_init();
ps("Smart training...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spt_training(t,c,146+(i*17),131+(i*14),111+(i*10),94+(i*6),2020+(i%5));}
ps("\nEvent management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spt_event(t,c,135+(i*15),121+(i*12),103+(i*8),90+(i*5),2021+(i%4));}
ps("\nSports health...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spt_health(t,c,127+(i*13),113+(i*10),97+(i*7),86+(i*4),2022+(i%3));}
ps("\nSports venues...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spt_venue(t,c,119+(i*11),107+(i*9),93+(i*6),83+(i*3),2023+(i%2));}
ps("\nSports marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spt_marketing(t,c,113+(i*9),102+(i*7),89+(i*5),81+(i*3),2024);}
ps("\n");spt_report();spt_state();ps("\n=== Demo Complete ===\n");return 0;}
