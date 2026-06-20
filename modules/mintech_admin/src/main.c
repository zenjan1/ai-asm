/* mintech_admin: Mining technology administration (v1.0)
 * Mineral exploration, mining, mineral processing, mine safety, mine environmental
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mit_t;
typedef struct{int n_pb,n_mn,n_mp,n_ms,n_me,t_f1,t_f2,t_f3,t_f4,t_f5;} mit_state_t;
static mit_t pbs[N],mns[N-2],mps[N-4],mss[N-6],mes[N-6]; static mit_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mit_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mit_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MIT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mit_init(void){if(init)return -1;st.n_pb=0;st.n_mn=0;st.n_mp=0;st.n_ms=0;st.n_me=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pbs[i].active=0;for(int i=0;i<N-2;i++)mns[i].active=0;for(int i=0;i<N-4;i++)mps[i].active=0;for(int i=0;i<N-6;i++)mss[i].active=0;for(int i=0;i<N-6;i++)mes[i].active=0;init=1;ps("[MIT] Mintech initialized\n");return 0;}
int mit_probe(int t,int c,int a,int b,int d,int e,int y){return add(pbs,&st.n_pb,&st.t_f1,N,t,c,a,b,d,e,y);}
int mit_mining(int t,int c,int a,int b,int d,int e,int y){return add(mns,&st.n_mn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mit_process(int t,int c,int a,int b,int d,int e,int y){return add(mps,&st.n_mp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mit_safety(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mit_env(int t,int c,int a,int b,int d,int e,int y){return add(mes,&st.n_me,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mit_report(void){ps("[MIT] Probe: ");pi(st.n_pb);ps(" Geo=");pi(st.t_f1);ps("\nMine: ");pi(st.n_mn);ps(" Ton=");pi(st.t_f2);ps("\nProc: ");pi(st.n_mp);ps(" Flot=");pi(st.t_f3);ps("\nSafe: ");pi(st.n_ms);ps(" Vent=");pi(st.t_f4);ps("\nEnv: ");pi(st.n_me);ps(" Rec=");pi(st.t_f5);ps("\n");}
void mit_state(void){ps("[MIT] Pb=");pi(st.n_pb);ps(" Mn=");pi(st.n_mn);ps(" Mp=");pi(st.n_mp);ps(" Ms=");pi(st.n_ms);ps(" Me=");pi(st.n_me);ps("\n");}
int main(void){
ps("=== Mining Tech Admin Demo ===\n\n");mit_init();
ps("Mineral exploration...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mit_probe(t,c,164+(i*17),149+(i*14),129+(i*10),112+(i*6),2020+(i%5));}
ps("\nMining...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mit_mining(t,c,153+(i*15),139+(i*12),121+(i*8),108+(i*5),2021+(i%4));}
ps("\nMineral processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mit_process(t,c,145+(i*13),131+(i*10),115+(i*7),104+(i*4),2022+(i%3));}
ps("\nMine safety...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mit_safety(t,c,137+(i*11),125+(i*9),111+(i*6),101+(i*3),2023+(i%2));}
ps("\nMine environmental...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mit_env(t,c,131+(i*9),120+(i*7),107+(i*5),99+(i*3),2024);}
ps("\n");mit_report();mit_state();ps("\n=== Demo Complete ===\n");return 0;}
