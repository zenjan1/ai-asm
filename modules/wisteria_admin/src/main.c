/* wisteria_admin: Wisteria management technology administration (v1.0)
 * Wisteria planning, wisteria execution, wisteria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wis_t;
typedef struct{int n_wip,n_wie,n_wiv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wis_state_t;
static wis_t wips[N],wies[N-2],wivs[N-4],acs[N-6],mks[N-6]; static wis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wis_init(void){if(init)return -1;st.n_wip=0;st.n_wie=0;st.n_wiv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wips[i].active=0;for(int i=0;i<N-2;i++)wies[i].active=0;for(int i=0;i<N-4;i++)wivs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WIS] Wisteria initialized\n");return 0;}
int wis_planning(int t,int c,int a,int b,int d,int e,int y){return add(wips,&st.n_wip,&st.t_f1,N,t,c,a,b,d,e,y);}
int wis_execution(int t,int c,int a,int b,int d,int e,int y){return add(wies,&st.n_wie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wivs,&st.n_wiv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wis_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wis_report(void){ps("[WIS] Wip: ");pi(st.n_wip);ps(" PCS=");pi(st.t_f1);ps("\nWie: ");pi(st.n_wie);ps(" PCS=");pi(st.t_f2);ps("\nWiv: ");pi(st.n_wiv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wis_state(void){ps("[WIS] Wip=");pi(st.n_wip);ps(" Wie=");pi(st.n_wie);ps(" Wiv=");pi(st.n_wiv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wisteria Admin Demo ===\n\n");wis_init();
ps("Wisteria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wis_planning(t,c,584+(i*17),573+(i*14),553+(i*10),535+(i*6),2020+(i%5));}
ps("\nWisteria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wis_execution(t,c,573+(i*15),562+(i*12),544+(i*8),531+(i*5),2021+(i%4));}
ps("\nWisteria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wis_evaluation(t,c,565+(i*13),554+(i*10),538+(i*7),527+(i*4),2022+(i%3));}
ps("\nWisteria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wis_accessory(t,c,557+(i*11),548+(i*9),534+(i*6),524+(i*3),2023+(i%2));}
ps("\nWisteria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wis_market(t,c,551+(i*9),542+(i*7),529+(i*5),521+(i*3),2024);}
ps("\n");wis_report();wis_state();ps("\n=== Demo Complete ===\n");return 0;}
