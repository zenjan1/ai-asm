/* achillea_admin: Achillea management technology administration (v1.0)
 * Achillea planning, achillea execution, achillea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ahl_t;
typedef struct{int n_ahp,n_ahe,n_ahv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ahl_state_t;
static ahl_t ahlps[N],ahlss[N-2],ahlvss[N-4],ahlas[N-6],ahlmks[N-6]; static ahl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ahl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ahl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AHL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ahl_init(void){if(init)return -1;st.n_ahp=0;st.n_ahe=0;st.n_ahv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ahlps[i].active=0;for(int i=0;i<N-2;i++)ahlss[i].active=0;for(int i=0;i<N-4;i++)ahlvss[i].active=0;for(int i=0;i<N-6;i++)ahlas[i].active=0;for(int i=0;i<N-6;i++)ahlmks[i].active=0;init=1;ps("[AHL] Achillea initialized\n");return 0;}
int ahl_planning(int t,int c,int a,int b,int d,int e,int y){return add(ahlps,&st.n_ahp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ahl_execution(int t,int c,int a,int b,int d,int e,int y){return add(ahlss,&st.n_ahe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ahl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ahlvss,&st.n_ahv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ahl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ahlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ahl_market(int t,int c,int a,int b,int d,int e,int y){return add(ahlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ahl_report(void){ps("[AHL] Ahp: ");pi(st.n_ahp);ps(" PCS=");pi(st.t_f1);ps("\nAhe: ");pi(st.n_ahe);ps(" PCS=");pi(st.t_f2);ps("\nAhv: ");pi(st.n_ahv);ps(" PCS=");pi(st.t_f3);ps("\nAhc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ahl_state(void){ps("[AHL] Ahp=");pi(st.n_ahp);ps(" Ahe=");pi(st.n_ahe);ps(" Ahv=");pi(st.n_ahv);ps(" Ahc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Achillea Admin Demo ===\n\n");ahl_init();
ps("Achillea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ahl_planning(t,c,884+(i*17),873+(i*14),853+(i*10),835+(i*6),2020+(i%5));}
ps("\nAchillea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ahl_execution(t,c,873+(i*15),862+(i*12),844+(i*8),831+(i*5),2021+(i%4));}
ps("\nAchillea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ahl_evaluation(t,c,865+(i*13),854+(i*10),838+(i*7),827+(i*4),2022+(i%3));}
ps("\nAchillea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ahl_accessory(t,c,857+(i*11),848+(i*9),834+(i*6),824+(i*3),2023+(i%2));}
ps("\nAchillea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ahl_market(t,c,851+(i*9),842+(i*7),829+(i*5),821+(i*3),2024);}
ps("\n");ahl_report();ahl_state();ps("\n=== Demo Complete ===\n");return 0;}
