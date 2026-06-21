/* urtica_admin: Urtica management technology administration (v1.0)
 * Urtica planning, urtica execution, urtica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} urt_t;
typedef struct{int n_urtp,n_urte,n_urt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} urt_state_t;
static urt_t urtps[N],urtss[N-2],urtvss[N-4],urtas[N-6],urtmks[N-6]; static urt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(urt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;urt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[URT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int urt_init(void){if(init)return -1;st.n_urtp=0;st.n_urte=0;st.n_urt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)urtps[i].active=0;for(int i=0;i<N-2;i++)urtss[i].active=0;for(int i=0;i<N-4;i++)urtvss[i].active=0;for(int i=0;i<N-6;i++)urtas[i].active=0;for(int i=0;i<N-6;i++)urtmks[i].active=0;init=1;ps("[URT] Urtica initialized\n");return 0;}
int urt_planning(int t,int c,int a,int b,int d,int e,int y){return add(urtps,&st.n_urtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int urt_execution(int t,int c,int a,int b,int d,int e,int y){return add(urtss,&st.n_urte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int urt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(urtvss,&st.n_urt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int urt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(urtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int urt_market(int t,int c,int a,int b,int d,int e,int y){return add(urtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void urt_report(void){ps("[URT] Urtp: ");pi(st.n_urtp);ps(" PCS=");pi(st.t_f1);ps("\nUrte: ");pi(st.n_urte);ps(" PCS=");pi(st.t_f2);ps("\nUrtv: ");pi(st.n_urt2);ps(" PCS=");pi(st.t_f3);ps("\nUrtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void urt_state(void){ps("[URT] Urtp=");pi(st.n_urtp);ps(" Urte=");pi(st.n_urte);ps(" Urtv=");pi(st.n_urt2);ps(" Urtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Urtica Admin Demo ===\n\n");urt_init();
ps("Urtica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;urt_planning(t,c,955+(i*17),944+(i*14),924+(i*10),906+(i*6),2020+(i%5));}
ps("\nUrtica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;urt_execution(t,c,944+(i*15),933+(i*12),915+(i*8),902+(i*5),2021+(i%4));}
ps("\nUrtica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;urt_evaluation(t,c,936+(i*13),925+(i*10),909+(i*7),898+(i*4),2022+(i%3));}
ps("\nUrtica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;urt_accessory(t,c,928+(i*11),919+(i*9),905+(i*6),895+(i*3),2023+(i%2));}
ps("\nUrtica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;urt_market(t,c,922+(i*9),913+(i*7),900+(i*5),892+(i*3),2024);}
ps("\n");urt_report();urt_state();ps("\n=== Demo Complete ===\n");return 0;}
