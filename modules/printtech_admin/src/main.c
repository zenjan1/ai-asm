/* printtech_admin: Printing technology administration (v1.0)
 * Book printing, packaging printing, commercial printing, digital printing, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prt_t;
typedef struct{int n_bk,n_pk,n_cm,n_dg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prt_state_t;
static prt_t bks[N],pks[N-2],cms[N-4],dgs[N-6],mks[N-6]; static prt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prt_init(void){if(init)return -1;st.n_bk=0;st.n_pk=0;st.n_cm=0;st.n_dg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bks[i].active=0;for(int i=0;i<N-2;i++)pks[i].active=0;for(int i=0;i<N-4;i++)cms[i].active=0;for(int i=0;i<N-6;i++)dgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PRT] Printtech initialized\n");return 0;}
int prt_book(int t,int c,int a,int b,int d,int e,int y){return add(bks,&st.n_bk,&st.t_f1,N,t,c,a,b,d,e,y);}
int prt_package(int t,int c,int a,int b,int d,int e,int y){return add(pks,&st.n_pk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prt_commercial(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prt_digital(int t,int c,int a,int b,int d,int e,int y){return add(dgs,&st.n_dg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prt_report(void){ps("[PRT] Bk: ");pi(st.n_bk);ps(" PCS=");pi(st.t_f1);ps("\nPk: ");pi(st.n_pk);ps(" PCS=");pi(st.t_f2);ps("\nCm: ");pi(st.n_cm);ps(" PCS=");pi(st.t_f3);ps("\nDg: ");pi(st.n_dg);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prt_state(void){ps("[PRT] Bk=");pi(st.n_bk);ps(" Pk=");pi(st.n_pk);ps(" Cm=");pi(st.n_cm);ps(" Dg=");pi(st.n_dg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Printing Tech Admin Demo ===\n\n");prt_init();
ps("Book printing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prt_book(t,c,247+(i*17),232+(i*14),212+(i*10),194+(i*6),2020+(i%5));}
ps("\nPackaging printing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prt_package(t,c,236+(i*15),222+(i*12),204+(i*8),191+(i*5),2021+(i%4));}
ps("\nCommercial printing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prt_commercial(t,c,228+(i*13),214+(i*10),198+(i*7),187+(i*4),2022+(i%3));}
ps("\nDigital printing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prt_digital(t,c,220+(i*11),208+(i*9),194+(i*6),184+(i*3),2023+(i%2));}
ps("\nPrinting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prt_market(t,c,214+(i*9),203+(i*7),190+(i*5),182+(i*3),2024);}
ps("\n");prt_report();prt_state();ps("\n=== Demo Complete ===\n");return 0;}
