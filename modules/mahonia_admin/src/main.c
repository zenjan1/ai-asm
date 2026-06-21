/* mahonia_admin: Mahonia management technology administration (v1.0)
 * Mahonia planning, mahonia execution, mahonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mhn_t;
typedef struct{int n_mhp,n_mhe,n_mhv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mhn_state_t;
static mhn_t mhnps[N],mhnss[N-2],mhnvss[N-4],mhnas[N-6],mhn_mks[N-6]; static mhn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mhn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mhn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MHN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mhn_init(void){if(init)return -1;st.n_mhp=0;st.n_mhe=0;st.n_mhv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mhnps[i].active=0;for(int i=0;i<N-2;i++)mhnss[i].active=0;for(int i=0;i<N-4;i++)mhnvss[i].active=0;for(int i=0;i<N-6;i++)mhnas[i].active=0;for(int i=0;i<N-6;i++)mhn_mks[i].active=0;init=1;ps("[MHN] Mahonia initialized\n");return 0;}
int mhn_planning(int t,int c,int a,int b,int d,int e,int y){return add(mhnps,&st.n_mhp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mhn_execution(int t,int c,int a,int b,int d,int e,int y){return add(mhnss,&st.n_mhe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mhn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mhnvss,&st.n_mhv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mhn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mhnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mhn_market(int t,int c,int a,int b,int d,int e,int y){return add(mhn_mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mhn_report(void){ps("[MHN] Mhp: ");pi(st.n_mhp);ps(" PCS=");pi(st.t_f1);ps("\nMhe: ");pi(st.n_mhe);ps(" PCS=");pi(st.t_f2);ps("\nMhv: ");pi(st.n_mhv);ps(" PCS=");pi(st.t_f3);ps("\nMhc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mhn_state(void){ps("[MHN] Mhp=");pi(st.n_mhp);ps(" Mhe=");pi(st.n_mhe);ps(" Mhv=");pi(st.n_mhv);ps(" Mhc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mahonia Admin Demo ===\n\n");mhn_init();
ps("Mahonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mhn_planning(t,c,835+(i*17),824+(i*14),804+(i*10),786+(i*6),2020+(i%5));}
ps("\nMahonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mhn_execution(t,c,824+(i*15),813+(i*12),795+(i*8),782+(i*5),2021+(i%4));}
ps("\nMahonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mhn_evaluation(t,c,816+(i*13),805+(i*10),789+(i*7),778+(i*4),2022+(i%3));}
ps("\nMahonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mhn_accessory(t,c,808+(i*11),799+(i*9),785+(i*6),775+(i*3),2023+(i%2));}
ps("\nMahonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mhn_market(t,c,802+(i*9),793+(i*7),780+(i*5),772+(i*3),2024);}
ps("\n");mhn_report();mhn_state();ps("\n=== Demo Complete ===\n");return 0;}
