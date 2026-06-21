/* catalpa_admin: Catalpa management technology administration (v1.0)
 * Catalpa planning, catalpa execution, catalpa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cat_t;
typedef struct{int n_catp,n_cate,n_cat2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cat_state_t;
static cat_t catps[N],catss[N-2],catvss[N-4],cadas2[N-6],catmks[N-6]; static cat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cat_init(void){if(init)return -1;st.n_catp=0;st.n_cate=0;st.n_cat2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)catps[i].active=0;for(int i=0;i<N-2;i++)catss[i].active=0;for(int i=0;i<N-4;i++)catvss[i].active=0;for(int i=0;i<N-6;i++)cadas2[i].active=0;for(int i=0;i<N-6;i++)catmks[i].active=0;init=1;ps("[CAT] Catalpa initialized\n");return 0;}
int cat_planning(int t,int c,int a,int b,int d,int e,int y){return add(catps,&st.n_catp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cat_execution(int t,int c,int a,int b,int d,int e,int y){return add(catss,&st.n_cate,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cat_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(catvss,&st.n_cat2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cat_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cadas2,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cat_market(int t,int c,int a,int b,int d,int e,int y){return add(catmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cat_report(void){ps("[CAT] Catp: ");pi(st.n_catp);ps(" PCS=");pi(st.t_f1);ps("\nCate: ");pi(st.n_cate);ps(" PCS=");pi(st.t_f2);ps("\nCatv: ");pi(st.n_cat2);ps(" PCS=");pi(st.t_f3);ps("\nCatc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cat_state(void){ps("[CAT] Catp=");pi(st.n_catp);ps(" Cate=");pi(st.n_cate);ps(" Catv=");pi(st.n_cat2);ps(" Catc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Catalpa Admin Demo ===\n\n");cat_init();
ps("Catalpa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cat_planning(t,c,905+(i*17),894+(i*14),874+(i*10),856+(i*6),2020+(i%5));}
ps("\nCatalpa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cat_execution(t,c,894+(i*15),883+(i*12),865+(i*8),852+(i*5),2021+(i%4));}
ps("\nCatalpa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cat_evaluation(t,c,886+(i*13),875+(i*10),859+(i*7),848+(i*4),2022+(i%3));}
ps("\nCatalpa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cat_accessory(t,c,878+(i*11),869+(i*9),855+(i*6),845+(i*3),2023+(i%2));}
ps("\nCatalpa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cat_market(t,c,872+(i*9),863+(i*7),850+(i*5),842+(i*3),2024);}
ps("\n");cat_report();cat_state();ps("\n=== Demo Complete ===\n");return 0;}
