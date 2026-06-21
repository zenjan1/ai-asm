/* typha_admin: Typha management technology administration (v1.0)
 * Typha planning, typha execution, typha evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} typ_t;
typedef struct{int n_typ,n_type,n_typ2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} typ_state_t;
static typ_t typps[N],typss[N-2],typvss[N-4],typac[N-6],typmks[N-6]; static typ_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(typ_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;typ_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TYP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int typ_init(void){if(init)return -1;st.n_typ=0;st.n_type=0;st.n_typ2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)typps[i].active=0;for(int i=0;i<N-2;i++)typss[i].active=0;for(int i=0;i<N-4;i++)typvss[i].active=0;for(int i=0;i<N-6;i++)typac[i].active=0;for(int i=0;i<N-6;i++)typmks[i].active=0;init=1;ps("[TYP] Typha initialized\n");return 0;}
int typ_planning(int t,int c,int a,int b,int d,int e,int y){return add(typps,&st.n_typ,&st.t_f1,N,t,c,a,b,d,e,y);}
int typ_execution(int t,int c,int a,int b,int d,int e,int y){return add(typss,&st.n_type,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int typ_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(typvss,&st.n_typ2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int typ_accessory(int t,int c,int a,int b,int d,int e,int y){return add(typac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int typ_market(int t,int c,int a,int b,int d,int e,int y){return add(typmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void typ_report(void){ps("[TYP] Typp: ");pi(st.n_typ);ps(" PCS=");pi(st.t_f1);ps("\nType: ");pi(st.n_type);ps(" PCS=");pi(st.t_f2);ps("\nTypv: ");pi(st.n_typ2);ps(" PCS=");pi(st.t_f3);ps("\nTypc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void typ_state(void){ps("[TYP] Typp=");pi(st.n_typ);ps(" Type=");pi(st.n_type);ps(" Typv=");pi(st.n_typ2);ps(" Typc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Typha Admin Demo ===\n\n");typ_init();
ps("Typha planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;typ_planning(t,c,953+(i*17),942+(i*14),922+(i*10),904+(i*6),2020+(i%5));}
ps("\nTypha execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;typ_execution(t,c,942+(i*15),931+(i*12),913+(i*8),900+(i*5),2021+(i%4));}
ps("\nTypha evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;typ_evaluation(t,c,934+(i*13),923+(i*10),907+(i*7),896+(i*4),2022+(i%3));}
ps("\nTypha accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;typ_accessory(t,c,926+(i*11),917+(i*9),903+(i*6),893+(i*3),2023+(i%2));}
ps("\nTypha marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;typ_market(t,c,920+(i*9),911+(i*7),898+(i*5),890+(i*3),2024);}
ps("\n");typ_report();typ_state();ps("\n=== Demo Complete ===\n");return 0;}
