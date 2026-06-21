/* mentha_admin: Mentha management technology administration (v1.0)
 * Mentha planning, mentha execution, mentha evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} men_t;
typedef struct{int n_menp,n_mene,n_men2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} men_state_t;
static men_t menps[N],menss[N-2],menvss[N-4],menac[N-6],menmks[N-6]; static men_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(men_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;men_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int men_init(void){if(init)return -1;st.n_menp=0;st.n_mene=0;st.n_men2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)menps[i].active=0;for(int i=0;i<N-2;i++)menss[i].active=0;for(int i=0;i<N-4;i++)menvss[i].active=0;for(int i=0;i<N-6;i++)menac[i].active=0;for(int i=0;i<N-6;i++)menmks[i].active=0;init=1;ps("[MEN] Mentha initialized\n");return 0;}
int men_planning(int t,int c,int a,int b,int d,int e,int y){return add(menps,&st.n_menp,&st.t_f1,N,t,c,a,b,d,e,y);}
int men_execution(int t,int c,int a,int b,int d,int e,int y){return add(menss,&st.n_mene,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int men_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(menvss,&st.n_men2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int men_accessory(int t,int c,int a,int b,int d,int e,int y){return add(menac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int men_market(int t,int c,int a,int b,int d,int e,int y){return add(menmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void men_report(void){ps("[MEN] Menp: ");pi(st.n_menp);ps(" PCS=");pi(st.t_f1);ps("\nMene: ");pi(st.n_mene);ps(" PCS=");pi(st.t_f2);ps("\nMenv: ");pi(st.n_men2);ps(" PCS=");pi(st.t_f3);ps("\nMenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void men_state(void){ps("[MEN] Menp=");pi(st.n_menp);ps(" Mene=");pi(st.n_mene);ps(" Menv=");pi(st.n_men2);ps(" Menc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mentha Admin Demo ===\n\n");men_init();
ps("Mentha planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;men_planning(t,c,940+(i*17),929+(i*14),909+(i*10),891+(i*6),2020+(i%5));}
ps("\nMentha execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;men_execution(t,c,929+(i*15),918+(i*12),900+(i*8),887+(i*5),2021+(i%4));}
ps("\nMentha evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;men_evaluation(t,c,921+(i*13),910+(i*10),894+(i*7),883+(i*4),2022+(i%3));}
ps("\nMentha accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;men_accessory(t,c,913+(i*11),904+(i*9),890+(i*6),880+(i*3),2023+(i%2));}
ps("\nMentha marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;men_market(t,c,907+(i*9),898+(i*7),885+(i*5),877+(i*3),2024);}
ps("\n");men_report();men_state();ps("\n=== Demo Complete ===\n");return 0;}
