/* fouquieria_admin: Fouquieria management technology administration (v1.0)
 * Fouquieria planning, fouquieria execution, fouquieria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fou_t;
typedef struct{int n_foup,n_foue,n_fou2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fou_state_t;
static fou_t foups[N],fouss[N-2],fouvss[N-4],fouas[N-6],foumks[N-6]; static fou_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fou_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fou_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FOU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fou_init(void){if(init)return -1;st.n_foup=0;st.n_foue=0;st.n_fou2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)foups[i].active=0;for(int i=0;i<N-2;i++)fouss[i].active=0;for(int i=0;i<N-4;i++)fouvss[i].active=0;for(int i=0;i<N-6;i++)fouas[i].active=0;for(int i=0;i<N-6;i++)foumks[i].active=0;init=1;ps("[FOU] Fouquieria initialized\n");return 0;}
int fou_planning(int t,int c,int a,int b,int d,int e,int y){return add(foups,&st.n_foup,&st.t_f1,N,t,c,a,b,d,e,y);}
int fou_execution(int t,int c,int a,int b,int d,int e,int y){return add(fouss,&st.n_foue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fou_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fouvss,&st.n_fou2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fou_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fouas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fou_market(int t,int c,int a,int b,int d,int e,int y){return add(foumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fou_report(void){ps("[FOU] Fouup: ");pi(st.n_foup);ps(" PCS=");pi(st.t_f1);ps("\nFoue: ");pi(st.n_foue);ps(" PCS=");pi(st.t_f2);ps("\nFouv: ");pi(st.n_fou2);ps(" PCS=");pi(st.t_f3);ps("\nFouc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fou_state(void){ps("[FOU] Fouup=");pi(st.n_foup);ps(" Foue=");pi(st.n_foue);ps(" Fouv=");pi(st.n_fou2);ps(" Fouc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fouquieria Admin Demo ===\n\n");fou_init();
ps("Fouquieria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fou_planning(t,c,1004+(i*17),993+(i*14),973+(i*10),955+(i*6),2020+(i%5));}
ps("\nFouquieria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fou_execution(t,c,993+(i*15),982+(i*12),964+(i*8),951+(i*5),2021+(i%4));}
ps("\nFouquieria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fou_evaluation(t,c,985+(i*13),974+(i*10),958+(i*7),947+(i*4),2022+(i%3));}
ps("\nFouquieria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fou_accessory(t,c,977+(i*11),968+(i*9),954+(i*6),944+(i*3),2023+(i%2));}
ps("\nFouquieria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fou_market(t,c,971+(i*9),962+(i*7),949+(i*5),941+(i*3),2024);}
ps("\n");fou_report();fou_state();ps("\n=== Demo Complete ===\n");return 0;}
