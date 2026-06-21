/* zamia_admin: Zamia management technology administration (v1.0)
 * Zamia planning, zamia execution, zamia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zam_t;
typedef struct{int n_zamp,n_zame,n_zam2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zam_state_t;
static zam_t zam_ps[N],zamss[N-2],zamvss[N-4],zamas[N-6],zam_mks[N-6]; static zam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zam_init(void){if(init)return -1;st.n_zamp=0;st.n_zame=0;st.n_zam2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zam_ps[i].active=0;for(int i=0;i<N-2;i++)zamss[i].active=0;for(int i=0;i<N-4;i++)zamvss[i].active=0;for(int i=0;i<N-6;i++)zamas[i].active=0;for(int i=0;i<N-6;i++)zam_mks[i].active=0;init=1;ps("[ZAM] Zamia initialized\n");return 0;}
int zam_planning(int t,int c,int a,int b,int d,int e,int y){return add(zam_ps,&st.n_zamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zam_execution(int t,int c,int a,int b,int d,int e,int y){return add(zamss,&st.n_zame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zam_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zamvss,&st.n_zam2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zam_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zamas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zam_market(int t,int c,int a,int b,int d,int e,int y){return add(zam_mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zam_report(void){ps("[ZAM] Zamp: ");pi(st.n_zamp);ps(" PCS=");pi(st.t_f1);ps("\nZame: ");pi(st.n_zame);ps(" PCS=");pi(st.t_f2);ps("\nZamv: ");pi(st.n_zam2);ps(" PCS=");pi(st.t_f3);ps("\nZamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zam_state(void){ps("[ZAM] Zamp=");pi(st.n_zamp);ps(" Zame=");pi(st.n_zame);ps(" Zamv=");pi(st.n_zam2);ps(" Zamc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zamia Admin Demo ===\n\n");zam_init();
ps("Zamia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zam_planning(t,c,964+(i*17),953+(i*14),933+(i*10),915+(i*6),2020+(i%5));}
ps("\nZamia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zam_execution(t,c,953+(i*15),942+(i*12),924+(i*8),911+(i*5),2021+(i%4));}
ps("\nZamia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zam_evaluation(t,c,945+(i*13),934+(i*10),918+(i*7),907+(i*4),2022+(i%3));}
ps("\nZamia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zam_accessory(t,c,937+(i*11),928+(i*9),914+(i*6),904+(i*3),2023+(i%2));}
ps("\nZamia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zam_market(t,c,931+(i*9),922+(i*7),909+(i*5),901+(i*3),2024);}
ps("\n");zam_report();zam_state();ps("\n=== Demo Complete ===\n");return 0;}
