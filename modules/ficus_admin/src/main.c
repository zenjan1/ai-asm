/* ficus_admin: Ficus management technology administration (v1.0)
 * Ficus planning, ficus execution, ficus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fic_t;
typedef struct{int n_ficp,n_fice,n_fic2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fic_state_t;
static fic_t ficps[N],ficss[N-2],ficvss[N-4],ficas[N-6],ficmks[N-6]; static fic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fic_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fic_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FIC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fic_init(void){if(init)return -1;st.n_ficp=0;st.n_fice=0;st.n_fic2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ficps[i].active=0;for(int i=0;i<N-2;i++)ficss[i].active=0;for(int i=0;i<N-4;i++)ficvss[i].active=0;for(int i=0;i<N-6;i++)ficas[i].active=0;for(int i=0;i<N-6;i++)ficmks[i].active=0;init=1;ps("[FIC] Ficus initialized\n");return 0;}
int fic_planning(int t,int c,int a,int b,int d,int e,int y){return add(ficps,&st.n_ficp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fic_execution(int t,int c,int a,int b,int d,int e,int y){return add(ficss,&st.n_fice,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fic_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ficvss,&st.n_fic2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fic_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ficas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fic_market(int t,int c,int a,int b,int d,int e,int y){return add(ficmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fic_report(void){ps("[FIC] Ficcip: ");pi(st.n_ficp);ps(" PCS=");pi(st.t_f1);ps("\nFice: ");pi(st.n_fice);ps(" PCS=");pi(st.t_f2);ps("\nFicv: ");pi(st.n_fic2);ps(" PCS=");pi(st.t_f3);ps("\nFicc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fic_state(void){ps("[FIC] Ficcip=");pi(st.n_ficp);ps(" Fice=");pi(st.n_fice);ps(" Ficsv=");pi(st.n_fic2);ps(" Ficc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ficus Admin Demo ===\n\n");fic_init();
ps("Ficus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fic_planning(t,c,1006+(i*17),995+(i*14),975+(i*10),957+(i*6),2020+(i%5));}
ps("\nFicus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fic_execution(t,c,995+(i*15),984+(i*12),966+(i*8),953+(i*5),2021+(i%4));}
ps("\nFicus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fic_evaluation(t,c,987+(i*13),976+(i*10),960+(i*7),949+(i*4),2022+(i%3));}
ps("\nFicus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fic_accessory(t,c,979+(i*11),970+(i*9),956+(i*6),946+(i*3),2023+(i%2));}
ps("\nFicus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fic_market(t,c,973+(i*9),964+(i*7),951+(i*5),943+(i*3),2024);}
ps("\n");fic_report();fic_state();ps("\n=== Demo Complete ===\n");return 0;}
