/* watercress_admin: Watercress management technology administration (v1.0)
 * Watercress planning, watercress execution, watercress evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wa_t;
typedef struct{int n_wtp,n_wte,n_wtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wa_state_t;
static wa_t wtps[N],wtss[N-2],wtvss[N-4],wtas[N-6],wtmks[N-6]; static wa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WTC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wt_init(void){if(init)return -1;st.n_wtp=0;st.n_wte=0;st.n_wtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wtps[i].active=0;for(int i=0;i<N-2;i++)wtss[i].active=0;for(int i=0;i<N-4;i++)wtvss[i].active=0;for(int i=0;i<N-6;i++)wtas[i].active=0;for(int i=0;i<N-6;i++)wtmks[i].active=0;init=1;ps("[WTC] Watercress initialized\n");return 0;}
int wt_planning(int t,int c,int a,int b,int d,int e,int y){return add(wtps,&st.n_wtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wt_execution(int t,int c,int a,int b,int d,int e,int y){return add(wtss,&st.n_wte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wtvss,&st.n_wtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wt_market(int t,int c,int a,int b,int d,int e,int y){return add(wtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wt_report(void){ps("[WTC] Wtp: ");pi(st.n_wtp);ps(" PCS=");pi(st.t_f1);ps("\nWte: ");pi(st.n_wte);ps(" PCS=");pi(st.t_f2);ps("\nWtv: ");pi(st.n_wtv);ps(" PCS=");pi(st.t_f3);ps("\nWtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wt_state(void){ps("[WTC] Wtp=");pi(st.n_wtp);ps(" Wte=");pi(st.n_wte);ps(" Wtv=");pi(st.n_wtv);ps(" Wtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Watercress Admin Demo ===\n\n");wt_init();
ps("Watercress planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wt_planning(t,c,712+(i*17),701+(i*14),681+(i*10),663+(i*6),2020+(i%5));}
ps("\nWatercress execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wt_execution(t,c,701+(i*15),690+(i*12),672+(i*8),659+(i*5),2021+(i%4));}
ps("\nWatercress evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wt_evaluation(t,c,693+(i*13),682+(i*10),666+(i*7),655+(i*4),2022+(i%3));}
ps("\nWatercress accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wt_accessory(t,c,685+(i*11),676+(i*9),662+(i*6),652+(i*3),2023+(i%2));}
ps("\nWatercress marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wt_market(t,c,679+(i*9),670+(i*7),657+(i*5),649+(i*3),2024);}
ps("\n");wt_report();wt_state();ps("\n=== Demo Complete ===\n");return 0;}
