/* lythrum_admin: Lythrum management technology administration (v1.0)
 * Lythrum planning, lythrum execution, lythrum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lyt_t;
typedef struct{int n_lytp,n_lyte,n_lyt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lyt_state_t;
static lyt_t lytps[N],lytss[N-2],lytvss[N-4],lytac[N-6],lytmks[N-6]; static lyt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lyt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lyt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LYT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lyt_init(void){if(init)return -1;st.n_lytp=0;st.n_lyte=0;st.n_lyt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lytps[i].active=0;for(int i=0;i<N-2;i++)lytss[i].active=0;for(int i=0;i<N-4;i++)lytvss[i].active=0;for(int i=0;i<N-6;i++)lytac[i].active=0;for(int i=0;i<N-6;i++)lytmks[i].active=0;init=1;ps("[LYT] Lythrum initialized\n");return 0;}
int lyt_planning(int t,int c,int a,int b,int d,int e,int y){return add(lytps,&st.n_lytp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lyt_execution(int t,int c,int a,int b,int d,int e,int y){return add(lytss,&st.n_lyte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lyt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lytvss,&st.n_lyt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lyt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lytac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lyt_market(int t,int c,int a,int b,int d,int e,int y){return add(lytmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lyt_report(void){ps("[LYT] Lytp: ");pi(st.n_lytp);ps(" PCS=");pi(st.t_f1);ps("\nLyte: ");pi(st.n_lyte);ps(" PCS=");pi(st.t_f2);ps("\nLytv: ");pi(st.n_lyt2);ps(" PCS=");pi(st.t_f3);ps("\nLytoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lyt_state(void){ps("[LYT] Lytp=");pi(st.n_lytp);ps(" Lyte=");pi(st.n_lyte);ps(" Lytv=");pi(st.n_lyt2);ps(" Lytoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lythrum Admin Demo ===\n\n");lyt_init();
ps("Lythrum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lyt_planning(t,c,939+(i*17),928+(i*14),908+(i*10),890+(i*6),2020+(i%5));}
ps("\nLythrum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lyt_execution(t,c,928+(i*15),917+(i*12),899+(i*8),886+(i*5),2021+(i%4));}
ps("\nLythrum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lyt_evaluation(t,c,920+(i*13),909+(i*10),893+(i*7),882+(i*4),2022+(i%3));}
ps("\nLythrum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyt_accessory(t,c,912+(i*11),903+(i*9),889+(i*6),879+(i*3),2023+(i%2));}
ps("\nLythrum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lyt_market(t,c,906+(i*9),897+(i*7),884+(i*5),876+(i*3),2024);}
ps("\n");lyt_report();lyt_state();ps("\n=== Demo Complete ===\n");return 0;}
