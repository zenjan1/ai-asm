/* hesperis_admin: Hesperis management technology administration (v1.0)
 * Hesperis planning, hesperis execution, hesperis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hpd_t;
typedef struct{int n_hdp,n_hde,n_hdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hpd_state_t;
static hpd_t hdps[N],hdess[N-2],hdvss[N-4],hdas[N-6],hdmks[N-6]; static hpd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hpd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hpd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HPD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hpd_init(void){if(init)return -1;st.n_hdp=0;st.n_hde=0;st.n_hdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hdps[i].active=0;for(int i=0;i<N-2;i++)hdess[i].active=0;for(int i=0;i<N-4;i++)hdvss[i].active=0;for(int i=0;i<N-6;i++)hdas[i].active=0;for(int i=0;i<N-6;i++)hdmks[i].active=0;init=1;ps("[HPD] Hesperis initialized\n");return 0;}
int hpd_planning(int t,int c,int a,int b,int d,int e,int y){return add(hdps,&st.n_hdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hpd_execution(int t,int c,int a,int b,int d,int e,int y){return add(hdess,&st.n_hde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hpd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hdvss,&st.n_hdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hpd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hpd_market(int t,int c,int a,int b,int d,int e,int y){return add(hdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hpd_report(void){ps("[HPD] Hdp: ");pi(st.n_hdp);ps(" PCS=");pi(st.t_f1);ps("\nHde: ");pi(st.n_hde);ps(" PCS=");pi(st.t_f2);ps("\nHdv: ");pi(st.n_hdv);ps(" PCS=");pi(st.t_f3);ps("\nHdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hpd_state(void){ps("[HPD] Hdp=");pi(st.n_hdp);ps(" Hde=");pi(st.n_hde);ps(" Hdv=");pi(st.n_hdv);ps(" Hdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hesperis Admin Demo ===\n\n");hpd_init();
ps("Hesperis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hpd_planning(t,c,828+(i*17),817+(i*14),797+(i*10),779+(i*6),2020+(i%5));}
ps("\nHesperis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hpd_execution(t,c,817+(i*15),806+(i*12),788+(i*8),775+(i*5),2021+(i%4));}
ps("\nHesperis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hpd_evaluation(t,c,809+(i*13),798+(i*10),782+(i*7),771+(i*4),2022+(i%3));}
ps("\nHesperis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hpd_accessory(t,c,801+(i*11),792+(i*9),778+(i*6),768+(i*3),2023+(i%2));}
ps("\nHesperis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hpd_market(t,c,795+(i*9),786+(i*7),773+(i*5),765+(i*3),2024);}
ps("\n");hpd_report();hpd_state();ps("\n=== Demo Complete ===\n");return 0;}
