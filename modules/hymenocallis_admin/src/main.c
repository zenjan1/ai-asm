/* hymenocallis_admin: Hymenocallis management technology administration (v1.0)
 * Hymenocallis planning, hymenocallis execution, hymenocallis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hym_t;
typedef struct{int n_hymp,n_hyme,n_hym2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hym_state_t;
static hym_t hymps[N],hymss[N-2],hymvss[N-4],hymas[N-6],hymmks[N-6]; static hym_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hym_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hym_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hym_init(void){if(init)return -1;st.n_hymp=0;st.n_hyme=0;st.n_hym2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hymps[i].active=0;for(int i=0;i<N-2;i++)hymss[i].active=0;for(int i=0;i<N-4;i++)hymvss[i].active=0;for(int i=0;i<N-6;i++)hymas[i].active=0;for(int i=0;i<N-6;i++)hymmks[i].active=0;init=1;ps("[HYM] Hymenocallis initialized\n");return 0;}
int hym_planning(int t,int c,int a,int b,int d,int e,int y){return add(hymps,&st.n_hymp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hym_execution(int t,int c,int a,int b,int d,int e,int y){return add(hymss,&st.n_hyme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hym_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hymvss,&st.n_hym2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hym_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hymas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hym_market(int t,int c,int a,int b,int d,int e,int y){return add(hymmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hym_report(void){ps("[HYM] Hymp: ");pi(st.n_hymp);ps(" PCS=");pi(st.t_f1);ps("\nHyme: ");pi(st.n_hyme);ps(" PCS=");pi(st.t_f2);ps("\nHymv: ");pi(st.n_hym2);ps(" PCS=");pi(st.t_f3);ps("\nHymc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hym_state(void){ps("[HYM] Hymp=");pi(st.n_hymp);ps(" Hyme=");pi(st.n_hyme);ps(" Hymv=");pi(st.n_hym2);ps(" Hymc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hymenocallis Admin Demo ===\n\n");hym_init();
ps("Hymenocallis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hym_planning(t,c,1023+(i*17),1012+(i*14),992+(i*10),974+(i*6),2020+(i%5));}
ps("\nHymenocallis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hym_execution(t,c,1012+(i*15),1001+(i*12),983+(i*8),970+(i*5),2021+(i%4));}
ps("\nHymenocallis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hym_evaluation(t,c,1004+(i*13),993+(i*10),977+(i*7),966+(i*4),2022+(i%3));}
ps("\nHymenocallis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hym_accessory(t,c,996+(i*11),987+(i*9),973+(i*6),963+(i*3),2023+(i%2));}
ps("\nHymenocallis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hym_market(t,c,990+(i*9),981+(i*7),968+(i*5),960+(i*3),2024);}
ps("\n");hym_report();hym_state();ps("\n=== Demo Complete ===\n");return 0;}
