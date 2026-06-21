/* roystonea_admin: Roystonea management technology administration (v1.0)
 * Roystonea planning, roystonea execution, roystonea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} roo_t;
typedef struct{int n_roop,n_rooe,n_roo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} roo_state_t;
static roo_t roops[N],rooss[N-2],roovss[N-4],rooas[N-6],roomks[N-6]; static roo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(roo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;roo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ROO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int roo_init(void){if(init)return -1;st.n_roop=0;st.n_rooe=0;st.n_roo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)roops[i].active=0;for(int i=0;i<N-2;i++)rooss[i].active=0;for(int i=0;i<N-4;i++)roovss[i].active=0;for(int i=0;i<N-6;i++)rooas[i].active=0;for(int i=0;i<N-6;i++)roomks[i].active=0;init=1;ps("[ROO] Roystonea initialized\n");return 0;}
int roo_planning(int t,int c,int a,int b,int d,int e,int y){return add(roops,&st.n_roop,&st.t_f1,N,t,c,a,b,d,e,y);}
int roo_execution(int t,int c,int a,int b,int d,int e,int y){return add(rooss,&st.n_rooe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int roo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(roovss,&st.n_roo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int roo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rooas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int roo_market(int t,int c,int a,int b,int d,int e,int y){return add(roomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void roo_report(void){ps("[ROO] royp: ");pi(st.n_roop);ps(" PCS=");pi(st.t_f1);ps("\nroye: ");pi(st.n_rooe);ps(" PCS=");pi(st.t_f2);ps("\nroyv: ");pi(st.n_roo2);ps(" PCS=");pi(st.t_f3);ps("\nroyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void roo_state(void){ps("[ROO] royp=");pi(st.n_roop);ps(" roye=");pi(st.n_rooe);ps(" royv=");pi(st.n_roo2);ps(" royc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Roystonea Admin Demo ===\n\n");roo_init();
ps("Roystonea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;roo_planning(t,c,1095+(i*17),1084+(i*14),1064+(i*10),1046+(i*6),2020+(i%5));}
ps("\nRoystonea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;roo_execution(t,c,1084+(i*15),1073+(i*12),1055+(i*8),1042+(i*5),2021+(i%4));}
ps("\nRoystonea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;roo_evaluation(t,c,1076+(i*13),1065+(i*10),1049+(i*7),1038+(i*4),2022+(i%3));}
ps("\nRoystonea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;roo_accessory(t,c,1068+(i*11),1059+(i*9),1045+(i*6),1035+(i*3),2023+(i%2));}
ps("\nRoystonea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;roo_market(t,c,1062+(i*9),1053+(i*7),1040+(i*5),1032+(i*3),2024);}
ps("\n");roo_report();roo_state();ps("\n=== Demo Complete ===\n");return 0;}
