/* eustoma_admin: Eustoma management technology administration (v1.0)
 * Eustoma planning, eustoma execution, eustoma evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eus_t;
typedef struct{int n_eusp,n_euse,n_eus2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eus_state_t;
static eus_t eusps[N],eusss[N-2],eusvss[N-4],eusas[N-6],eusmks[N-6]; static eus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eus_init(void){if(init)return -1;st.n_eusp=0;st.n_euse=0;st.n_eus2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eusps[i].active=0;for(int i=0;i<N-2;i++)eusss[i].active=0;for(int i=0;i<N-4;i++)eusvss[i].active=0;for(int i=0;i<N-6;i++)eusas[i].active=0;for(int i=0;i<N-6;i++)eusmks[i].active=0;init=1;ps("[EUS] Eustoma initialized\n");return 0;}
int eus_planning(int t,int c,int a,int b,int d,int e,int y){return add(eusps,&st.n_eusp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eus_execution(int t,int c,int a,int b,int d,int e,int y){return add(eusss,&st.n_euse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eus_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eusvss,&st.n_eus2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eus_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eusas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eus_market(int t,int c,int a,int b,int d,int e,int y){return add(eusmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eus_report(void){ps("[EUS] Euspp: ");pi(st.n_eusp);ps(" PCS=");pi(st.t_f1);ps("\nEuse: ");pi(st.n_euse);ps(" PCS=");pi(st.t_f2);ps("\nEusv: ");pi(st.n_eus2);ps(" PCS=");pi(st.t_f3);ps("\nEusc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eus_state(void){ps("[EUS] Euspp=");pi(st.n_eusp);ps(" Euse=");pi(st.n_euse);ps(" Eusv=");pi(st.n_eus2);ps(" Eusc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eustoma Admin Demo ===\n\n");eus_init();
ps("Eustoma planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eus_planning(t,c,1141+(i*17),1130+(i*14),1110+(i*10),1092+(i*6),2020+(i%5));}
ps("\nEustoma execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eus_execution(t,c,1130+(i*15),1119+(i*12),1101+(i*8),1088+(i*5),2021+(i%4));}
ps("\nEustoma evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eus_evaluation(t,c,1122+(i*13),1111+(i*10),1095+(i*7),1084+(i*4),2022+(i%3));}
ps("\nEustoma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eus_accessory(t,c,1114+(i*11),1105+(i*9),1091+(i*6),1081+(i*3),2023+(i%2));}
ps("\nEustoma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eus_market(t,c,1108+(i*9),1099+(i*7),1086+(i*5),1078+(i*3),2024);}
ps("\n");eus_report();eus_state();ps("\n=== Demo Complete ===\n");return 0;}
