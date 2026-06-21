/* penstemon_admin: Penstemon management technology administration (v1.0)
 * Penstemon planning, penstemon execution, penstemon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pen_t;
typedef struct{int n_penp,n_pene,n_pen2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pen_state_t;
static pen_t penps[N],penss[N-2],penvss[N-4],penas[N-6],penmks[N-6]; static pen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pen_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pen_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pen_init(void){if(init)return -1;st.n_penp=0;st.n_pene=0;st.n_pen2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)penps[i].active=0;for(int i=0;i<N-2;i++)penss[i].active=0;for(int i=0;i<N-4;i++)penvss[i].active=0;for(int i=0;i<N-6;i++)penas[i].active=0;for(int i=0;i<N-6;i++)penmks[i].active=0;init=1;ps("[PEN] Penstemon initialized\n");return 0;}
int pen_planning(int t,int c,int a,int b,int d,int e,int y){return add(penps,&st.n_penp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pen_execution(int t,int c,int a,int b,int d,int e,int y){return add(penss,&st.n_pene,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pen_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(penvss,&st.n_pen2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pen_accessory(int t,int c,int a,int b,int d,int e,int y){return add(penas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pen_market(int t,int c,int a,int b,int d,int e,int y){return add(penmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pen_report(void){ps("[PEN] Penp: ");pi(st.n_penp);ps(" PCS=");pi(st.t_f1);ps("\nPene: ");pi(st.n_pene);ps(" PCS=");pi(st.t_f2);ps("\nPeny: ");pi(st.n_pen2);ps(" PCS=");pi(st.t_f3);ps("\nPenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pen_state(void){ps("[PEN] Penp=");pi(st.n_penp);ps(" Pene=");pi(st.n_pene);ps(" Peny=");pi(st.n_pen2);ps(" Penc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Penstemon Admin Demo ===\n\n");pen_init();
ps("Penstemon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pen_planning(t,c,943+(i*17),932+(i*14),912+(i*10),894+(i*6),2020+(i%5));}
ps("\nPenstemon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pen_execution(t,c,932+(i*15),921+(i*12),903+(i*8),890+(i*5),2021+(i%4));}
ps("\nPenstemon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pen_evaluation(t,c,924+(i*13),913+(i*10),897+(i*7),886+(i*4),2022+(i%3));}
ps("\nPenstemon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pen_accessory(t,c,916+(i*11),907+(i*9),893+(i*6),883+(i*3),2023+(i%2));}
ps("\nPenstemon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pen_market(t,c,910+(i*9),901+(i*7),888+(i*5),880+(i*3),2024);}
ps("\n");pen_report();pen_state();ps("\n=== Demo Complete ===\n");return 0;}
