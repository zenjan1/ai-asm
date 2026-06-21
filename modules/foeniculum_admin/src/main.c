/* foeniculum_admin: Foeniculum management technology administration (v1.0)
 * Foeniculum planning, foeniculum execution, foeniculum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} foe_t;
typedef struct{int n_foep,n_foee,n_foe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} foe_state_t;
static foe_t foeps[N],foess[N-2],foevss[N-4],foeas[N-6],foemks[N-6]; static foe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(foe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;foe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FOE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int foe_init(void){if(init)return -1;st.n_foep=0;st.n_foee=0;st.n_foe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)foeps[i].active=0;for(int i=0;i<N-2;i++)foess[i].active=0;for(int i=0;i<N-4;i++)foevss[i].active=0;for(int i=0;i<N-6;i++)foeas[i].active=0;for(int i=0;i<N-6;i++)foemks[i].active=0;init=1;ps("[FOE] Foeniculum initialized\n");return 0;}
int foe_planning(int t,int c,int a,int b,int d,int e,int y){return add(foeps,&st.n_foep,&st.t_f1,N,t,c,a,b,d,e,y);}
int foe_execution(int t,int c,int a,int b,int d,int e,int y){return add(foess,&st.n_foee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int foe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(foevss,&st.n_foe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int foe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(foeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int foe_market(int t,int c,int a,int b,int d,int e,int y){return add(foemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void foe_report(void){ps("[FOE] Foep: ");pi(st.n_foep);ps(" PCS=");pi(st.t_f1);ps("\nFoee: ");pi(st.n_foee);ps(" PCS=");pi(st.t_f2);ps("\nFoey: ");pi(st.n_foe2);ps(" PCS=");pi(st.t_f3);ps("\nFoec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void foe_state(void){ps("[FOE] Foep=");pi(st.n_foep);ps(" Foee=");pi(st.n_foee);ps(" Foey=");pi(st.n_foe2);ps(" Foec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Foeniculum Admin Demo ===\n\n");foe_init();
ps("Foeniculum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;foe_planning(t,c,1008+(i*17),997+(i*14),977+(i*10),959+(i*6),2020+(i%5));}
ps("\nFoeniculum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;foe_execution(t,c,997+(i*15),986+(i*12),968+(i*8),955+(i*5),2021+(i%4));}
ps("\nFoeniculum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;foe_evaluation(t,c,989+(i*13),978+(i*10),962+(i*7),951+(i*4),2022+(i%3));}
ps("\nFoeniculum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;foe_accessory(t,c,981+(i*11),972+(i*9),958+(i*6),948+(i*3),2023+(i%2));}
ps("\nFoeniculum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;foe_market(t,c,975+(i*9),966+(i*7),953+(i*5),945+(i*3),2024);}
ps("\n");foe_report();foe_state();ps("\n=== Demo Complete ===\n");return 0;}
