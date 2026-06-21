/* gentian_admin: Gentian management technology administration (v1.0)
 * Gentian planning, gentian execution, gentian evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gtn_t;
typedef struct{int n_gtp,n_gte,n_gtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gtn_state_t;
static gtn_t gtps[N],gtss[N-2],gtvss[N-4],gtas[N-6],gtmks[N-6]; static gtn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gtn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gtn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GTN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gtn_init(void){if(init)return -1;st.n_gtp=0;st.n_gte=0;st.n_gtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gtps[i].active=0;for(int i=0;i<N-2;i++)gtss[i].active=0;for(int i=0;i<N-4;i++)gtvss[i].active=0;for(int i=0;i<N-6;i++)gtas[i].active=0;for(int i=0;i<N-6;i++)gtmks[i].active=0;init=1;ps("[GTN] Gentian initialized\n");return 0;}
int gtn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gtps,&st.n_gtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gtn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gtss,&st.n_gte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gtn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gtvss,&st.n_gtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gtn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gtn_market(int t,int c,int a,int b,int d,int e,int y){return add(gtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gtn_report(void){ps("[GTN] Gtp: ");pi(st.n_gtp);ps(" PCS=");pi(st.t_f1);ps("\nGte: ");pi(st.n_gte);ps(" PCS=");pi(st.t_f2);ps("\nGtv: ");pi(st.n_gtv);ps(" PCS=");pi(st.t_f3);ps("\nGtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gtn_state(void){ps("[GTN] Gtp=");pi(st.n_gtp);ps(" Gte=");pi(st.n_gte);ps(" Gtv=");pi(st.n_gtv);ps(" Gtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gentian Admin Demo ===\n\n");gtn_init();
ps("Gentian planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gtn_planning(t,c,823+(i*17),812+(i*14),792+(i*10),774+(i*6),2020+(i%5));}
ps("\nGentian execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gtn_execution(t,c,812+(i*15),801+(i*12),783+(i*8),770+(i*5),2021+(i%4));}
ps("\nGentian evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gtn_evaluation(t,c,804+(i*13),793+(i*10),779+(i*7),768+(i*4),2022+(i%3));}
ps("\nGentian accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gtn_accessory(t,c,796+(i*11),787+(i*9),773+(i*6),763+(i*3),2023+(i%2));}
ps("\nGentian marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gtn_market(t,c,790+(i*9),781+(i*7),768+(i*5),760+(i*3),2024);}
ps("\n");gtn_report();gtn_state();ps("\n=== Demo Complete ===\n");return 0;}
