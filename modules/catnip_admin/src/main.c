/* catnip_admin: Catnip management technology administration (v1.0)
 * Catnip planning, catnip execution, catnip evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cn_t;
typedef struct{int n_cnp,n_cne,n_cnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cn_state_t;
static cn_t cnps[N],cnss[N-2],cnvss[N-4],cnas[N-6],cnmks[N-6]; static cn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CNP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cn_init(void){if(init)return -1;st.n_cnp=0;st.n_cne=0;st.n_cnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cnps[i].active=0;for(int i=0;i<N-2;i++)cnss[i].active=0;for(int i=0;i<N-4;i++)cnvss[i].active=0;for(int i=0;i<N-6;i++)cnas[i].active=0;for(int i=0;i<N-6;i++)cnmks[i].active=0;init=1;ps("[CNP] Catnip initialized\n");return 0;}
int cn_planning(int t,int c,int a,int b,int d,int e,int y){return add(cnps,&st.n_cnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cn_execution(int t,int c,int a,int b,int d,int e,int y){return add(cnss,&st.n_cne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cnvss,&st.n_cnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cn_market(int t,int c,int a,int b,int d,int e,int y){return add(cnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cn_report(void){ps("[CNP] Cnp: ");pi(st.n_cnp);ps(" PCS=");pi(st.t_f1);ps("\nCne: ");pi(st.n_cne);ps(" PCS=");pi(st.t_f2);ps("\nCnv: ");pi(st.n_cnv);ps(" PCS=");pi(st.t_f3);ps("\nCnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cn_state(void){ps("[CNP] Cnp=");pi(st.n_cnp);ps(" Cne=");pi(st.n_cne);ps(" Cnv=");pi(st.n_cnv);ps(" Cnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Catnip Admin Demo ===\n\n");cn_init();
ps("Catnip planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cn_planning(t,c,665+(i*17),654+(i*14),634+(i*10),616+(i*6),2020+(i%5));}
ps("\nCatnip execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cn_execution(t,c,654+(i*15),643+(i*12),625+(i*8),612+(i*5),2021+(i%4));}
ps("\nCatnip evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cn_evaluation(t,c,646+(i*13),635+(i*10),619+(i*7),608+(i*4),2022+(i%3));}
ps("\nCatnip accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cn_accessory(t,c,638+(i*11),629+(i*9),615+(i*6),605+(i*3),2023+(i%2));}
ps("\nCatnip marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cn_market(t,c,632+(i*9),623+(i*7),610+(i*5),602+(i*3),2024);}
ps("\n");cn_report();cn_state();ps("\n=== Demo Complete ===\n");return 0;}
