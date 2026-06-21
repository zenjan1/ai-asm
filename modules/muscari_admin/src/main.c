/* muscari_admin: Muscari management technology administration (v1.0)
 * Muscari planning, muscari execution, muscari evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mus_t;
typedef struct{int n_musp,n_muse,n_mus2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mus_state_t;
static mus_t musps[N],muss[N-2],musvss[N-4],musac[N-6],musmks[N-6]; static mus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mus_init(void){if(init)return -1;st.n_musp=0;st.n_muse=0;st.n_mus2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)musps[i].active=0;for(int i=0;i<N-2;i++)muss[i].active=0;for(int i=0;i<N-4;i++)musvss[i].active=0;for(int i=0;i<N-6;i++)musac[i].active=0;for(int i=0;i<N-6;i++)musmks[i].active=0;init=1;ps("[MUS] Muscari initialized\n");return 0;}
int mus_planning(int t,int c,int a,int b,int d,int e,int y){return add(musps,&st.n_musp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mus_execution(int t,int c,int a,int b,int d,int e,int y){return add(muss,&st.n_muse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mus_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(musvss,&st.n_mus2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mus_accessory(int t,int c,int a,int b,int d,int e,int y){return add(musac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mus_market(int t,int c,int a,int b,int d,int e,int y){return add(musmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mus_report(void){ps("[MUS] Musp: ");pi(st.n_musp);ps(" PCS=");pi(st.t_f1);ps("\nMuse: ");pi(st.n_muse);ps(" PCS=");pi(st.t_f2);ps("\nMusv: ");pi(st.n_mus2);ps(" PCS=");pi(st.t_f3);ps("\nMusc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mus_state(void){ps("[MUS] Musp=");pi(st.n_musp);ps(" Muse=");pi(st.n_muse);ps(" Musv=");pi(st.n_mus2);ps(" Musc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Muscari Admin Demo ===\n\n");mus_init();
ps("Muscari planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mus_planning(t,c,942+(i*17),931+(i*14),911+(i*10),893+(i*6),2020+(i%5));}
ps("\nMuscari execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mus_execution(t,c,931+(i*15),920+(i*12),902+(i*8),889+(i*5),2021+(i%4));}
ps("\nMuscari evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mus_evaluation(t,c,923+(i*13),912+(i*10),896+(i*7),885+(i*4),2022+(i%3));}
ps("\nMuscari accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_accessory(t,c,915+(i*11),906+(i*9),892+(i*6),882+(i*3),2023+(i%2));}
ps("\nMuscari marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_market(t,c,909+(i*9),900+(i*7),887+(i*5),879+(i*3),2024);}
ps("\n");mus_report();mus_state();ps("\n=== Demo Complete ===\n");return 0;}
