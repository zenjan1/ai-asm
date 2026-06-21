/* thalictrum_admin: Thalictrum management technology administration (v1.0)
 * Thalictrum planning, thalictrum execution, thalictrum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tha_t;
typedef struct{int n_thap,n_thae,n_tha2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tha_state_t;
static tha_t thaps[N],thass[N-2],thavss[N-4],thaac[N-6],thamks[N-6]; static tha_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tha_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tha_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tha_init(void){if(init)return -1;st.n_thap=0;st.n_thae=0;st.n_tha2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)thaps[i].active=0;for(int i=0;i<N-2;i++)thass[i].active=0;for(int i=0;i<N-4;i++)thavss[i].active=0;for(int i=0;i<N-6;i++)thaac[i].active=0;for(int i=0;i<N-6;i++)thamks[i].active=0;init=1;ps("[THA] Thalictrum initialized\n");return 0;}
int tha_planning(int t,int c,int a,int b,int d,int e,int y){return add(thaps,&st.n_thap,&st.t_f1,N,t,c,a,b,d,e,y);}
int tha_execution(int t,int c,int a,int b,int d,int e,int y){return add(thass,&st.n_thae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tha_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(thavss,&st.n_tha2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tha_accessory(int t,int c,int a,int b,int d,int e,int y){return add(thaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tha_market(int t,int c,int a,int b,int d,int e,int y){return add(thamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tha_report(void){ps("[THA] Thap: ");pi(st.n_thap);ps(" PCS=");pi(st.t_f1);ps("\nThae: ");pi(st.n_thae);ps(" PCS=");pi(st.t_f2);ps("\nThav: ");pi(st.n_tha2);ps(" PCS=");pi(st.t_f3);ps("\nThac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tha_state(void){ps("[THA] Thap=");pi(st.n_thap);ps(" Thae=");pi(st.n_thae);ps(" Thav=");pi(st.n_tha2);ps(" Thac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thalictrum Admin Demo ===\n\n");tha_init();
ps("Thalictrum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tha_planning(t,c,951+(i*17),940+(i*14),920+(i*10),902+(i*6),2020+(i%5));}
ps("\nThalictrum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tha_execution(t,c,940+(i*15),929+(i*12),911+(i*8),898+(i*5),2021+(i%4));}
ps("\nThalictrum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tha_evaluation(t,c,932+(i*13),921+(i*10),905+(i*7),894+(i*4),2022+(i%3));}
ps("\nThalictrum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tha_accessory(t,c,924+(i*11),915+(i*9),901+(i*6),891+(i*3),2023+(i%2));}
ps("\nThalictrum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tha_market(t,c,918+(i*9),909+(i*7),896+(i*5),888+(i*3),2024);}
ps("\n");tha_report();tha_state();ps("\n=== Demo Complete ===\n");return 0;}
