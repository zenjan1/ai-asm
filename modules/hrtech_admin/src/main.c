/* hrtech_admin: HR technology administration (v1.0)
 * Recruitment, HR outsourcing, training development, performance management, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hrt_t;
typedef struct{int n_rc,n_ho,n_td,n_pm,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hrt_state_t;
static hrt_t rcs[N],hos[N-2],tds[N-4],pms[N-6],mks[N-6]; static hrt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hrt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hrt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hrt_init(void){if(init)return -1;st.n_rc=0;st.n_ho=0;st.n_td=0;st.n_pm=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rcs[i].active=0;for(int i=0;i<N-2;i++)hos[i].active=0;for(int i=0;i<N-4;i++)tds[i].active=0;for(int i=0;i<N-6;i++)pms[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HRT] Hrtech initialized\n");return 0;}
int hrt_recruit(int t,int c,int a,int b,int d,int e,int y){return add(rcs,&st.n_rc,&st.t_f1,N,t,c,a,b,d,e,y);}
int hrt_outsource(int t,int c,int a,int b,int d,int e,int y){return add(hos,&st.n_ho,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hrt_train(int t,int c,int a,int b,int d,int e,int y){return add(tds,&st.n_td,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hrt_performance(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hrt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hrt_report(void){ps("[HRT] Rc: ");pi(st.n_rc);ps(" PCS=");pi(st.t_f1);ps("\nHo: ");pi(st.n_ho);ps(" PCS=");pi(st.t_f2);ps("\nTd: ");pi(st.n_td);ps(" PCS=");pi(st.t_f3);ps("\nPm: ");pi(st.n_pm);ps(" Jobs=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hrt_state(void){ps("[HRT] Rc=");pi(st.n_rc);ps(" Ho=");pi(st.n_ho);ps(" Td=");pi(st.n_td);ps(" Pm=");pi(st.n_pm);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== HR Tech Admin Demo ===\n\n");hrt_init();
ps("Recruitment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hrt_recruit(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nHR outsourcing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hrt_outsource(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nTraining development...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hrt_train(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nPerformance management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrt_performance(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nHR marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrt_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");hrt_report();hrt_state();ps("\n=== Demo Complete ===\n");return 0;}
