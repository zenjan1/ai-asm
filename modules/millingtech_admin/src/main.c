/* millingtech_admin: Milling technology administration (v1.0)
 * Face milling, profile milling, cavity milling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mlg_t;
typedef struct{int n_fc,n_pr,n_cv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mlg_state_t;
static mlg_t fcs[N],prs[N-2],cvs[N-4],acs[N-6],mks[N-6]; static mlg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mlg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mlg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mlg_init(void){if(init)return -1;st.n_fc=0;st.n_pr=0;st.n_cv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fcs[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)cvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MLG] Millingtech initialized\n");return 0;}
int mlg_face(int t,int c,int a,int b,int d,int e,int y){return add(fcs,&st.n_fc,&st.t_f1,N,t,c,a,b,d,e,y);}
int mlg_profile(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mlg_cavity(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mlg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mlg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mlg_report(void){ps("[MLG] Fc: ");pi(st.n_fc);ps(" PCS=");pi(st.t_f1);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f2);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mlg_state(void){ps("[MLG] Fc=");pi(st.n_fc);ps(" Pr=");pi(st.n_pr);ps(" Cv=");pi(st.n_cv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Milling Tech Admin Demo ===\n\n");mlg_init();
ps("Face milling...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mlg_face(t,c,317+(i*17),302+(i*14),282+(i*10),264+(i*6),2020+(i%5));}
ps("\nProfile milling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mlg_profile(t,c,306+(i*15),292+(i*12),274+(i*8),261+(i*5),2021+(i%4));}
ps("\nCavity milling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mlg_cavity(t,c,298+(i*13),284+(i*10),268+(i*7),257+(i*4),2022+(i%3));}
ps("\nMilling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlg_accessory(t,c,290+(i*11),278+(i*9),264+(i*6),254+(i*3),2023+(i%2));}
ps("\nMilling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlg_market(t,c,284+(i*9),273+(i*7),260+(i*5),252+(i*3),2024);}
ps("\n");mlg_report();mlg_state();ps("\n=== Demo Complete ===\n");return 0;}
