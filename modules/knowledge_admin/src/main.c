/* knowledge_admin: Knowledge management technology administration (v1.0)
 * Knowledge acquisition, knowledge sharing, knowledge application, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} knx_t;
typedef struct{int n_ka,n_ks,n_kp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} knx_state_t;
static knx_t kas[N],kss[N-2],kps[N-4],acs[N-6],mks[N-6]; static knx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(knx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;knx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KNX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int knx_init(void){if(init)return -1;st.n_ka=0;st.n_ks=0;st.n_kp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kas[i].active=0;for(int i=0;i<N-2;i++)kss[i].active=0;for(int i=0;i<N-4;i++)kps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[KNX] Knowledge initialized\n");return 0;}
int knx_acquisition(int t,int c,int a,int b,int d,int e,int y){return add(kas,&st.n_ka,&st.t_f1,N,t,c,a,b,d,e,y);}
int knx_sharing(int t,int c,int a,int b,int d,int e,int y){return add(kss,&st.n_ks,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int knx_application(int t,int c,int a,int b,int d,int e,int y){return add(kps,&st.n_kp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int knx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int knx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void knx_report(void){ps("[KNX] Ka: ");pi(st.n_ka);ps(" PCS=");pi(st.t_f1);ps("\nKs: ");pi(st.n_ks);ps(" PCS=");pi(st.t_f2);ps("\nKp: ");pi(st.n_kp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void knx_state(void){ps("[KNX] Ka=");pi(st.n_ka);ps(" Ks=");pi(st.n_ks);ps(" Kp=");pi(st.n_kp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Knowledge Admin Demo ===\n\n");knx_init();
ps("Knowledge acquisition...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;knx_acquisition(t,c,454+(i*17),443+(i*14),423+(i*10),405+(i*6),2020+(i%5));}
ps("\nKnowledge sharing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;knx_sharing(t,c,443+(i*15),432+(i*12),414+(i*8),401+(i*5),2021+(i%4));}
ps("\nKnowledge application...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;knx_application(t,c,435+(i*13),424+(i*10),408+(i*7),397+(i*4),2022+(i%3));}
ps("\nKnowledge accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;knx_accessory(t,c,427+(i*11),418+(i*9),404+(i*6),394+(i*3),2023+(i%2));}
ps("\nKnowledge marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;knx_market(t,c,421+(i*9),412+(i*7),399+(i*5),391+(i*3),2024);}
ps("\n");knx_report();knx_state();ps("\n=== Demo Complete ===\n");return 0;}
