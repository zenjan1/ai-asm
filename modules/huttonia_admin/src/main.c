/* huttonia_admin: Huttonia management technology administration (v1.0)
 * Huttonia planning, huttonia execution, huttonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ht_t;
typedef struct{int n_htp,n_hte,n_htv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ht_state_t;
static ht_t htps[N],htss[N-2],htvss[N-4],htas[N-6],htmks[N-6]; static ht_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ht_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ht_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HTN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int htn_init(void){if(init)return -1;st.n_htp=0;st.n_hte=0;st.n_htv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)htps[i].active=0;for(int i=0;i<N-2;i++)htss[i].active=0;for(int i=0;i<N-4;i++)htvss[i].active=0;for(int i=0;i<N-6;i++)htas[i].active=0;for(int i=0;i<N-6;i++)htmks[i].active=0;init=1;ps("[HTN] Huttonia initialized\n");return 0;}
int htn_planning(int t,int c,int a,int b,int d,int e,int y){return add(htps,&st.n_htp,&st.t_f1,N,t,c,a,b,d,e,y);}
int htn_execution(int t,int c,int a,int b,int d,int e,int y){return add(htss,&st.n_hte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int htn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(htvss,&st.n_htv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int htn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(htas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int htn_market(int t,int c,int a,int b,int d,int e,int y){return add(htmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void htn_report(void){ps("[HTN] Htp: ");pi(st.n_htp);ps(" PCS=");pi(st.t_f1);ps("\nHte: ");pi(st.n_hte);ps(" PCS=");pi(st.t_f2);ps("\nHtv: ");pi(st.n_htv);ps(" PCS=");pi(st.t_f3);ps("\nHtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void htn_state(void){ps("[HTN] Htp=");pi(st.n_htp);ps(" Hte=");pi(st.n_hte);ps(" Htv=");pi(st.n_htv);ps(" Htc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Huttonia Admin Demo ===\n\n");htn_init();
ps("Huttonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;htn_planning(t,c,787+(i*17),776+(i*14),756+(i*10),738+(i*6),2020+(i%5));}
ps("\nHuttonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;htn_execution(t,c,776+(i*15),765+(i*12),747+(i*8),734+(i*5),2021+(i%4));}
ps("\nHuttonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;htn_evaluation(t,c,768+(i*13),757+(i*10),743+(i*7),732+(i*4),2022+(i%3));}
ps("\nHuttonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;htn_accessory(t,c,760+(i*11),751+(i*9),737+(i*6),727+(i*3),2023+(i%2));}
ps("\nHuttonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;htn_market(t,c,754+(i*9),745+(i*7),732+(i*5),724+(i*3),2024);}
ps("\n");htn_report();htn_state();ps("\n=== Demo Complete ===\n");return 0;}
