/* jateorhiza_admin: Jateorhiza management technology administration (v1.0)
 * Jateorhiza planning, jateorhiza execution, jateorhiza evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jat_t;
typedef struct{int n_jatp,n_jate,n_jat2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jat_state_t;
static jat_t jatps[N],jatss[N-2],jatvss[N-4],jatas[N-6],jatmks[N-6]; static jat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jat_init(void){if(init)return -1;st.n_jatp=0;st.n_jate=0;st.n_jat2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jatps[i].active=0;for(int i=0;i<N-2;i++)jatss[i].active=0;for(int i=0;i<N-4;i++)jatvss[i].active=0;for(int i=0;i<N-6;i++)jatas[i].active=0;for(int i=0;i<N-6;i++)jatmks[i].active=0;init=1;ps("[JAT] Jateorhiza initialized\n");return 0;}
int jat_planning(int t,int c,int a,int b,int d,int e,int y){return add(jatps,&st.n_jatp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jat_execution(int t,int c,int a,int b,int d,int e,int y){return add(jatss,&st.n_jate,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jat_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jatvss,&st.n_jat2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jat_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jatas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jat_market(int t,int c,int a,int b,int d,int e,int y){return add(jatmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jat_report(void){ps("[JAT] Jatp: ");pi(st.n_jatp);ps(" PCS=");pi(st.t_f1);ps("\nJate: ");pi(st.n_jate);ps(" PCS=");pi(st.t_f2);ps("\nJatv: ");pi(st.n_jat2);ps(" PCS=");pi(st.t_f3);ps("\nJatc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jat_state(void){ps("[JAT] Jatp=");pi(st.n_jatp);ps(" Jate=");pi(st.n_jate);ps(" Jatv=");pi(st.n_jat2);ps(" Jatc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jateorhiza Admin Demo ===\n\n");jat_init();
ps("Jateorhiza planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jat_planning(t,c,1028+(i*17),1017+(i*14),997+(i*10),979+(i*6),2020+(i%5));}
ps("\nJateorhiza execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jat_execution(t,c,1017+(i*15),1006+(i*12),988+(i*8),975+(i*5),2021+(i%4));}
ps("\nJateorhiza evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jat_evaluation(t,c,1009+(i*13),998+(i*10),982+(i*7),971+(i*4),2022+(i%3));}
ps("\nJateorhiza accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jat_accessory(t,c,1001+(i*11),992+(i*9),978+(i*6),968+(i*3),2023+(i%2));}
ps("\nJateorhiza marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jat_market(t,c,995+(i*9),986+(i*7),973+(i*5),965+(i*3),2024);}
ps("\n");jat_report();jat_state();ps("\n=== Demo Complete ===\n");return 0;}
