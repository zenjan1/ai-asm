/* gamestch_admin: Games technology administration (v1.0)
 * Game engine, game AI, game art, game operations, esports
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gam_t;
typedef struct{int n_en,n_ai,n_ar,n_op,n_es,t_f1,t_f2,t_f3,t_f4,t_f5;} gam_state_t;
static gam_t ens[N],ais[N-2],ars[N-4],ops[N-6],ess[N-6]; static gam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gam_init(void){if(init)return -1;st.n_en=0;st.n_ai=0;st.n_ar=0;st.n_op=0;st.n_es=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ens[i].active=0;for(int i=0;i<N-2;i++)ais[i].active=0;for(int i=0;i<N-4;i++)ars[i].active=0;for(int i=0;i<N-6;i++)ops[i].active=0;for(int i=0;i<N-6;i++)ess[i].active=0;init=1;ps("[GAM] Gamestch initialized\n");return 0;}
int gam_engine(int t,int c,int a,int b,int d,int e,int y){return add(ens,&st.n_en,&st.t_f1,N,t,c,a,b,d,e,y);}
int gam_ai(int t,int c,int a,int b,int d,int e,int y){return add(ais,&st.n_ai,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gam_art(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gam_ops(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gam_esport(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gam_report(void){ps("[GAM] Engine: ");pi(st.n_en);ps(" FPS=");pi(st.t_f1);ps("\nAI: ");pi(st.n_ai);ps(" NPC=");pi(st.t_f2);ps("\nArt: ");pi(st.n_ar);ps(" 3D=");pi(st.t_f3);ps("\nOps: ");pi(st.n_op);ps(" DAU=");pi(st.t_f4);ps("\nEsport: ");pi(st.n_es);ps(" View=");pi(st.t_f5);ps("\n");}
void gam_state(void){ps("[GAM] En=");pi(st.n_en);ps(" Ai=");pi(st.n_ai);ps(" Ar=");pi(st.n_ar);ps(" Op=");pi(st.n_op);ps(" Es=");pi(st.n_es);ps("\n");}
int main(void){
ps("=== Games Tech Admin Demo ===\n\n");gam_init();
ps("Game engine...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gam_engine(t,c,155+(i*17),140+(i*14),120+(i*10),103+(i*6),2020+(i%5));}
ps("\nGame AI...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gam_ai(t,c,144+(i*15),130+(i*12),112+(i*8),99+(i*5),2021+(i%4));}
ps("\nGame art...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gam_art(t,c,136+(i*13),122+(i*10),106+(i*7),95+(i*4),2022+(i%3));}
ps("\nGame operations...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gam_ops(t,c,128+(i*11),116+(i*9),102+(i*6),92+(i*3),2023+(i%2));}
ps("\nEsports...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gam_esport(t,c,122+(i*9),111+(i*7),98+(i*5),90+(i*3),2024);}
ps("\n");gam_report();gam_state();ps("\n=== Demo Complete ===\n");return 0;}
