/* laburnum_admin: Laburnum management technology administration (v1.0)
 * Laburnum planning, laburnum execution, laburnum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lab_t;
typedef struct{int n_labp,n_labe,n_lab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lab_state_t;
static lab_t labps[N],labss[N-2],labvss[N-4],labas[N-6],labmks[N-6]; static lab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lab_init(void){if(init)return -1;st.n_labp=0;st.n_labe=0;st.n_lab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)labps[i].active=0;for(int i=0;i<N-2;i++)labss[i].active=0;for(int i=0;i<N-4;i++)labvss[i].active=0;for(int i=0;i<N-6;i++)labas[i].active=0;for(int i=0;i<N-6;i++)labmks[i].active=0;init=1;ps("[LAB] Laburnum initialized\n");return 0;}
int lab_planning(int t,int c,int a,int b,int d,int e,int y){return add(labps,&st.n_labp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lab_execution(int t,int c,int a,int b,int d,int e,int y){return add(labss,&st.n_labe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(labvss,&st.n_lab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(labas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lab_market(int t,int c,int a,int b,int d,int e,int y){return add(labmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lab_report(void){ps("[LAB] Labp: ");pi(st.n_labp);ps(" PCS=");pi(st.t_f1);ps("\nLabe: ");pi(st.n_labe);ps(" PCS=");pi(st.t_f2);ps("\nLabv: ");pi(st.n_lab2);ps(" PCS=");pi(st.t_f3);ps("\nLabc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lab_state(void){ps("[LAB] Labp=");pi(st.n_labp);ps(" Labe=");pi(st.n_labe);ps(" Labv=");pi(st.n_lab2);ps(" Labc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Laburnum Admin Demo ===\n\n");lab_init();
ps("Laburnum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lab_planning(t,c,934+(i*17),923+(i*14),903+(i*10),885+(i*6),2020+(i%5));}
ps("\nLaburnum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lab_execution(t,c,923+(i*15),912+(i*12),894+(i*8),881+(i*5),2021+(i%4));}
ps("\nLaburnum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lab_evaluation(t,c,915+(i*13),904+(i*10),888+(i*7),877+(i*4),2022+(i%3));}
ps("\nLaburnum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lab_accessory(t,c,907+(i*11),898+(i*9),884+(i*6),874+(i*3),2023+(i%2));}
ps("\nLaburnum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lab_market(t,c,901+(i*9),892+(i*7),879+(i*5),871+(i*3),2024);}
ps("\n");lab_report();lab_state();ps("\n=== Demo Complete ===\n");return 0;}
